/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c)  2010, Chris Hogan (coh5@cornell.edu)
                2010, Karl Gluck (kwg8@cornell.edu)


 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the Fab@Home nor the
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
#include "centerofgravity.h"
#include <cstdlib>
#include "Math.h"

 /*
  *Allow multiple data types to be returned from the Monte Carlo method
  */
struct MonteCarloReturnValues {

    //the centerpoint of the mesh, calculated by the vertices
    FAHVector3 centerPoint;
    //an accumulator used to track the growing center of mass in Monte Carlo
    FAHVector3 acc;
    //counts how many times Monte Carlo accumulates a value
    int counter;

    //initialize values in constructor
    MonteCarloReturnValues () {

        acc.zero();
        centerPoint.zero();
        counter=0;
    } //end constructor
}; //end struct

/*
 * Calculate the center of gravity of the mesh of triangles
 * @param triangles (the mesh of triangles that represent the shape whose center of gravity calculateCoG finds)
 * @param tolerance (the tolerance of change between two consecutive center of gravity calculations)
 * @return cog (the estimated center of gravity for the triangle mesh)
 */
FAHVector3 calculateCoG(const QList<FAHTriangle>& triangles,
                        double tolerance);

/*
 * Randomly pick points inside the mesh and use these points to estimate the center of gravity
 * @param triangles (the mesh object)
 * @param values (the structure that contains the centerpoint, counter, accumulator)
 */
void monteCarlo(const QList <FAHTriangle> &triangles,
                MonteCarloReturnValues *values);

/*
 * Use a series of dot and cross products to determine if the ray intersects the triangle
 * @param origin (represent the origin of the ray)
 * @param ray (the vector from the point up to infinity)
 * @param point0 (one of 3 vertices that defines the triangle)
 * @param point1 (one of 3 vertices that defines the triangle)
 * @param point2 (one of 3 vertices that defines the triangle)
 * @return boolean (true if ray and triangle intersect, false if no intersection)
 */
bool rayTriangleIntersection (FAHVector3 origin,
                              FAHVector3 ray,
                              FAHVector3 point0,
                              FAHVector3 point1,
                              FAHVector3 point2);


 /*
  * Calculate the center of gravity of the objecte represented by the triangle mesh
  * @param meshes (represents an object)
  * @return toRet (the center of gravity of meshes)
  */
FAHVector3 calculateCenterOfGravity(const QList<FAHTriangle>& meshes) {

  //1.0 is the tolerance
  FAHVector3 toRet=calculateCoG(meshes,1.0);
  return toRet;
} //end calculateCenterOfGravity

 /*
  * Calculate the center of gravity of the mesh of triangles
  * @param triangles (the mesh of triangles that represent the shape whose center of gravity calculateCoG finds)
  * @param tolerance (the tolerance of change between two consecutive center of gravity calculations)
  * @return cog (the estimated center of gravity for the triangle mesh)
  */
FAHVector3 calculateCoG(const QList<FAHTriangle>& triangles,
                        double tolerance) {

    //accumulate as more center of gravity calculations are made
    FAHVector3 accumulator;
    //remember the previous center of gravity to check the tolerance
    FAHVector3 last;
    //used to check if last center of gravity is within tolerance of current center of gravity
    FAHVector3 temp;
    //the estimated center of gravity of the mesh
    FAHVector3 cog;

    //initialize variables
    last.zero();
    accumulator.zero();
    int counter=1;
    bool loop=true;
    MonteCarloReturnValues values;

    //continually calculate center of gravity until estimates of center of gravity remain within tolerance
    while (loop) {

        //store last CoG
        last.set(accumulator).scale(1.0/counter);

        //calculate current CoG
        monteCarlo(triangles, &values);

        accumulator.add(values.acc);
        counter+=values.counter;

        //see if last CoG is within tolerance of current CoG
        temp.set(accumulator).scale(1.0/counter);
        loop=(tolerance<temp.distanceTo(last));
    } //end while

    //average cog based off accumulator and the number of trials
    cog.set(accumulator).scale(1.0/counter);

    return cog;
} //end calculateCoG


/*
 * Randomly pick points inside the mesh and use these points to estimate the center of gravity
 * @param triangles (the mesh object)
 * @param values (the structure that contains the centerpoint, counter, accumulator)
 */
