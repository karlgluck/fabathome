/*---------------------------------------------------------------------------*\
 This file is part of the Fab@Home Project.
 Fab@Home operates under the BSD Open Source License.

 Copyright (c) 2010, Karl Gluck (kwg8@cornell.edu)
               2010, Jeremy Cohen (jrc352@cornell.edu)

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

#include "arrangebuildtray.h"
#include "shared/fabathome-constants.h"
#include <QList>

/**
 * A rectangle class that stores the width, height, x, y, and orientation values.
 */
class Rectangle{

private:

  ///
  double width, height;

  ///
  double x, y;

  /// Location; index of the original footprint for which this rectangle was generated
  int loc;

public:

  Rectangle();

  /**
   *
   */
  Rectangle(double newWidth, double newHeight);

  /**
   *
   */
  Rectangle(double newWidth, double newHeight, int newLoc);

  /**
   *
   */
  ~Rectangle();

  ///
  int isRotated;

  /**
    Swaps the values of the width and height, thus also changing the rotation of the rectangle.
   **/
  void swapWandH();

  /**
   *
   */
  void setWidth(double newWidth);

  /**
   *
   */
  void setHeight(double newHeight);

  /**
   *
   */
  double getWidth();

  /**
   *
   */
  double getHeight();

  /**
   *
   */
  void setLoc(int newLoc);

  /**
   *
   */
  int getLoc();

  /**
   *
   */
  void setX(double newX);

  /**
   *
   */
  void setY(double newY);

  /**
   *
   */
  double getX();

  /**
   *
   */
  double getY();

  /**
    @return  The maximum x value of the rectangle.
    **/
  double getMaxX();

  /**
    @return  The maximum y value of the rectangle.
    **/
  double getMaxY();
};



/**
  returns the original location of the rectangle currently at location loc
  @param rectangles  The list of rectangles.
  @param loc  The current location of the examined rectangle.
  @return  The original location of the examined rectangle.
  **/
int getIndexFromLoc(QList<Rectangle> rectangles, int loc);



/**
  Sorts the rectangles by largest to smallest height with height being the larger dimension
  @param rectangles  The list of rectangles.
  **/
void sortRectangles(QList<Rectangle>& rectangles);



/**
  Arranges the rectangles so that they are packed together based on how they are sorted by height.
  @param rectangles  The list of rectangles.
  **/
void arrangeRectangles(QList<Rectangle>& rectangles, double trayWidth);



/**
  Outputs the positions and orientations into a QList.
  @param rectangles  The list of rectangles.
  @return  The positions and orientations of the rectangles in a structure in a QList.
  **/
QList<PositionAndOrientation> outputRectangles(QList<Rectangle> rectangles);


/**
  Checks to see if any of the two rectangles overlap.
  @param rectangles  The list of rectangles.
  @return  A boolean that states whether any of the two rectangles overlap.
  **/
bool hasOverlap(QList<Rectangle> rectangles);



QList<PositionAndOrientation> arrangeBuildTray(const QList<FAHLoopInXYPlane>& footprints, double trayWidth)
{
    //TODO: Need to figure out the width of the printing tray so that the items can be packed together.
    QList<Rectangle> rectangles;
    for (int i = 0; i < footprints.size(); i++)
    {
        double height = footprints.at(i).max().x - footprints.at(i).min().x;
        double width = footprints.at(i).max().y - footprints.at(i).min().y;
        Rectangle r;
        r.setHeight(height);
        r.setWidth(width);
        r.setLoc(i);
        rectangles.push_back(r);
    }
    sortRectangles(rectangles);
    arrangeRectangles(rectangles, trayWidth);
    return outputRectangles(rectangles);
}

Rectangle::Rectangle()
{width = 0; height = 0;}
Rectangle::Rectangle(double newWidth, double newHeight)
{
    width = newWidth;
    height = newHeight;
    isRotated = 0;
}
Rectangle::Rectangle(double newWidth, double newHeight, int newLoc)
{
    width = newWidth;
    height = newHeight;
    isRotated = 0;
    loc = newLoc;
}

