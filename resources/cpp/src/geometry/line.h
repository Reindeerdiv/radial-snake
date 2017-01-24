#ifndef GEO_LINE_H
#define GEO_LINE_H

#include <emscripten/val.h>
#include "point.h"

using namespace Geometry;

namespace Geometry {
  namespace Line {
    struct Shape {
      double x1;
      double y1;
      double x2;
      double y2;
    };

    emscripten::val getX(Shape context, double y);
    emscripten::val getY(Shape context, double x);
    int hasPoint(Shape context, double x, double y);
    int boundsHavePoint(Shape context, double x, double y);
    Point::Shape getLineIntersection(Shape context, Shape line);
  }
}

#endif