void monteCarlo(const QList <FAHTriangle> &triangles,
                MonteCarloReturnValues *values) {

    //the minimum possible x,y,z bounds
    FAHVector3 minBoundary;
    //the maximum possible x,y,z bounds
    FAHVector3 maxBoundary;
    //store the random points that are inside the mesh
    FAHVector3 acc;
    //used for efficiency, higher variance yields more checks with more vertices on mesh, data will be more accurate, but longer processing time
    double variance=10;

    //initialize vars
    acc.zero();
    maxBoundary.set(triangles.at(0).v[0]);
    minBoundary.set(triangles.at(0).v[0]);

    //check all points in triangles
    //store min/max x,y,z
    for(int i=0;i<triangles.length();i++) {

        maxBoundary.max(triangles.at(i).v[0]);
        maxBoundary.max(triangles.at(i).v[1]);
        maxBoundary.max(triangles.at(i).v[2]);

        minBoundary.min(triangles.at(i).v[0]);
        minBoundary.min(triangles.at(i).v[1]);
        minBoundary.min(triangles.at(i).v[2]);

    } //end for

    FAHVector3 centerPoint;
    centerPoint.set(maxBoundary.copy().add(minBoundary)).scale(1/2);

    //declare infinity
    FAHVector3 infinity;
    int counter=0;

    for(int i=0;i<300;i++) {

        //randomly find a point
        FAHVector3 randPoint;
        randPoint.set(((maxBoundary.copy().sub(minBoundary)).scale(qrand()/double(RAND_MAX))).add(minBoundary));

        //define infinity to be directly above the random point
        infinity.set(randPoint);
        infinity.z = fabs(infinity.z) * 2;

        //create a ray of the point
        FAHVector3 ray;
        ray.set(infinity).sub(randPoint);
        int intersectionCounter=0;

        //check all nearby triangles to see how many the ray intersects
        for(int j=0;j<triangles.length();j++) {

            //the three points that make up the vertices of a triangle
            FAHVector3 p1;
            FAHVector3 p2;
            FAHVector3 p3;

            //calculate plausible triangles that the ray of your point to infinity could intersect
            p1.set(triangles.at(j).v[0]);
            p2.set(triangles.at(j).v[1]);
            p3.set(triangles.at(j).v[2]);

            //increment intersection counter if at least one vertex of the triangle has a z coordinate greater than the random point's z coordinate,
            //and if the ray and triangle intersect. For ease of reading, it is broken into 2 if statements.
            if ((fabs(p1.x-randPoint.x)<variance&&fabs(p1.y-randPoint.y)<variance&&p1.z>randPoint.z)
                             ||fabs((p2.x-randPoint.x)<variance&&fabs(p2.y-randPoint.y)<variance&&p3.z>randPoint.z)||
                             fabs((p3.x-randPoint.x)<variance&&fabs(p3.y-randPoint.y)<variance&&p3.z>randPoint.z))
                     if (rayTriangleIntersection(randPoint,ray,p1,p2,p3)==1)

                             intersectionCounter++;
        } //end for j

        //if the ray intersects an odd number of triangles, it is an interior point, therefore, increase vars
        if(intersectionCounter%2==1) {
            acc.set(randPoint);
            counter++;
        } //end if

    } //end for i

    values->centerPoint=centerPoint;
    values->acc=acc;
    values->counter=counter;
} //end monteCarlo

/*
 * Use a series of dot and cross products to determine if the ray intersects the triangle
 * @param origin (represent the origin of the ray)
 * @param ray (the vector from the point up to infinity)
 * @param point0 (one of 3 vertices that defines the triangle)
 * @param point1 (one of 3 vertices that defines the triangle)
 * @param point2 (one of 3 vertices that defines the triangle)
 * @return boolean (true if ray and triangle intersect, false if no intersection)
 */
bool rayTriangleIntersection (FAHVector3 origin,
                              FAHVector3 ray,
                              FAHVector3 point0,
                              FAHVector3 point1,
                              FAHVector3 point2) {

    double epsilon=0.00001;
    FAHVector3 e1,e2;
    e1.set(point1).sub(point0);
    e2.set(point2).sub(point0);

    FAHVector3 q;
    q=ray.cross(e2);
    double a=e1.dot(q);

    //vector is parallel to plane
    if(a>-epsilon && a<epsilon)
        return 0;

    double f=1/a;
    FAHVector3 s;
    s.set(origin).sub(point0);
    double u=f*s.dot(q);

    //the intersection is outside the triangle
    if(u<0.0)
        return 0;

    FAHVector3 r;
    r=s.cross(e1);
    double v=f*ray.dot(r);

    //the intersection is outside the triangle
   if(v<0.0 || u+v>1.0)
       return 0;

   return 1;
} //end rayTriangleIntersection

