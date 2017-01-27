#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../utils.h"
#include "line.h"

namespace Geometry {
  Line::Line(double x1, double y1, double x2, double y2):
             _x1(x1), _y1(y1), _x2(x2), _y2(y2) {

  }

  emscripten::val Line::getMatchingX(double y) {
    // If an error was thrown it means we divided a number by zero,
    // in which case there is not intersection point
    double x = Utils::trim(
      (((y - _y1) * (_x2 - _x1)) /
       (_y2 - _y1)) + _x1
    , 9, "exact");

    // Check if result is in values range
    if (Utils::isBetween(x, _x1, _x2, "round")) {
      return emscripten::val(x);
    }

    return emscripten::val::undefined();
  }

  emscripten::val Line::getMatchingY(double x) {
    // If an error was thrown it means we divided a number by zero,
    // in which case there is not intersection point
    double y = Utils::trim(
      (((x - _x1) * (_y2 - _y1)) /
       (_x2 - _x1)) + _y1
    , 9, "exact");

    // Check if result is in values range
    if (Utils::isBetween(y, _y1, _y2, "round")) {
      return emscripten::val(y);
    }

    return emscripten::val::undefined();
  }

  bool Line::hasPoint(double x, double y) {
    if (!boundsHavePoint(x, y)) return 0;

    double m = Utils::trim(
      (_y2 - _y1) / (_x2 - _x1),
    9, "exact");

    return (y - _y1) / (x - _x1) == m;
  }

  bool Line::boundsHavePoint(double x, double y) {
    return Utils::isBetween(x, _x1, _x2, "round") &&
           Utils::isBetween(y, _y1, _y2, "round");
  }

  emscripten::val Line::getLineIntersection(Line line) {
    // Escape if lines are parallel
    if (!(((_x1 - _x2) * (line.getY1() - line.getY2())) -
          ((_y1 - _y2) * (line.getX1() - line.getX2()))))
      return emscripten::val::undefined();

    // Intersection point formula
    double x = Utils::trim(
      ((((_x1 * _y2) - (_y1 * _x2)) * (line.getX1() - line.getX2())) -
       ((_x1 - _x2) * ((line.getX1() * line.getY2()) - (line.getY1() * line.getX2())))) /
      (((_x1 - _x2) * (line.getY1() - line.getY2())) - ((_y1 - _y2) *
        (line.getX1() - line.getX2())))
    , 9, "exact");
    double y = Utils::trim(
      ((((_x1 * _y2) - (_y1 * _x2)) * (line.getY1() - line.getY2())) -
       ((_y1 - _y2) * ((line.getX1() * line.getY2()) - (line.getY1() * line.getX2())))) /
      (((_x1 - _x2) * (line.getY1() - line.getY2())) - ((_y1 - _y2) *
        (line.getX1() - line.getX2())))
    , 9, "exact");

    if (Utils::isBetween(x, _x1, _x2, "round") &&
        Utils::isBetween(x, line.getX1(), line.getX2(), "round") &&
        Utils::isBetween(y, _y1, _y2, "round") &&
        Utils::isBetween(y, line.getY1(), line.getY2(), "round")) {
      emscripten::val js_result = emscripten::val::object();
      js_result.set("x", x);
      js_result.set("y", y);
      return js_result;
    }

    return emscripten::val::undefined();
  }

  double Line::getX1() const {
    return _x1;
  }

  void Line::setX1(double x) {
    _x1 = x;
  }

  double Line::getY1() const {
    return _y1;
  }

  void Line::setY1(double y) {
    _y1 = y;
  }

  double Line::getX2() const {
    return _x2;
  }

  void Line::setX2(double x) {
    _x2 = x;
  }

  double Line::getY2() const {
    return _y2;
  }

  void Line::setY2(double y) {
    _y2 = y;
  }
}

EMSCRIPTEN_BINDINGS(geometry_line_module) {
  emscripten::class_<Geometry::Line>("geometry_line")
    .constructor<double, double, double, double>()
    .function("getMatchingX", &Geometry::Line::getMatchingX)
    .function("getMatchingY", &Geometry::Line::getMatchingY)
    .function("hasPoint", &Geometry::Line::hasPoint)
    .function("boundsHavePoint", &Geometry::Line::boundsHavePoint)
    .function("getLineIntersection", &Geometry::Line::getLineIntersection)
    .property("x1", &Geometry::Line::getX1, &Geometry::Line::setX1)
    .property("y1", &Geometry::Line::getY1, &Geometry::Line::setY1)
    .property("x2", &Geometry::Line::getX2, &Geometry::Line::setX2)
    .property("y2", &Geometry::Line::getY2, &Geometry::Line::setY2);


  // emscripten::value_object<Geometry::Line::Shape>("geometry_line_shape")
  //   .field("x1", &Geometry::Line::Shape::x1)
  //   .field("y1", &Geometry::Line::Shape::y1)
  //   .field("x2", &Geometry::Line::Shape::x2)
  //   .field("y2", &Geometry::Line::Shape::y2);

  // emscripten::function("geometry_line_getX",
  //   &Geometry::Line::getX);
  // emscripten::function("geometry_line_getY",
  //   &Geometry::Line::getY);
  // emscripten::function("geometry_line_hasPoint",
  //   &Geometry::Line::hasPoint);
  // emscripten::function("geometry_line_boundsHavePoint",
  //   &Geometry::Line::boundsHavePoint);
  // emscripten::function("geometry_line_getLineIntersection",
  //   &Geometry::Line::getLineIntersection);
}