/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2009, Karl Gluck (kwg8@cornell.edu)

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
#include "toolscriptsetting.h"
#include <math.h>

#include <QDomDocument>
#include <QtScript>


ToolScriptSetting::ToolScriptSetting(const QDomNode& sourceDomNode) {
  QDomNamedNodeMap attributes = sourceDomNode.attributes();
  name_ = sourceDomNode.nodeName();
  script_variable_ =
      attributes.contains("scriptVariable")
      ? attributes.namedItem("scriptVariable").nodeValue()
      : name_;
  text_ =
      attributes.contains("text")
      ? attributes.namedItem("text").nodeValue()
      : sourceDomNode.nodeName();
  units_ =
      attributes.contains("units")
      ? attributes.namedItem("units").nodeValue()
      : "units";

  bool ok;

  // get the default setting value
  ok = sourceDomNode.childNodes().size() == 1;
  if (ok) {
    QDomNode node = sourceDomNode.childNodes().at(0);
    value_ = node.nodeValue().toDouble(&ok);
  }
  if (!ok) value_ = 0.0;

}

const QString& ToolScriptSetting::name() const {
  return name_;
}

const QString& ToolScriptSetting::scriptVariable() const {
  return script_variable_;
}

const QString& ToolScriptSetting::text() const {
  return text_;
}

const QString& ToolScriptSetting::units() const {
  return units_;
}

QString ToolScriptSetting::valueString(bool append_units) const {
  QString value;
  value.sprintf("%f%s%s", value_, append_units ? " " : "", append_units ? units_.toAscii().data() : "");
  return value;
}
