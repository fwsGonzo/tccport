// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <os>
#include <net/inet4>
#include "tcc.hpp"

extern "C"
void* dlopen(const char *filename, int flag)
{
  printf("dlopen called for %s with flag %#x\n", filename, flag);
  return nullptr;
}
extern "C"
char* dlerror(void)
{
  printf("dlerror\n");
  return nullptr;
}
extern "C"
void* dlsym(void*, const char* symbol)
{
  printf("dlsym\n");
  return nullptr;
}
extern "C"
int   dlclose(void*)
{
  printf("dlclose\n");
  return 0;
}

int execvp(const char *file, char *const argv[])
{
  printf("execvp called: %s\n", file);
}


void Service::start()
{
  // add own serial out after service start
  OS::add_stdout_default_serial();

  // show that we are starting :)
  printf("*** " SERVICE_NAME " starting up...\n");
  //_enable_heap_debugging_verbose(1);

  // default configuration (with DHCP)
  auto& inet = net::Inet4::ifconfig<>(10);
  inet.network_config(
      {  10, 0,  0, 42 },  // IP
      { 255,255,255, 0 },  // Netmask
      {  10, 0,  0,  1 },  // Gateway
      {  10, 0,  0,  1 }); // DNS

  /// TCC
  TCC tcc;
  tcc.add_sym("printf", (void*) printf);
  
  try {
    tcc.compile(R"HEYHEY(
int printf(const char*, ...);

void other_func(int param)
{
  printf("printf %d works too!!!!\n", param);
}

int test(int param)
{
  other_func(param);
  return 666;
}
    )HEYHEY",
    [] (const char* msg) {
      printf("\n\t%s\n", msg);
    });
    // call test
    int res = tcc.call("test", 33);
    printf("call() res: %d\n", res);
  }
  catch (std::runtime_error err)
  {
    printf("TCC: %s\n", err.what());
  }
}
