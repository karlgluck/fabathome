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
#include "slicer.h"
#include "common/todo.h"
#include "slicerprogresscallback.h"
#include <QDebug>
#include "math.h"

/*

  THIS FILE HAS ONE OR MORE UNRESOLVED ISSUES (April 8 2010)
  =============================================================

  - The algorithm to find loops from segments is ~ O(n^3) n=segments

  - The algorithm to intersect each slicing plane with triangles uses no
    acceleration structure, and is ~ O(mn) m=slices n=triangles

  - Error codes and error reporting are not implemented

  - Sequencing is deprecated, it is now done by the fab file writer

 */

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


QStringList Slicer::alignmentStrings() {
  QStringList list;
  list.append("Tray");
  list.append("Top of Model");
  list.append("Bottom of Model");
  list.append("Center of Model");
  return list;
}

Slicer::Slicer() {
  slice_height = 0.1;
  alignment = kAlignBottom;
}

void Slicer::setAlignment(Alignment alignment) {
  this->alignment = alignment;
}

void Slicer::setSliceHeight(FAHFloat slice_height) {
  this->slice_height = slice_height;
}


inline FAHFloat min(FAHFloat a, FAHFloat b) {
  return a < b ? a : b;
}


inline FAHFloat max( FAHFloat a, FAHFloat b) {
  return a > b ? a : b;
}

