//
//  Circle.h
//  HDF
//
//  Created by Marten Seemann on 13.09.11.
//  Copyright (c) 2011 ---. All rights reserved.
//

#ifndef HDF_Circle_h
#define HDF_Circle_h

#include <iostream>
#include <cmath>
#include "functions.h"


class Ellipse {
private:
  double x,y; //coordinates in space
  double site_a, site_b;
  
private:
  double getAntiDerivativeValue(const double var, const double y1) const;
  short liesInside(const double x, const double y) const;
  
public:
  Ellipse(const double x, const double y, const double site_a, const double site_b);
  double getArea(const double start_x, const double width_x, const double start_y, const double width_y) const;
};


#endif