Rectangle::~Rectangle()
{}
void Rectangle::swapWandH()
{
    double temp = width;
    width = height;
    height = temp;
    (++isRotated) % 2;
}
void Rectangle::setWidth(double newWidth){width = newWidth;}
void Rectangle::setHeight(double newHeight){height = newHeight;}
double Rectangle::getWidth(){return width;}
double Rectangle::getHeight(){return height;}
//The variale loc is to represent the number in which it was retrieved from the input. This way we can make
//sure that every line of the input corresponds to that of the output. The only time these methods should be
//called is when either reading or writing to the input or output, respectively.
void Rectangle::setLoc(int newLoc){loc = newLoc;}
int Rectangle::getLoc(){return loc;}
void Rectangle::setX(double newX){x = newX;}
void Rectangle::setY(double newY){y = newY;}
double Rectangle::getX() {return x;}
double Rectangle::getY() {return y;}
double Rectangle::getMaxX() {return x + height;}
double Rectangle::getMaxY() {return y + width;}


//gets the index of the desired rectangle
int getIndexFromLoc(QList<Rectangle> rectangles, int loc)
{
    for (int i = 0; i < rectangles.size(); i++)
    {
        if (rectangles[i].getLoc() == loc)
           return i;
    }
    return -1;
}

void sortRectangles(QList<Rectangle>& rectangles)
{
    //This method uses a selection sort to 1) rotate the rectangles such that height >= width, and 2) sort the rectangles by the largest height

    //makes sure that height >= width for all rectangles
    for (int i = 0; i < rectangles.size(); i++)
    {
        if (rectangles[i].getWidth() <= rectangles[i].getHeight())
        {
            rectangles[i].swapWandH();
        }

    }
    int maxHLoc;
    double maxH;
    for (int i = 0; i < rectangles.size(); i++)
    {
        maxH = rectangles[i].getHeight();
        maxHLoc = i;
        for (int j = i+1; j < rectangles.size(); j++)
        {
            if (maxH < rectangles[j].getHeight())
            {
                maxH = rectangles[j].getHeight();
                maxHLoc = j;
            }
        }
        Rectangle *temp = new Rectangle();
        *temp = rectangles[i];
        rectangles[i] = rectangles[maxHLoc];
        rectangles[maxHLoc] = *temp;
        delete temp;
    }
}

void arrangeRectangles(QList<Rectangle>& rectangles, double trayWidth)
{
    double currentHeight = 0;
    int i = 0;
    rectangles[0].setX(rectangles[0].getHeight() / 2);
    rectangles[0].setY(rectangles[0].getWidth() / 2);
    currentHeight = rectangles[i].getHeight() / 2;
    double maxHeight = rectangles[0].getHeight() / 2;
    for (i = 1; i < rectangles.size(); i++)
    {
        if (rectangles[i-1].getY() + rectangles[i-1].getWidth() / 2 + rectangles[i].getWidth() <= trayWidth)
        {
            rectangles[i].setX(currentHeight);
            rectangles[i].setY(rectangles[i-1].getY() + rectangles[i-1].getWidth() / 2 + rectangles[i].getWidth() / 2);
        }
        else
        {
            currentHeight += maxHeight + rectangles[i].getHeight() / 2;
            maxHeight = rectangles[i].getHeight() / 2;
            rectangles[i].setX(currentHeight);
            rectangles[i].setY(rectangles[i].getWidth() / 2);
        }
    }
}

QList<PositionAndOrientation> outputRectangles(QList<Rectangle> rectangles)
{
    int index;
    QList<PositionAndOrientation> output;
    for (int i = 0; i < rectangles.size(); i++)
    {
        index = getIndexFromLoc(rectangles, i);
        PositionAndOrientation temp;
        if (index >= 0)
        {
            temp.isRotated = rectangles[index].isRotated;
            temp.x = rectangles[index].getX();
            temp.y = rectangles[index].getY();
        }
        else
        {
            temp.isRotated = -1;
            temp.x = -1;
            temp.y = -1;
        }
        output.push_back(temp);
    }
    return output;
}

bool hasOverlap(QList<Rectangle> rectangles)
{
    for (int i = 0; i < rectangles.size() - 1; i++)
    {
        for (int j = i+1; j < rectangles.size(); j++)
        {
            if (rectangles[i].getX() < rectangles[j].getMaxX() && rectangles[i].getMaxX() > rectangles[j].getX())
            {
                if (rectangles[i].getY() < rectangles[j].getMaxY() && rectangles[i].getMaxY() > rectangles[j].getY())
                {
                    return true;
                }
            }
        }
    }
    return false;
}



/**

void increaseCapacities(Rectangle *&R,double widthIncreaser, double heightIncreaser)
{
    for (int i = 0; i < numOfRectangles; i++)
    {
        R[i].setWidth(R[i].getWidth() + widthIncreaser);
        R[i].setHeight(R[i].getHeight() + heightIncreaser);
    }
}
**/
