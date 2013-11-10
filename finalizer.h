#ifndef FINALIZE_H_
#define FINALIZE_H_

#include <functional>

// Using C++11 RAII idiom it's possible to create a finalizer object that
// performs special task when going out of scope.
//
// It basically uses the same idiom as "safe" pointers in C++11.
// Although user specifies what to do upon object destruction.
class Finalizer {
 public:
  // NOTE(Olster): Really funky stuff going on here. This piece has been put
  // together after investigatins std::thread constructor that accepts
  // lambdas.

  template <class Func, class... Args>
  Finalizer(Func&& func, Args&&... args) {
    m_func = std::bind(std::forward<Func>(func),
                       std::forward<Args>(args)...);
  }

   ~Finalizer() {
     m_func();
   }

   /*
   Finalizer() = delete;
   Finalizer(const Finalizer& other) = delete;
   Finalizer(Finalizer&& other) = delete;
   Finalizer& operator=(const Finalizer& other) = delete;
   Finalizer& operator=(Finalizer&& other) = delete;
   */

 private:
  // This function is called in the destructor, its return type doesn't matter
  // because it will never be used anywhere.
  std::function<void()> m_func;
};

#ifdef ALLOW_FINALIZER_MACRO
// This also makes sure that we have only one finalizer.
// That is, obviously, if user uses only macro and doesn't instantiate object.
#define finalize(func, ...) Finalizer fin(func, ##__VA_ARGS__)
#endif

#endif // FINALIZE_H_
