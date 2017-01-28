#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../nullable.h"
#include "../utils.h"
#include "point.h"
#include "line.h"

namespace geometry {
  Line::Line(double x1, double y1, double x2, double y2) {
    _x1 = utils::trim(x1, 9);
    _y1 = utils::trim(y1, 9);
    _x2 = utils::trim(x2, 9);
    _y2 = utils::trim(y2, 9);
  }

  Nullable<double> Line::getMatchingX(double y) {
    // If an error was thrown it means we divided a number by zero,
    // in which case there is not intersection point
    double x = utils::trim(
      (((y - _y1) * (_x2 - _x1)) /
       (_y2 - _y1)) + _x1
    , 9, "exact");

    // Check if result is in values range
    if (utils::isBetween(x, _x1, _x2, "round")) {
      return Nullable<double>(x);
    }

    return Nullable<double>();
  }

  Nullable<double> Line::getMatchingY(double x) {
    // If an error was thrown it means we divided a number by zero,
    // in which case there is not intersection point
    double y = utils::trim(
      (((x - _x1) * (_y2 - _y1)) /
       (_x2 - _x1)) + _y1
    , 9, "exact");

    // Check if result is in values range
    if (utils::isBetween(y, _y1, _y2, "round")) {
      return Nullable<double>(y);
    }

    return Nullable<double>();
  }

  bool Line::hasPoint(double x, double y) {
    if (!boundsHavePoint(x, y)) return 0;

    double m = utils::trim(
      (_y2 - _y1) / (_x2 - _x1),
    9, "exact");

    return (y - _y1) / (x - _x1) == m;
  }

  bool Line::boundsHavePoint(double x, double y) {
    return utils::isBetween(x, _x1, _x2, "round") &&
           utils::isBetween(y, _y1, _y2, "round");
  }

  Nullable<Point> Line::getLineIntersection(Line line) {
    // Escape if lines are parallel
    if (!(((_x1 - _x2) * (line._y1 - line._y2)) -
          ((_y1 - _y2) * (line._x1 - line._x2))))
      return Nullable<Point>();

    // Intersection point formula
    double x = utils::trim(
      ((((_x1 * _y2) - (_y1 * _x2)) * (line._x1 - line._x2)) -
       ((_x1 - _x2) * ((line._x1 * line._y2) - (line._y1 * line._x2)))) /
      (((_x1 - _x2) * (line._y1 - line._y2)) - ((_y1 - _y2) *
        (line._x1 - line._x2)))
    , 9, "exact");
    double y = utils::trim(
      ((((_x1 * _y2) - (_y1 * _x2)) * (line._y1 - line._y2)) -
       ((_y1 - _y2) * ((line._x1 * line._y2) - (line._y1 * line._x2)))) /
      (((_x1 - _x2) * (line._y1 - line._y2)) - ((_y1 - _y2) *
        (line._x1 - line._x2)))
    , 9, "exact");

    if (utils::isBetween(x, _x1, _x2, "round") &&
        utils::isBetween(x, line._x1, line._x2, "round") &&
        utils::isBetween(y, _y1, _y2, "round") &&
        utils::isBetween(y, line._y1, line._y2, "round")) {
      return Nullable<Point>({ x, y });
    }

    return Nullable<Point>();
  }

  EMLine::EMLine(double x1, double y1, double x2, double y2):  Line(x1, y1, x2, y2) {

  }

  emscripten::val EMLine::getMatchingX(double y) {
    Nullable<double> nullableX = Line::getMatchingX(y);
    return nullableX.hasValue() ?
      emscripten::val(nullableX.getValue()) :
      emscripten::val::undefined();
  }

  emscripten::val EMLine::getMatchingY(double x) {
    Nullable<double> nullableY = Line::getMatchingY(x);
    return nullableY.hasValue() ?
      emscripten::val(nullableY.getValue()) :
      emscripten::val::undefined();
  }

  bool EMLine::hasPoint(double x, double y) {
    return Line::hasPoint(x, y);
  }

  bool EMLine::boundsHavePoint(double x, double y) {
    return Line::boundsHavePoint(x, y);
  }

  emscripten::val EMLine::getLineIntersection(EMLine emLine) {
    Line line = Line(emLine._x1, emLine._y1, emLine._x2, emLine._y2);
    Nullable<Point> nullablePoint = Line::getLineIntersection(line);

    if (nullablePoint.isNull()) return emscripten::val::undefined();

    Point point = nullablePoint.getValue();
    emscripten::val emPoint = emscripten::val::object();
    emPoint.set("x", emscripten::val(point.x));
    emPoint.set("y", emscripten::val(point.y));
    return emPoint;
  }

  double EMLine::getX1() const {
    return _x1;
  }

  void EMLine::setX1(double x) {
    _x1 = x;
  }

  double EMLine::getY1() const {
    return _y1;
  }

  void EMLine::setY1(double y) {
    _y1 = y;
  }

  double EMLine::getX2() const {
    return _x2;
  }

  void EMLine::setX2(double x) {
    _x2 = x;
  }

  double EMLine::getY2() const {
    return _y2;
  }

  void EMLine::setY2(double y) {
    _y2 = y;
  }
}

EMSCRIPTEN_BINDINGS(geometry_line_module) {
  emscripten::class_<geometry::EMLine>("geometry_line")
    .constructor<double, double, double, double>()
    .property("x1", &geometry::EMLine::getX1, &geometry::EMLine::setX1)
    .property("y1", &geometry::EMLine::getY1, &geometry::EMLine::setY1)
    .property("x2", &geometry::EMLine::getX2, &geometry::EMLine::setX2)
    .property("y2", &geometry::EMLine::getY2, &geometry::EMLine::setY2)
    .function("getMatchingX", &geometry::EMLine::getMatchingX)
    .function("getMatchingY", &geometry::EMLine::getMatchingY)
    .function("hasPoint", &geometry::EMLine::hasPoint)
    .function("boundsHavePoint", &geometry::EMLine::boundsHavePoint)
    .function("getLineIntersection", &geometry::EMLine::getLineIntersection);
}