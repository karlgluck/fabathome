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

#include "path.h"
#include <common/confirm.h>
#include <common/todo.h>



//----[  Path  ]---------------------------------------------------------------
Path::Path() {
}

//----[  Path  ]---------------------------------------------------------------
Path::Path(const Path* other_path) {
  points_ += other_path->points_;
}


//----[  Path  ]---------------------------------------------------------------
Path::Path(const FAHLoopInXYPlane& loop) {
  confirm(loop.points.isEmpty() == false) then {
    for (int i = 0; i < loop.points.size(); ++i) {
      addPathPointEnd(loop.points[i]);
    }
    addPathPointEnd(loop.points[0]);
  }
}



//----[  points  ]-------------------------------------------------------------
const QVector<FAHVector3>& Path::points() const {
  return points_;
}


void Path::addPathPointStart(const FAHVector3& point) {
  points_.push_front(point);
}

void Path::addPathPointEnd(const FAHVector3& point) {
  points_.push_back(point);
}


//----[  limits  ]-------------------------------------------------------------
void Path::limits(FAHVector3* min, FAHVector3* max) {
  if (points_.size() < 1) return;
  min->set(points_.at(0));
  max->set(points_.at(0));
  for (int i = 1; i < points_.size(); ++i) {
    min->min(points_[i]);
    max->max(points_[i]);
  }
}



//----[  length  ]-------------------------------------------------------------
FAHFloat Path::length() {
  FAHFloat length = FAHFloat(0.0);
  for (int i = 1; i < points_.size(); ++i) {
    const FAHVector3& last = points_.at(i-1);
    const FAHVector3& current = points_.at(i);
    length += current.copy().sub(last).length();
  }
  return length;
}



//----[  start  ]--------------------------------------------------------------
const FAHVector3& Path::start() const {
  confirm(points_.size() > 1);
  return points_.at(0);
}


//----[  end  ]----------------------------------------------------------------
const FAHVector3& Path::end() const {
  confirm(points_.size() > 1);
  return points_.at(points_.size()-1);
}


bool Path::crossedBy(const FAHLine& segment) const {
  for (int i = 1; i < points_.size(); ++i) {
    FAHLine line(points_.at(i-1), points_.at(i));
    FAHVector3 pt;
    if (line.intersectSegmentWithSegment2DXY(segment, &pt) &&
        !segment.isEndpoint(pt)) {
      return true;
    }
  }
  return false;
}



void Path::append(const Path& other) {
  confirm(&other != this) else return;
  for (int i = 0; i < other.points_.size(); ++i) {
    points_.push_back(other.points_.at(i));
  }
}

void Path::append(const QVector<FAHVector3>& other) {
  points_ += other;
}

void Path::appendReversed(const Path& other) {
  confirm(&other != this) else return;
  for (int i = other.points_.size() - 1; i >= 0; --i) {
    points_.push_back(other.points_.at(i));
  }
}

void Path::appendReversed(const QVector<FAHVector3>& other) {
  for (int i = other.size() - 1; i >= 0; --i) {
    points_.push_back(other.at(i));
  }
}

void Path::prepend(const Path& other) {
  confirm(&other != this) else return;
  for (int i = other.points_.size() - 1; i >= 0; --i) {
    points_.push_front(other.points_.at(i));
  }
}

void Path::prependReversed(const Path& other) {
  confirm(&other != this) else return;
  for (int i = 0; i < other.points_.size(); ++i) {
    points_.push_front(other.points_.at(i));
  }
}

void Path::clear() {
  points_.clear();
}
