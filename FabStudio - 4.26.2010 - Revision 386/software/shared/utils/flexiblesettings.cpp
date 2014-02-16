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
#include "flexiblesettings.h"
#include "common/confirm.h"
#include <stdio.h>


void FlexibleSettings::set(const QString& name, float value) {
  SettingValue setting;
  setting.type = SettingValue::FLOAT;
  setting.f = value;
  setting.string.sprintf("%f", value);
  values_.insert(name, setting);
}

void FlexibleSettings::set(const QString& name, int value) {
  SettingValue setting;
  setting.type = SettingValue::INT;
  setting.i = value;
  setting.string.sprintf("%i", value);
  values_.insert(name, setting);
}

void FlexibleSettings::set(const QString& name, bool value) {
  SettingValue setting;
  setting.type = SettingValue::BOOL;
  setting.b = value;
  setting.string = value ? "1" : "0";
  values_.insert(name, setting);
}

void FlexibleSettings::set(const QString& name, const QString& value) {
  SettingValue setting;
  setting.type = SettingValue::STRING;
  setting.string = value;
  values_.insert(name, setting);
}

bool FlexibleSettings::isSet(const QString& name) const {
  return values_.contains(name);
}


float FlexibleSettings::get(const QString& name, float default_value) {
  QMap<QString,SettingValue>::const_iterator i = values_.find(name);
  if (i == values_.end()) {
    return default_value;
  } else {
    const SettingValue& setting = i.value();
    switch (setting.type) {
      case SettingValue::FLOAT:   return setting.f;
      case SettingValue::INT:     return float(setting.i);
      case SettingValue::BOOL:    return setting.b ? 1.0f : 0.0f;
      default:
        { confirm(!"unknown setting type"); }
      case SettingValue::STRING: {
        float v;
        sscanf(setting.string.toAscii().data(), "%f", &v);
        return v;
      }
    }
  }
}


int FlexibleSettings::get(const QString& name, int default_value) {
  QMap<QString,SettingValue>::const_iterator i = values_.find(name);
  if (i == values_.end()) {
    return default_value;
  } else {
    const SettingValue& setting = i.value();
    switch (setting.type) {
      case SettingValue::FLOAT:   return int(setting.f);
      case SettingValue::INT:     return setting.i;
      case SettingValue::BOOL:    return setting.b ? 1 : 0;
      default:
        { confirm(!"unknown setting type"); }
      case SettingValue::STRING: {
        int i;
        sscanf(setting.string.toAscii().data(), "%d", &i);
        return i;
      }
    }
  }
}


bool FlexibleSettings::get(const QString& name, bool default_value) {
  QMap<QString,SettingValue>::const_iterator i = values_.find(name);
  if (i == values_.end()) {
    return default_value;
  } else {
    const SettingValue& setting = i.value();
    switch (setting.type) {
      case SettingValue::FLOAT:   return setting.f > 0.0f;
      case SettingValue::INT:     return setting.i != 0;
      case SettingValue::BOOL:    return setting.b;
      default:
        { confirm(!"unknown setting type"); }
      case SettingValue::STRING: {
        int i;
        sscanf(setting.string.toAscii().data(), "%d", &i);
        return i != 0;
      }
    }
  }
}


QString FlexibleSettings::get(const QString& name, const QString& default_value) {
  QMap<QString,SettingValue>::const_iterator i = values_.find(name);
  if (i == values_.end()) {
    return default_value;
  } else {
    const SettingValue& setting = i.value();
    return setting.string;
  }
}