bool Slicer::doSlicing(const AMFRegion* input_region,
                       SlicerProgressCallback* callback) {

  this->callback = callback;
  slicestack.clear();

  // problems with this in the past
  //jlz27 - does not work in Qt 4.6.2
  //confirm (_finite(slice_height) && !_isnan(slice_height)) else return false;

  QList<FAHTriangle> triangles;
  input_region->getTriangles(&triangles);

  if (triangles.isEmpty()) {
    if (callback) {
      callback->encounteredIssue("Region was empty");
    }
    return true;
  }

  FAHVector3 min, max;
  min.set(triangles.at(0).v[0]);
  max.set(triangles.at(0).v[0]);
  for (int i = 0; i < triangles.size(); ++i) {
    for (int j = 0; j < 3; ++j) {
      const FAHTriangle& triangle = triangles.at(i);
      min.min(triangle.v[j]);
      max.max(triangle.v[j]);
    }
  }

  QList<FAHFloat> slice_z;

  // This value is set to put the first slice at z = slice height
  // no matter where the alignment is specified to occur
  float z_adjustment;

  switch(alignment) {
  default:
    confirm(!"unknown value for alignment; defaulting to 'tray' alignment");

  case kAlignTray: {
    z_adjustment = 0.0f;
    FAHFloat z = slice_height;
    while (z < max.z) {
      slice_z.push_back(z);
      z += slice_height;
    }
    } break;

  case kAlignTop: {
    FAHFloat z = max.z - slice_height;
    while (z > min.z) {
      slice_z.push_front(z);
      z -= slice_height;
    }
    z_adjustment = slice_height-z;
  } break;

  case kAlignBottom: {
    FAHFloat z = min.z + slice_height;
    z_adjustment = slice_height-z;
    while (z < max.z) {
      slice_z.push_back(z);
      z += slice_height;
    }
  } break;

  case kAlignCenter: {
    FAHFloat z = (min.z + max.z) / 2;
    while (z > min.z) z -= slice_height;
    z_adjustment = slice_height-z;
    z += slice_height;
    while (z < max.z) {
      slice_z.push_back(z);
      z += slice_height;
    }
  } break;
  }

  if (callback) callback->setEstimatedNumberOfSlices(slice_z.size());

  for (int slice_index = 0; slice_index < slice_z.size(); ++slice_index) {
    QList<FAHLine> segments;
    const float current_z = slice_z.at(slice_index);

    // Form a list of all of the line segments in the slice by
    // intersecting the plane with triangles.
    todo("kwg8", "make this intersection test not check every single triangle in the source mesh");
    for (int j=0; j < triangles.size(); ++j) {
      FAHLine line;
      if (triangles.at(j).intersectXYPlaneAtZ(current_z, &line)) {
        line.a.z += z_adjustment;
        line.b.z += z_adjustment;
        segments.append(line);
      }
    }

    // no intersections? skip this plane.
    if (segments.isEmpty()) {
      QString description;
      QTextStream(&description) << "warning: slice " << slice_index << " is empty\n";
      if (callback) callback->encounteredIssue(description);
      continue;
    }

    // Discover all of the loops in the segments
    QVector<FAHLoopInXYPlane> loops;
    findLoops(segments, &loops);

    // pull out all of the outer boundaries into regions
    QVector<SliceMaterialRegion> regions;
    for (int i = loops.size() - 1; i >= 0; --i) {
      const FAHLoopInXYPlane& loop = loops.at(i);
      switch (loop.sense()) {
      case FAHLoopInXYPlane::kCounterClockwise: {
        // add as the outer boundary of a region
        SliceMaterialRegion region;
        region.setOuterBoundary(loops.at(i));
        loops.remove(i);
        regions.append(region);
      } break;
      case FAHLoopInXYPlane::kClockwise:
        // don't do anything with this yet
        break;
      default:
      case FAHLoopInXYPlane::kUndefined:
        // discard this malformed loop
        loops.remove(i);
        if (callback) {
          QString issue_text;
          QTextStream(&issue_text) << "malformed loop removed";
          callback->encounteredIssue(issue_text);
        }
        break;
      }
    }

    // for each of the inner boundaries, find the tightest
    // outer boundary that contains that inner boundary
    // and make this inner boundary a child of that region
    for (int i = loops.size() - 1; i >= 0; --i) {
      const FAHLoopInXYPlane& loop = loops.at(i);
      int tightest_container_index = -1;
      for (int j = 0; j < regions.size(); ++j) {
        if (regions.at(j).getOuterBoundary().contains(loop) &&
            (tightest_container_index < 0 ||
             regions.at(tightest_container_index).getOuterBoundary().contains(regions.at(j).getOuterBoundary()))) {
          tightest_container_index = j;
        }
      }
      // we should always have a container for an inner
      // boundary, otherwise something is wrong with the
      // model.
      if (tightest_container_index >= 0) {
        regions[tightest_container_index].addInnerBoundary(loop);
      } else {
        if (callback) {
          QString issue_text;
          QTextStream(&issue_text) << "Inner-boundary loop was uncontained by an outer boundary";
          callback->encounteredIssue(issue_text);
        }
      }
    }

    // create the regions in the output material
    SliceMaterial material(input_region->getFillMaterialId());
    for (int i = 0; i < regions.size(); ++i) {
      material.addSliceMaterialRegion(regions.at(i));
    }

    // build the output slice
    Slice slice(current_z + z_adjustment);
    slice.addSliceMaterial(material);
    slicestack.addSlice(slice);

    // report that computation of this slice is complete
    if (callback) callback->sliceHasBeenComputed(slice_index, current_z);
  }

  // all of the slices have been finished
  if (callback) callback->finishedSlicing(slicestack);

  return true;
}


Slicer::Error Slicer::getLastError() const {
  todo("kwg8","implement error codes");
  return kNone;
}

QString Slicer::getLastErrorString() const {
  return getErrorString(getLastError());
}

QString Slicer::getErrorString(Error error) {
  switch (error) {
    case kNone: return "No error";
    case kGenericError: return "Generic Error";
    default: return "Unknown Error";
  }
}

const SliceStack& Slicer::getSliceStack(){
  return slicestack;
}

