#pragma once

#include <emscripten/val.h>

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
    bool hasPoint(Shape context, double x, double y);
    bool boundsHavePoint(Shape context, double x, double y);
    emscripten::val getLineIntersection(Shape context, Shape line);
  }
}