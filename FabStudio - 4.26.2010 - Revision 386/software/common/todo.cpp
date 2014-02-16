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
#include "todo.h"
#ifdef TODO_IS_ENABLED

#include <vector>     // for holding all todo()s when printing report
#include <algorithm>  // for sorting the report's vector by # of times hit
#include <string>
#include <time.h>     // for clock()
#include <QTextStream>// QString formatted output

//----[  LinkStaticTodoStructIfValid  ]----------------------------------------
// Adds the TodoStruct ts to a linked list of structures local to this method,
// as long as ts != NULL.
// The head of the list is returned.
TodoStruct* LinkStaticTodoStructIfValid(TodoStruct* ts) {
  static TodoStruct* first = 0;
  if (ts) {
    ts->next = first;
    first = ts;
  }
  return first;
}



//----[  TodoStruct  ]---------------------------------------------------------
TodoStruct::TodoStruct(const char* file,
                       unsigned int line,
                       const char* who,
                       const char* what)
    : file(file), line(line), who(who), what(what), hits(0), local_ms(0) {
  LinkStaticTodoStructIfValid(this);
}



//----[  hit  ]----------------------------------------------------------------
void TodoStruct::hit() {
  ++hits;
}



//----[  TodoTimer  ]----------------------------------------------------------
TodoTimer::TodoTimer(TodoStruct* todo_struct) : todo_struct(todo_struct) {
  start = (int)clock();
}



//----[  ~TodoTimer  ]---------------------------------------------------------
TodoTimer::~TodoTimer() {
  todo_struct->local_ms += clock() - start;
}

//----[  TodoStructSTLSortPredicateByHits  ]-----------------------------------
//----[  TodoStructSTLSortPredicateByTime  ]-----------------------------------
// sorts TodoStruct pointers in an STL container by descending number of hits
// when used as the predicate for std::sort
bool TodoStructSTLSortPredicateByHits(const TodoStruct* lhs, const TodoStruct* rhs) {
  return lhs->hits > rhs->hits;
}
bool TodoStructSTLSortPredicateByTime(const TodoStruct* lhs, const TodoStruct* rhs) {
  return lhs->local_ms > rhs->local_ms;
}



//----[  TodoReport  ]---------------------------------------------------------
TodoReport::TodoReport() : text(0) {
}



//----[  TodoReport  ]---------------------------------------------------------
TodoReport::TodoReport(TodoReport& rhs) : text(rhs.text) {
}


//----[  ~TodoReport  ]--------------------------------------------------------
TodoReport::~TodoReport() {
}



//----[  generateReport  ]-----------------------------------------------------
TodoReport TodoReport::generateReport() {
  QString text;
  TodoStruct* next_todo = LinkStaticTodoStructIfValid(NULL);
  typedef std::vector<TodoStruct*> TodoStructs;
  TodoStructs all;
  while (next_todo) { // add every TodoStruct to the vector
    all.push_back(next_todo);
    next_todo = next_todo->next;
  }

  for (int report_type = 0; report_type < 2; ++report_type) {
    switch (report_type) {
    case 0:
      text.append("\n----[  todo() report by hits  ]----------------------------------\n");
      std::sort(all.begin(), all.end(), TodoStructSTLSortPredicateByHits);
      break;
    case 1:
      text.append("\n----[  todo() report by time  ]----------------------------------\n");
      std::sort(all.begin(), all.end(), TodoStructSTLSortPredicateByTime);
      break;
    }
    for (TodoStructs::iterator i = all.begin(); i != all.end(); ++i) {
      TodoStruct* ts = *i;
      QString line;
      QTextStream(&line)
        << ts->file
        << "(" << ts->line << "):  \""
        << ts->what
        << "\" - " << ts->who
        << " (hit " << ts->hits << " time" << (ts->hits==1 ? "":"s")
        << " - " << ts->local_ms << "ms)\n";
      text.append(line);
    }
  }
  text.append("-----------------------------------------------------------------\n");

  // allocate output buffer
  TodoReport tr;
  tr.text = text;
  return tr;
}



#endif
