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
#include "tcc.hpp"

void Service::start()
{
  // add own serial out after service start
  OS::add_stdout_default_serial();

  // show that we are starting :)
  printf("*** " SERVICE_NAME " starting up...\n");

  TCC tcc;
  tcc.add_sym("printf", (void*) printf);
  
  try {
    // compile multiline string
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
    // compilation error handler
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

int execvp(const char *file, char *const argv[])
{
  printf("execvp called: %s\n", file);
}
