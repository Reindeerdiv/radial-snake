#pragma once

#include <emscripten/val.h>
#include "../nullable.h"
#include "point.h"

namespace geometry {
  class Line {
  public:
    double _x1;
    double _y1;
    double _x2;
    double _y2;

    Line(double x1, double y1, double x2, double y2);

    Nullable<double> getMatchingX(double y);

    Nullable<double> getMatchingY(double x);

    bool hasPoint(double x, double y);

    bool boundsHavePoint(double x, double y);

    Nullable<Point> getLineIntersection(Line line);
  };

  class EMLine : public Line {
  public:
    EMLine(double x1, double y1, double x2, double y2);

    emscripten::val getMatchingX(double y);

    emscripten::val getMatchingY(double x);

    bool hasPoint(double x, double y);

    bool boundsHavePoint(double x, double y);

    emscripten::val getLineIntersection(EMLine line);

    double getX1() const;

    void setX1(double x);

    double getY1() const;

    void setY1(double x);

    double getX2() const;

    void setX2(double x);

    double getY2() const;

    void setY2(double x);
  };
}