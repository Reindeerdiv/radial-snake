#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../utils.h"
#include "point.h"

using namespace Geometry;

namespace Geometry::Line {
  emscripten::val getX(Shape context, double y) {
    // If an error was thrown it means we divided a number by zero,
    // in which case there is not intersection point
    double x = Utils::trim(
      (((y - context.y1) * (context.x2 - context.x1)) /
       (context.y2 - context.y1)) + context.x1
    , 9);

    // Check if result is in values range
    if (Utils::isBetween(x, context.x1, context.x2)) return emscripten::val(x);
    return emscripten::val::undefined();
  }

  emscripten::val getY(Shape context, double x) {
    // If an error was thrown it means we divided a number by zero,
    // in which case there is not intersection point
    double x = Utils::trim(
      (((x - context.x1) * (context.y2 - context.y1)) /
       (context.x2 - context.x1)) + context.y1
    , 9);

    // Check if result is in values range
    if (Utils::isBetween(y, context.y1, context.y2)) return emscripten::val(y);
    return emscripten::val::undefined();
  }

  int hasPoint(Shape context, double x, double y) {
    if (!boundsHavePoint(context, x, y)) return 0;
    double m = Utils::trim((context.y2 - context.y1) / (context.x2 - context.x1), 9);
    return (y - context.y1) / (x - context.x1) == m;
  }

  int boundsHavePoint(Shape context, double x, double y) {
    return Utils::isBetween(x, context.x1, context.x2) &&
           Utils::isBetween(y, context.y1, context.y2);
  }

  Point::Shape getLineIntersection(Shape context, Shape line) {
    Point::Shape result;

    // Escape if lines are parallel
    if (!(((context.x1 - context.x2) * (line.y1 - line.y2)) -
          ((context.y1 - context.y2) * (line.x1 - line.x2)))) return result;

    // Intersection point formula
    double x = Utils::trim(
      ((((context.x1 * context.y2) - (context.y1 * context.x2)) * (line.x1 - line.x2)) -
       ((context.x1 - context.x2) * ((line.x1 * line.y2) - (line.y1 * line.x2)))) /
      (((context.x1 - context.x2) * (line.y1 - line.y2)) - ((context.y1 - context.y2) *
        (line.x1 - line.x2)))
    , 9);
    double y = Utils::trim(
      ((((context.x1 * context.y2) - (context.y1 * context.x2)) * (line.y1 - line.y2)) -
       ((context.y1 - context.y2) * ((line.x1 * line.y2) - (line.y1 * line.x2)))) /
      (((context.x1 - context.x2) * (line.y1 - line.y2)) - ((context.y1 - context.y2) *
        (line.x1 - line.x2)))
    , 9);

    if (Utils::isBetween(x, context.x1, context.x2) &&
        Utils::isBetween(x, line.x1, line.x2) &&
        Utils::isBetween(y, context.y1, context.y2) &&
        Utils::isBetween(y, line.y1, line.y2)) {
      result.x = x;
      result.y = y;
    }

    return result;
  }
}

EMSCRIPTEN_BINDINGS(geometry_line_module) {
  emscripten::value_object<Line::Shape>("geometry_line_shape")
    .field("x1", &Line::Shape::x1)
    .field("y1", &Line::Shape::y1)
    .field("x2", &Line::Shape::x2)
    .field("y2", &Line::Shape::y2);

  emscripten::function("geometry_line_getX", &Line::getX);
  emscripten::function("geometry_line_getY", &Line::getY);
  emscripten::function("geometry_line_hasPoint", &Line::hasPoint);
  emscripten::function("geometry_line_boundsHavePoint", &Utils::boundsHavePoint);
  emscripten::function("geometry_line_getLineIntersection", &Utils::getLineIntersection);
}