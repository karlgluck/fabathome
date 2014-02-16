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
#include "simplecrosshatchpather.h"
#include <common/confirm.h>
#include <common/todo.h>

/*

 THIS FILE HAS AN UNRESOLVED ISSUE (Dec 8 2009)

 This pather has not been extensively tested on corner-case shapes.  Here are
 some situations where it may fail to produce the expected result:

  - path width is very large relative to the area of a region to be pathed
  - path width is very small (this is a performance issue, some of the
                              algorithms are N^2)
  - region boundaries where the inner boundary is very close to the outer
    boundary on only one edge; e.g.
        _______
       /     _ |
      /     | ||
     |      |_||
     |         |
      \________|


 */




SimpleCrossHatchPather::SimpleCrossHatchPather()
  : initialized_(false),
    path_width_(0.0) {
}



//----[  initialize  ]---------------------------------------------------------
void SimpleCrossHatchPather::initialize() {
  if (initialized_) {
    on_x_axis = !on_x_axis;
  } else {
    initialized_ = true;
    on_x_axis = false;
    path_width_ = getSettings().get("PathWidth", 1.5f);
  }
}


//----[  runPathingAlgorithm  ]------------------------------------------------
bool SimpleCrossHatchPather::runPathingAlgorithm(
        const SliceMaterialRegion& region,
        PathSliceRegion* returned_paths,
        PatherProgressCallback* callback) {
  initialize();

  // the slice's Z-coordinate
  const float z = region.getOuterBoundary().getAverageZCoordinate();

  { // Add all inner loops to returned paths
    const QVector<FAHLoopInXYPlane>& inner_boundaries
        = region.getInnerBoundaries();
    for (int i = 0; i < inner_boundaries.size(); ++i) {
      const FAHLoopInXYPlane& loop = inner_boundaries.at(i);
      QVector<FAHLoopInXYPlane> displaced_boundaries;
      loop.expand(-path_width_ / 2, &displaced_boundaries);
      for (int boundary = 0; boundary < displaced_boundaries.size(); ++boundary) {
        Path p(displaced_boundaries.at(boundary));
        returned_paths->addPath(p);
      }
    }
  }

  // Displace the outer boundary to generate the outer path.  It must be
  // displaced because the path defines the tool's location, and the tool
  // material extends half a path-width beyond the boundary on both sides.
  {
    const FAHLoopInXYPlane& loop = region.getOuterBoundary();
    QVector<FAHLoopInXYPlane> displaced_boundaries;
    loop.expand(-path_width_ / 2, &displaced_boundaries);
    for (int boundary = 0; boundary < displaced_boundaries.size(); ++boundary) {
      Path p(displaced_boundaries.at(boundary));
      returned_paths->addPath(p);
    }
  }

  { // Build the fill paths
    QVector<FAHLine> segments;

    // Obtain a bounding box around all of the paths
    FAHVector3 min = region.getOuterBoundary().min();
    FAHVector3 max = region.getOuterBoundary().max();

    // First, create all line segments within the outer boundary
    if (on_x_axis) {
      min.x -= 1.0;
      max.x += 1.0;
      for (float y = min.y + path_width_; y <= max.y; y += path_width_) {
        FAHLine line(FAHVector3::make(min.x, y, z),
                     FAHVector3::make(max.x, y, z));
        segments.append(line);
      }
    } else {
      min.y -= 1.0;
      max.y += 1.0;
      for (float x = min.x + path_width_; x <= max.x; x += path_width_) {
        FAHLine line(FAHVector3::make(x, min.y, z),
                     FAHVector3::make(x, max.y, z));
        segments.append(line);
      }
    }

    QVector<FAHLoopInXYPlane> outer_boundary_loops;
    QVector<FAHLoopInXYPlane> expanded_border_loops;

    { // Get the contracted the outer boundary
      // We contract by 3/2*(Path Width) because the outer boundary was already
      // contracted by half a path-width above to account for the fact that the
      // tip tracing the outer boundary has finite thickness on both sides
      // of the path.
      FAHLoopInXYPlane loop = region.getOuterBoundary();
      confirm(loop.sense() == FAHLoopInXYPlane::kCounterClockwise);
      loop.expand(-3 * path_width_ / 2, &outer_boundary_loops);
      if (outer_boundary_loops.isEmpty()) {
        // this boundary is too small to support any pathing
        float z = region.getOuterBoundary().getAverageZCoordinate();
        if (callback != NULL) {
          QString issue;
          issue.sprintf("Outer boundary is too small to fill region at z=%f", z);
          callback->encounteredIssue(issue);
        }
        return true; // don't finish the fill-pathing
      } else {
        expanded_border_loops += outer_boundary_loops;
      }
    }

    { // get the expanded inner boundaries
      const QVector<FAHLoopInXYPlane> inner_boundaries = region.getInnerBoundaries();
      for (int i = 0; i < inner_boundaries.size(); ++i) {
        FAHLoopInXYPlane loop = inner_boundaries.at(i);
        confirm(loop.sense() == FAHLoopInXYPlane::kClockwise);
        QVector<FAHLoopInXYPlane> loops;
        loop.expand(-3 * path_width_ / 2, &loops);
        bool contained = false;
        for (int j = 0; j < loops.size(); ++j) {
          const FAHLoopInXYPlane& test_loop = loops.at(j);
          for (int k = 0; !contained && k < outer_boundary_loops.size(); ++k) {
            contained = outer_boundary_loops.at(k).contains(test_loop);
          }
          if (!contained) {
            float z = region.getOuterBoundary().getAverageZCoordinate();
            QString issue;
            issue.sprintf("Expanded inner boundary %i at z=%f overlaps outer boundary; this is safe to ignore", j, z);
            if (callback != NULL) callback->encounteredIssue(issue);
          }
          // loop is correctly contained by at least one outer boundary
          expanded_border_loops += test_loop;
        }
      }
    }

    {
      confirm(!expanded_border_loops.isEmpty()) else {
        // break out because we need to split the segments, otherwise
        // the layer will get filled completely with really long
        // meaningless segments
        return true;
      }
      for (int i = 0; i < expanded_border_loops.size(); ++i) {
        expanded_border_loops[i].simplify(); // improve speed
        const FAHLoopInXYPlane& loop = expanded_border_loops.at(i);
        QVector<FAHLine> old_segments;
        old_segments += segments;
        segments.clear();
        for (int j = 0; j < old_segments.size(); ++j) {
          const FAHLine& current_segment = old_segments[j];
          QVector<FAHLine> inside;
          loop.findInnerSegments(current_segment, &inside);
          segments += inside;
        }
      }
    }

    // Stitch together the segments.  This is done with a very brute-force
    // algorithm.
    // 1.  Make every segment into a path
    // 2.  For some path's end vertex, find a segment to another path end
    //     vertex such that the segment doesn't cross another path or boundary
    // 3.  List all segments found this way for that vertex
    // 4.  Take the shortest segment found
    // 5.  For that segment's other vertex, repeat the process
    // 6.  If both paths' shortest segments are each other, connect the two
    // 7.  Repeat until no more paths can be connected
    QVector<Path> paths;
    for (int i = 0; i < segments.size(); ++i) {
      Path path;
      path.addPathPointEnd(segments.at(i).a);
      path.addPathPointEnd(segments.at(i).b);
      paths.append(path);
    }
    segments.clear();

    enum Check {
      kStartToStart = 0,
      kStartToEnd = 1,
      kEndToStart = 2,
      kEndToEnd = 3,
    };
RESTART_STITCHING:
    for (int from_index = 0; from_index < paths.size(); ++from_index) {
      Path& path = paths[from_index];
      int shortest_segment_index = -1;
      FAHFloat shortest_segment_length;
      Check shortest_segment_check;
      for (int check_value = 0; check_value < 4; ++check_value) {
        Check check = (Check)(check_value);
        const FAHVector3& from = (check == kStartToStart || check == kStartToEnd)
                               ? path.start()
                               : path.end();
        for (int to_index = 0; to_index < paths.size(); ++to_index) {

          // don't let the path connect to itself
          if (to_index == from_index) continue;

          const Path& other_path = paths.at(to_index);
          const FAHVector3& to = (check == kStartToStart || check == kEndToStart)
                                 ? other_path.start()
                                 : other_path.end();
          FAHLine segment(from, to);
          FAHFloat segment_length = segment.length();
          if (shortest_segment_index >= 0 &&
              shortest_segment_length < segment_length) {
            // skip out early; we already have a better segment
            continue;
          }
          bool create_path = true;
          create_path = !region.getOuterBoundary().crossedBy(segment);
          const QVector<FAHLoopInXYPlane>& inner_boundaries = region.getInnerBoundaries();
          for (int j = 0; create_path && j < inner_boundaries.size(); ++j) {
            create_path = !inner_boundaries.at(j).crossedBy(segment);
          }
          for (int j = 0; create_path && j < paths.size(); ++j) {
            create_path = !paths.at(j).crossedBy(segment);
          }
          if (create_path) {
            shortest_segment_length = segment_length;
            shortest_segment_index = to_index;
            shortest_segment_check = check;
          }
        }
      }
      if (shortest_segment_index >= 0) {
        // we found a segment with which to connect this segment
        const Path& other_path = paths.at(shortest_segment_index);
        switch (shortest_segment_check) {
        case kStartToStart: path.prependReversed(other_path); break;
        case kStartToEnd:   path.prepend(other_path); break;
        case kEndToStart:   path.append(other_path);  break;
        case kEndToEnd:     path.appendReversed(other_path); break;
        }
        paths.remove(shortest_segment_index);
        goto RESTART_STITCHING;
      }
    }

    todo("kwg8", "Sanitize the paths to remove paths that double-back on themselves");

    for (int i = 0; i < paths.size(); ++i) {
      returned_paths->addPath(paths.at(i));
    }
  }

  // Success
  return true;
}

