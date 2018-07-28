#ifndef XUV_CONTEXT_H
#define XUV_CONTEXT_H

#include <functional>
#include <ucontext.h>

namespace co {

class context_t {

public:

  explicit
  context_t(std::function<void()> &&f);

  context_t() = delete;

  ~context_t();

  void swap_in();

  void swap_out();

private:

  static
  ucontext_t *main_ucontext();

  ucontext_t *ucontext();

  ucontext_t ucontext_{};

  std::function<void()> f_;

};

} // end namespace co

#endif // XUV_CONTEXT_H
