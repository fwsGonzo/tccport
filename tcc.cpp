#include "tcc.hpp"
#include <cassert>

static void tcc_error(void* opaque, const char* msg)
{
  auto* tcc = (TCC*) opaque;
  if (tcc->on_error) tcc->on_error(msg);
}

TCC::TCC()
{
  this->state = tcc_new();
  assert(state);

  int res = tcc_set_output_type(this->state, TCC_OUTPUT_MEMORY);
  assert(res == 0);

  tcc_set_error_func(this->state, this, tcc_error);
}
TCC::~TCC() {
  tcc_delete(this->state);
}

bool TCC::add_sym(const std::string& name, void* symbol)
{
  int res = tcc_add_symbol(this->state, name.c_str(), symbol);
  return res == 0;
}

void* TCC::get_sym(const std::string& name)
{
  return tcc_get_symbol(state, name.c_str());
}

void TCC::compile(const std::string& code)
{
  int res;
  res = tcc_compile_string(state, code.c_str());
  if (res == -1) throw std::runtime_error("compilation failed");
  
  int size = tcc_relocate(state, nullptr);
  if (size == -1) throw std::runtime_error("relocation failed");
  
  delete[] this->text;
  this->text = new char[size];
  res = tcc_relocate(state, this->text);
  if (res == -1) throw std::runtime_error("relocation failed");
}
void TCC::compile(const std::string& code, error_func_t func)
{
  this->on_error = func;
  compile(code);
}