void Slicer::findLoops(const QList<FAHLine>& segments,
                       QVector<FAHLoopInXYPlane>* loops_out) {

  // Start with some minimum tolerance
  FAHFloat tolerance = 1e-8;
  const FAHFloat kHighestTolerance = 1e-2;
  QVector<FAHLoopInXYPlane> open_loops;
  QVector<FAHLoopInXYPlane>& closed_loops = *loops_out;

  // Add all segments as "open" loops
  foreach (const FAHLine& segment, segments) {
    FAHLoopInXYPlane loop;
    loop.add(segment.a);
    loop.add(segment.b);
    open_loops.append(loop);
  }

  // Try to merge open loops
  while (tolerance <= kHighestTolerance && !open_loops.isEmpty()) {
    // For each loop, try to merge it with other open loops
    for (int k = open_loops.size() - 1; k >= 0; --k) {
      FAHLoopInXYPlane& k_loop = open_loops[k];
      const FAHVector3& k_first = k_loop.points.first();
      const FAHVector3& k_last = k_loop.points.last();
      for (int j = k - 1; j >= 0; --j) {
        FAHLoopInXYPlane& j_loop = open_loops[j];
        const FAHVector3& j_first = j_loop.points.first();
        const FAHVector3& j_last = j_loop.points.last();

        // Are any two of the loops' endpoints coincident?
        if (k_first.distanceToSq(j_first) < tolerance) {
          // prepend the reversed list of points in j, except the first point
          for (int i = 1; i < j_loop.points.size(); ++i) {
            k_loop.addFirst(j_loop.points.at(i));
          }
        } else if (k_first.distanceToSq(j_last) < tolerance) {
          // prepend the list of points in j, except
          // for the last point
          for (int i = j_loop.points.size() - 2; i >= 0; --i) {
            k_loop.addFirst(j_loop.points.at(i));
          }
        } else if (k_last.distanceToSq(j_first) < tolerance) {
          // append the list of points in j, except the first point
          for (int i = 1; i < j_loop.points.size(); ++i) {
            k_loop.points.append(j_loop.points.at(i));
          }
        } else if (k_last.distanceToSq(j_last) < tolerance) {
          // append the reversed list of points in j, except the last point
          for (int i = j_loop.points.size() - 2; i >= 0; --i) {
            k_loop.points.append(j_loop.points.at(i));
          }
        } else {
          // these two loops do not connect
          continue;
        }

        // Has this loop been completed?
        if (k_loop.points.first().distanceToSq(k_loop.points.last()) < tolerance) {

          // Remove the duplicate point in this loop
          k_loop.points.remove(0);

          // The loop is now closed
          closed_loops.append(k_loop);
          open_loops.remove(k);
        }

        // We merged the j'th loop into the k'th one
        open_loops.remove(j);

        // Restart examining the list of open loops
        k = open_loops.size();
        j = 0;
        break;
      }
    }

    // If there are any outstanding open loops, the tolerance
    // might be too low to close the list; so, increase the
    // tolerance
    tolerance *= 2;
  }

  // Any remaining open loops are invalid.  However, to make the
  // best of the situation, we'll discard only those that are
  // truly invalid (those with only 2 points) and try to keep
  // the rest.
  if (!open_loops.isEmpty()) {
    QString issue_text;
    QTextStream(&issue_text) << open_loops.size()
                             << " unclosed loop"
                             << (open_loops.size() != 1 ? "s" : "")
                             << "; treating { ";
    bool is_first = true;
    for (int k = open_loops.size() - 1; k >= 0; --k) {
      if (open_loops.at(k).points.size() <= 2) {
        // remove this degenerate loop
        open_loops.remove(k);
      } else {
        if (is_first) {
          QTextStream(&issue_text) << open_loops.at(k).points.size();
        } else {
          QTextStream(&issue_text) << "," << open_loops.at(k).points.size();
        }
        is_first = false;
      }
    }
    QTextStream(&issue_text) << " } points as closed";
    if (callback) callback->encounteredIssue(issue_text);
    closed_loops += open_loops;
  }

  // Simplify all of the loops
  for (int i = closed_loops.size() - 1; i >= 0; --i) {
    closed_loops[i].simplify();
    if (closed_loops[i].sense() == FAHLoopInXYPlane::kUndefined) {
      closed_loops.remove(i);
    }
  }
}
