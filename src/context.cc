#include "context.h"

namespace co {

namespace {

void stack_init(stack_t &stack) {
  stack.ss_sp = malloc(SIGSTKSZ);
  stack.ss_size = SIGSTKSZ;
  stack.ss_flags = 0;
}

void stack_drop(stack_t &stack) { free(stack.ss_sp); }

void invoke(std::function<void()> *f) { (*f)(); }

} // end anonymous namespace

context_t::context_t(std::function<void()> &&f) : f_(f) {
  getcontext(ucontext());
  stack_init(ucontext()->uc_stack);
  ucontext()->uc_link = main_ucontext();
  makecontext(ucontext(), reinterpret_cast<void (*)()>(invoke), 1, &f_);
}

context_t::~context_t() { stack_drop(ucontext()->uc_stack); }

void context_t::swap_in() { swapcontext(main_ucontext(), ucontext()); }

void context_t::swap_out() { swapcontext(ucontext(), main_ucontext()); }

ucontext_t *context_t::ucontext() { return &ucontext_; }

ucontext_t *context_t::main_ucontext() {
  thread_local static ucontext_t instance;
  return &instance;
}

} // end namespace co
