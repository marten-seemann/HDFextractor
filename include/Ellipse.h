#ifndef HDF_Ellipse_h
#define HDF_Ellipse_h

#include <iostream>
#include <cmath>
#include "functions.h"


/**
 * @brief A class for calculating with ellipse-shaped figures
 *
 * This class writes to *one* output file, or to standard output
 * 
 */

class Ellipse {
private:
  /// coordinate @a x in space
  double x;
  /// coordinate @a y in space
  double y;
  /// one semiaxis of the ellipse
  double site_a;
  /// the other semiaxis of the ellipse
  double site_b;
  
private:
  /// get the area of the ellipse in a specified interval
  double getAntiDerivativeValue(const double var, const double y1) const;
  /// determines if the point (@a x, @a y) lies inside the ellipse
  /** @attention note that the center of the ellipse is *always* the origin (0, 0)
  * @param x the @a x coordinate
  * @param y the @a y coordinate
  * @return 1: if point lies inside, -1 if point lies outside, 0 if point lies exactly on the border of the ellipse
  */
  short liesInside(const double x, const double y) const;
  
public:
  /// constructor
  /**
   *  @param x the @a x coordinate of the ellipse
   *  @param y the @a y coordinate of the ellipse
   *  @param site_a one semiaxis of the ellipse
   *  @param site_b the other semiaxis of the ellipse
   */
  Ellipse(const double x, const double y, const double site_a, const double site_b);
  /// get the area between specified coordinates which is covered by the ellipse
  /** the given coordinates specify a rectangle
   *  @param start_x the @a x coordinate of the rectangle
   *  @param width_x the width of the rectangle in @a x direction
   *  @param start_y the @a y coordinate of the rectangle
   *  @param width_y the width of the rectangle in @a y direction
   *  @return the area of the specified rectangle covered by the ellipse 
   */
  double getArea(const double start_x, const double width_x, const double start_y, const double width_y) const;
};

#endif