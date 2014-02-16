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
#pragma once

#define CONFIRM_ENABLE

// assert() is commonly used to check assumptions made by the programmer, and
// will generate a run-time error when the condition passed as a parameter
// fails. The confirm() macro works like a better version of assert() with a
// slightly different release-mode behavior.  This is best demonstrated with an
// example:
/*

bool isEqualToZero(int* ptr) {
  confirm (ptr != NULL) else return false;
  return (*ptr) == 0;
}

*/
// In debug mode, this compiles to the equivalent of:
// 
//  bool isEqualToZero(int* ptr) {
//    if (ptr != NULL) {
//      static bool ignore_always = false;
//      if (!ignore_always) ignore_always = assert_dialog("ptr != NULL");
//      return false;
//    }
//    return (*ptr) == 0;
//  }
// 
//  In release mode, this code becomes:
// 
//  bool isEqualToZero(int* ptr) {
//    if (ptr == NULL) return false;
//    return (*ptr) == 0;
//  }
// 
// 
// If the above code were written with assert(ptr != NULL), the release-mode
// version would crash if the pointer were ever invalid since assert compiles
// to (noop), and the program would dereference null.  In many cases, such as
// the above example, there is some default behavior that can be executed so
// that the program doesn't have to crash in the case of an error.
// Additionally, when you want to use an assert() you can confirm() without the
// else clause to take advantage of its always-ignore feature.  confirm() can
// also be set to a logging release-mode, which behaves exactly like release-
// mode but will create a debug console message each time a confirm() is hit.
//
// There is one more trick to using confirm().  Sometimes you want to make sure
// a variable does have a certain value, then do something.  This is done in
// the following example:
/*

void setPointerValue(int* ptr, int value) {
 confirm (ptr != NULL) then {
   *ptr = value;
 }
}

*/
// The use of "then" can make your code very clear.  This compiles in release
// and debug mode to what you would expect:  both will execute then { ... }
// only when ptr != NULL.  If ptr == NULL in debug mode, a run-time error will
// be raised.
// 
// Like with assert(), do not use confirm() to check for erroneous user input,
// file data, or other information that could be invalid as part of the
// program's normal run-time behavior.  It should be used exclusively to check
// program logic.
// 
// Defend your code!  Use confirm()!


// Important things to note:
//  - to use the default confirm() failure handler, add confirm.cpp to your project
//  - confirm(), by default, will display messages in debug mode but not release mode
//  - to force confirm() messages to appear in release mode, define CONFIRM_ENABLE
//  - to turn all confirm() messages off in debug mode, define CONFIRM_DISABLE
//  - to log confirm() failures to the debugger and stdout, but avoid the message-box
//    prompt and breakpoint behavior for all failures (useful for userless apps like
//    a remote server) define CONFIRM_IGNORE_ALWAYS


#if (!defined(CONFIRM_DISABLE) &&\
     (defined(_DEBUG) || defined(DEBUG))) || defined(CONFIRM_ENABLE)

#define _confirm_sentinel_inner(name,line)  name##line
#define _confirm_sentinel(line)  _confirm_sentinel_inner(_confirm_sentinel_at_line_,line)


void _confirm_failed_fn(const char* expression, const char* file, unsigned int line, bool* sentinel);

#ifdef CONFIRM_IGNORE_ALWAYS
#define CONFIRM_DEFAULT_SENTINEL_VALUE  false
#else
#define CONFIRM_DEFAULT_SENTINEL_VALUE  true
#endif

inline void breakpoint() {
#if defined(_MSC_VER)
    __asm int 3
#else
    __asm("int3");
#endif
}
#define _confirm_failed(expression)   _confirm_failed_fn(expression, __FILE__, __LINE__, &_confirm_sentinel(__LINE__))
#define _confirm_ignored(expression)  _confirm_ignored_fn(expression, __FILE__, __LINE__)
#define confirm(expression)   ;;;static bool _confirm_sentinel(__LINE__) = CONFIRM_DEFAULT_SENTINEL_VALUE;\
                              if ((!(expression) && (_confirm_failed(#expression),1)))\
                              if (_confirm_sentinel(__LINE__)&&(breakpoint(),0)) {}

#else
#define confirm(expression)   if (!(expression)) if (false) {}
#endif

#define then                  else {} else
