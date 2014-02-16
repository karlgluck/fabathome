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
#include "confirm.h"
#include <stdio.h>  // printf
#include <QDebug>
#include <QMessageBox>
#include <QApplication> // for clipboard access
#include <QClipboard>



//----[  _confirm_failed_fn  ]-------------------------------------------------
void _confirm_failed_fn(const char* expression,
                        const char* file,
                        unsigned int line,
                        bool* sentinel) {
  char message[4096];

  // this message prints to a console window
  sprintf(message,
          "confirm(%s)\nFile:\t%s\nLine:\t%ui\n",
          expression,
          file,
          line);
  qWarning() << "Debug Error:  " << message;

  if (*sentinel) {

    sprintf(
      message,
      "Debug Error!\r\n\r\nThis program has encountered an error:"\
      "\r\n\r\nFailed:\t%s\r\nFile:\t%s\r\nLine:\t%ui\r\n\r\n\r\n"\
      "Press Ignore to continue normally and avoid seeing this message "\
      "again.\r\n\r\nTo open a debugger and examine the problem, press Retry."\
      "Select Abort to immediately end the program.\r\n\r\nThis text will be "\
      "copied to your clipboard.",
      expression,
      file,
      line);

    QApplication::clipboard()->setText(message);
    QMessageBox message_box(QMessageBox::Critical,
                            "Debug Error",
                            message,
                            QMessageBox::Abort|QMessageBox::Retry|QMessageBox::Ignore);
    message_box.setWindowModality(Qt::ApplicationModal);

    switch (message_box.exec()) {
    default:
    case QMessageBox::Ignore:
      // ignore this in the future
      *sentinel = false;
      qWarning() << message << "; user chose to ignore";
      break;
    case QMessageBox::Abort:
      // exit the application
      qCritical() << message;
      qFatal("confirm() failed; user chose to exit"); // this should exit...
      exit(0); // ... but we'll just be sure
      break;
    case QMessageBox::Retry:
      // continue on our way; the macro will invoke a breakpoint
      qWarning() << message << "; user chose to breakpoint";
      break;
    }
  }
}
