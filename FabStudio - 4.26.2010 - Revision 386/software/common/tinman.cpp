/*---------------------------------------------------------------------------*\
 Copyright (c) 2009, Karl Gluck (kwg8@cornell.edu)  All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the <organization> nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNERS OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\*---------------------------------------------------------------------------*/
#include "tinman.h"
#include <stdio.h>


//----[  staticInstance  ]-----------------------------------------------------
Tinman& Tinman::staticInstance(const char* test_name,
                              const char* file) {
  static Tinman t;
  t.next(test_name,file);
  return t;
}



//----[  next  ]---------------------------------------------------------------
void Tinman::next(const char* test_name, const char* file) {
  next_test_name_ = test_name;
  next_file_ = file;
}

void Tinman::operator = (bool rhs) {
  std::string test = next_test_name_ + " (" + next_file_ + ")";
  if (!rhs) {
    failed_.push_back(test);
  } else {
    succeeded_.push_back(test);
  }
}



//----[  buildReport  ]--------------------------------------------------------
const std::string& Tinman::buildReport() {
  if (report_.empty()) {
    char header[256];
    size_t total_tests = failed_.size() + succeeded_.size();
    sprintf(header,
            "%4lu total tests\n%4lu succeeded\n%4lu failed\n\n",
            total_tests,
            succeeded_.size(),
            failed_.size());
    report_.assign(header);
    typedef std::vector<std::string>::iterator Iter;
    report_ += "Failed:";
    if (failed_.empty()) {
      report_ += "\n  None.";
    } else {
      for (Iter f = failed_.begin(); f != failed_.end(); ++f) {
        report_ += std::string("\n  ") + *f;
      }
    }
    report_ += "\n\nSucceeded:";
    if (succeeded_.empty()) {
      report_ += "\n  None.";
    } else {
      for (Iter s = succeeded_.begin(); s != succeeded_.end(); ++s) {
        report_ += std::string("\n  ") + *s;
      }
    }
  }
  return report_;
}
