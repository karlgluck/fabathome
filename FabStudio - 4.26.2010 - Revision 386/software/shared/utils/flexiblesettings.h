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
#pragma once
#include <QString>
#include <QMap>


/**
 * Stores values internally paired with names and provides a way to access
 * that value.  If the value is set and obtained in a single type, it is
 * guaranteed that the value will be bitwise-identical (for raw types)
 * or be a copy-constructed version (for class types).
 * If a value is set as one type but obtained as another, this class
 * performs the most logical conversion and returns the result.
 * If the named parameter does not exist, or if conversion fails, the get()
 * method will return the default value provided as its parameter.
 * 
 */
class FlexibleSettings {

  /**
   * Structure used internally to save the settings
   */
  struct SettingValue {
  
    /**
     * Defines the type in which a setting was last defined
     */
    enum NativeType {
      FLOAT,
      INT,
      BOOL,
      STRING,
    };

    // Which local member contains the true representation of this setting
    NativeType type;

    // basic-type values
    union {
      float f;
      int i;
      bool b;
    };

    // value of this setting, if it is a string, or
    // the string-equivalent for lexical conversion
    QString string;
  };

public:

  // Changes the value associated with a name.  Overwrites anything
  // that currently exists.
  void set(const QString& name, float value);
  void set(const QString& name, int value);
  void set(const QString& name, bool value);
  void set(const QString& name, const QString& value);

  // Returns true if the setting's name has been mapped by one of the
  // set() methods.
  bool isSet(const QString& name) const;

  // Returns the setting, if it exists, or the default value
  float get(const QString& name, float default_value);
  int get(const QString& name, int default_value);
  bool get(const QString& name, bool default_value);
  QString get(const QString& name, const QString& default_value);

private:
  QMap<QString,SettingValue> values_;
};
