#include "tinycc/libtcc.h"
#include <delegate>
#include <stdexcept>
#include <string>

struct TCC
{
  typedef delegate<void(const char*)> error_func_t;
  
  void compile(const std::string& code);
  void compile(const std::string& code, error_func_t);
  
  // add symbol that TCC code has access to
  template <typename F>
  bool  add_sym(const std::string& name, F func);
  bool  add_sym(const std::string& name, void*);
  // retrieve location of a TCC-side symbol
  void* get_sym(const std::string& name);
  
  // call a function that looks like T **that returns int**
  // throws runtime_error when name doesn't exist
  template <typename... Args>
  inline int call(const std::string& name, Args&&...);
  
  TCC();
  ~TCC();
  
  error_func_t on_error;
private:
  TCCState* state = nullptr;
  char*     text  = nullptr;
};

template <typename... Args>
inline int TCC::call (const std::string& name, Args&&... args)
{
  auto* func = get_sym(name);
  if (!func) throw std::runtime_error("Symbol does not exist");
  
  return ((int(*)(...))func)(std::forward<Args>(args)...);
}

template <typename F>
bool TCC::add_sym(const std::string& name, F func)
{
  return add_sym(name, (void*) func);
}
