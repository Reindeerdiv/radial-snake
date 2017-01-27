#pragma once

#include <emscripten/val.h>

namespace Geometry {
  class Line {
    private:
      double _x1;
      double _y1;
      double _x2;
      double _y2;

    public:
      Line(double x1, double y1, double x2, double y2);

      emscripten::val getMatchingX(double y);

      emscripten::val getMatchingY(double x);

      bool hasPoint(double x, double y);

      bool boundsHavePoint(double x, double y);

      emscripten::val getLineIntersection(Line line);

      double getX1() const;

      void setX1(double x);

      double getY1() const;

      void setY1(double y);

      double getX2() const;

      void setX2(double x);

      double getY2() const;

      void setY2(double y);
  };
}