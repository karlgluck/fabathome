/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)

 All rights reserved.

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
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QSplashScreen>

#include "common/todo.h"

QFile debug_log("log.html");

/**
 * Hooks into QT's default debugging output so that it can be redirected to
 * the debug-log file.
 */
void fabStudioDebugLogOutput(QtMsgType type, const char *msg);

/**
 * Invoked when the application successfully terminates to output information
 * about what fixable entries were hit during execution.
 */
void logTodoReport(const QString& text);

/**
 * Entry point to the FabStudio application.
 */
int main(int argc, char *argv[]) {

  // Initialize the application log
  debug_log.open(QIODevice::WriteOnly | QIODevice::Append);
  QTextStream(&debug_log) << "<br/><br/><br/><br/><br/><br/><hr><font color=\"black\">FabStudio Log File, Starting "
                          << QDate::currentDate().toString("yyyy-MM-dd")
                          << " at "
                          << QTime::currentTime().toString("hh:mm:ss")
                          << "</font><br/><br/>";
  qInstallMsgHandler(fabStudioDebugLogOutput);

  // Initialize the application
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("Fab@Home");
  QCoreApplication::setOrganizationDomain("fabathome.org");
  QCoreApplication::setApplicationName("Fab Studio");

  // Set up a splash screen
  QPixmap pixmap(":/media/fabstudio.png");
  QSplashScreen* splash = new QSplashScreen(pixmap);
  splash->show();

  // Load the application window
  MainWindow w;
  w.show();

  // Erase the splash screen
  splash->finish(&w);
  delete splash;

  // If this is the first time the app has been run,
  // pop up a window with some information.
  w.checkFirstExecution();

  // Execute the application
  int return_value = a.exec();

  // Print the todo report
  todoReport(logTodoReport);

  // Return the execution result code
  return return_value;
}


void fabStudioDebugLogOutput(QtMsgType type, const char *msg) {

  switch (type) {
  case QtDebugMsg:
    debug_log.write("<pre><font color=\"black\">");
    debug_log.write(msg);
    debug_log.flush();
    break;
  case QtWarningMsg:
    debug_log.write("<pre><font color=\"orange\">");
    debug_log.write(msg);
    break;
  case QtCriticalMsg:
    debug_log.write("<pre><font color=\"red\">");
    debug_log.write(msg);
    break;
  case QtFatalMsg:
    debug_log.write("<pre><font color=\"red\">");
    debug_log.write(msg);
    break;
  }
  debug_log.write("</font></pre><br/>");
  if (type != QtDebugMsg) debug_log.flush();
  if (type == QtCriticalMsg ||
      type == QtFatalMsg) __asm("int3"); // let the debugger handle this problem!
  if (type == QtFatalMsg) abort();      // can't continue execution
}

void logTodoReport(const QString& text) {
  debug_log.write("<br/><br/><pre><font color=\"green\">");
  debug_log.write(text.toAscii());
  debug_log.write("</font></pre>");
}
