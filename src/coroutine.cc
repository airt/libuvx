#include "coroutine.h"
#include "context.h"
#include <cassert>
#include <climits>
#include <map>
#include <memory>

#define TRYDO(b)                                                               \
  do {                                                                         \
    try {                                                                      \
      b;                                                                       \
    } catch (...) {                                                            \
    }                                                                          \
  } while (0)

namespace co {

namespace {

constexpr coroutine_id_t INVALID_ID = 0;

enum class coroutine_state {
  running,
  suspended,
  terminated,
  waiting,
};

struct coroutine_t {

  coroutine_id_t id;
  coroutine_state state;
  context_t context;

  coroutine_t(coroutine_id_t id, task_t &&task)
      : id(id), state(coroutine_state::suspended), context([this, task] {
          TRYDO(task());
          running_coroutine_id() = INVALID_ID;
          this->state = coroutine_state::terminated;
        }) {}
};

struct scheduler_t {
  coroutine_id_t running_coroutine_id = INVALID_ID;
  std::map<coroutine_id_t, std::unique_ptr<coroutine_t>> coroutines;
};

scheduler_t &scheduler() {
  static scheduler_t instance;
  return instance;
}

coroutine_id_t gen_id() {
  static coroutine_id_t i = 0;
  if (i == INT_MAX) {
    i = 0;
  }
  return ++i;
}

coroutine_id_t gen_coroutine_id() {
  for (int i = 0; i <= INT_MAX; i++) {
    auto id = gen_id();
    if (scheduler().coroutines.count(id) == 0) {
      return id;
    }
  }
  assert(0);
  return INVALID_ID;
}

coroutine_t &get_coroutine(coroutine_id_t id) {
  assert(id != INVALID_ID);
  assert(scheduler().coroutines.count(id));
  return *scheduler().coroutines.at(id);
}

void cleanup_coroutines() {
  auto it = std::begin(scheduler().coroutines);
  while (it != std::end(scheduler().coroutines)) {
    if (it->second->state == coroutine_state::terminated) {
      it = scheduler().coroutines.erase(it);
    } else {
      it++;
    }
  }
}

void yield(coroutine_state next_state) {
  auto &coroutine = get_coroutine(running_coroutine_id());
  assert(coroutine.state == coroutine_state::running);
  running_coroutine_id() = INVALID_ID;
  coroutine.state = next_state;
  coroutine.context.swap_out();
}

void resume(coroutine_t &coroutine) {
  assert(running_coroutine_id() == INVALID_ID);
  assert(coroutine.state == coroutine_state::suspended);
  running_coroutine_id() = coroutine.id;
  coroutine.state = coroutine_state::running;
  coroutine.context.swap_in();
}

} // end anonymous namespace

// the returned id may be invalid
coroutine_id_t &running_coroutine_id() {
  return scheduler().running_coroutine_id;
}

coroutine_id_t create(task_t &&task) {
  auto id = gen_coroutine_id();
  scheduler().coroutines.emplace(
      id, std::make_unique<coroutine_t>(id, std::move(task)));
  return id;
}

// running >> suspended
void yield() { yield(coroutine_state::suspended); }

// suspended >> running
void resume(coroutine_id_t id) { resume(get_coroutine(id)); }

// running >> waiting
void wait() { yield(coroutine_state::waiting); }

// waiting >> suspended
void notify(coroutine_id_t id) {
  auto &coroutine = get_coroutine(id);
  assert(coroutine.state == coroutine_state::waiting);
  coroutine.state = coroutine_state::suspended;
}

void dispatch() {
  bool h = false;
  for (auto &t : scheduler().coroutines) {
    auto &coroutine = *t.second;
    if (coroutine.state == coroutine_state::suspended) {
      h = true;
      resume(coroutine);
    }
  }
  if (h) {
    dispatch();
  }
}

bool finished() {
  cleanup_coroutines();
  return scheduler().coroutines.empty();
}

} // end namespace co
