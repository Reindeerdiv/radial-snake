#include <emscripten/val.h>
#include "point.h"

using namespace Geometry;

namespace Geometry::Line {
  struct Shape {
    double x1;
    double y1;
    double x2;
    double y2;
  };

  double getX(Shape context, double y);
  double getY(Shape context, double x);
  int hasPoint(Shape context, double x, double y);
  int boundsHavePoint(Shape context, double x, double y);
  Point::Shape getLineIntersection(Shape context, Shape line);
}