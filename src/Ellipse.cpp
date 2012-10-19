#include "Ellipse.h"

using namespace std;

Ellipse::Ellipse(const double x, const double y, const double site_a, const double site_b) {
  this->x=x;
  this->y=y;
  this->site_a=site_a;
  this->site_b=site_b;
};

// Caution: this function needs coordinates x and y of an ellipse thats center is in the origin
short Ellipse::liesInside(const double x, const double y) const {
  double value = x*x/(site_a*site_a) + y*y/(site_b*site_b);
  if(value == 1) return 0;
  else if(value > 1) return -1;
  else return 1;
};

double Ellipse::getAntiDerivativeValue(const double var, const double y1) const {
  double value;
  value=0.5*(site_b*var*sqrt(1-var*var/(site_a*site_a)) + site_a*site_b*asin(var/site_a))- y1*var;
  return value;
};

double Ellipse::getArea(const double start_x, const double width_x, const double start_y, const double width_y) const {
  double area;
  double x1 = start_x - x;
  double y1 = start_y - y;
  
  if(start_y < 0) {
    y1 = -start_y - width_y;
  }
  
  double x2 = x1 + width_x;
  double y2 = y1 + width_y;

  if(liesInside(x1, y1) <= 0 && liesInside(x2, y1) <= 0 && liesInside(x1, y2) <= 0 && liesInside(x2, y2) <= 0) return 0; //whole area is outside the circle
  if(liesInside(x1, y1) >= 0 && liesInside(x2, y1) >= 0 && liesInside(x1, y2) >= 0 && liesInside(x2, y2) >= 0) { //whole area is inside the circle
    area = width_x*width_y;
    return area;
  }
  
  // we have a rectangle which partly lies inside and partly lies outside the circle
  if(y1*y2 < 0) {
    area = getArea(start_x, width_x, 0, -y1) + getArea(start_x, width_x, 0, y2);
    return area;
  }
  
  double nullstelle = site_a*sqrt(1-y1*y1/(site_b*site_b));
  double bound1 = min(nullstelle, abs(x1));
  if(x1 < 0) bound1 = -bound1;
  double bound2 = min(nullstelle, abs(x2));
  if(x2 < 0) bound2 = -bound2;
  area = getAntiDerivativeValue(bound2, y1) - getAntiDerivativeValue(bound1, y1);
  area -= getArea(x+x1, width_x, y+y1+width_y, 2*site_a*site_b);
  return area;
};