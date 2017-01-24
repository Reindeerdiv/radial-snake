#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../utils.h"
#include "line.h"

namespace Geometry {
  namespace Line {
    emscripten::val getX(Shape context, double y) {
      // If an error was thrown it means we divided a number by zero,
      // in which case there is not intersection point
      double x = Utils::trim(
        (((y - context.y1) * (context.x2 - context.x1)) /
         (context.y2 - context.y1)) + context.x1
      , 9, "exact");

      // Check if result is in values range
      if (Utils::isBetween(x, context.x1, context.x2, "round")) {
        return emscripten::val(x);
      }

      return emscripten::val::undefined();
    }

    emscripten::val getY(Shape context, double x) {
      // If an error was thrown it means we divided a number by zero,
      // in which case there is not intersection point
      double y = Utils::trim(
        (((x - context.x1) * (context.y2 - context.y1)) /
         (context.x2 - context.x1)) + context.y1
      , 9, "exact");

      // Check if result is in values range
      if (Utils::isBetween(y, context.y1, context.y2, "round")) {
        return emscripten::val(y);
      }

      return emscripten::val::undefined();
    }

    bool hasPoint(Shape context, double x, double y) {
      if (!boundsHavePoint(context, x, y)) return 0;

      double m = Utils::trim(
        (context.y2 - context.y1) / (context.x2 - context.x1),
      9, "exact");

      return (y - context.y1) / (x - context.x1) == m;
    }

    bool boundsHavePoint(Shape context, double x, double y) {
      return Utils::isBetween(x, context.x1, context.x2, "round") &&
             Utils::isBetween(y, context.y1, context.y2, "round");
    }

    emscripten::val getLineIntersection(Shape context, Shape line) {
      // Escape if lines are parallel
      if (!(((context.x1 - context.x2) * (line.y1 - line.y2)) -
            ((context.y1 - context.y2) * (line.x1 - line.x2))))
        return emscripten::val::undefined();

      // Intersection point formula
      double x = Utils::trim(
        ((((context.x1 * context.y2) - (context.y1 * context.x2)) * (line.x1 - line.x2)) -
         ((context.x1 - context.x2) * ((line.x1 * line.y2) - (line.y1 * line.x2)))) /
        (((context.x1 - context.x2) * (line.y1 - line.y2)) - ((context.y1 - context.y2) *
          (line.x1 - line.x2)))
      , 9, "exact");
      double y = Utils::trim(
        ((((context.x1 * context.y2) - (context.y1 * context.x2)) * (line.y1 - line.y2)) -
         ((context.y1 - context.y2) * ((line.x1 * line.y2) - (line.y1 * line.x2)))) /
        (((context.x1 - context.x2) * (line.y1 - line.y2)) - ((context.y1 - context.y2) *
          (line.x1 - line.x2)))
      , 9, "exact");

      if (Utils::isBetween(x, context.x1, context.x2, "round") &&
          Utils::isBetween(x, line.x1, line.x2, "round") &&
          Utils::isBetween(y, context.y1, context.y2, "round") &&
          Utils::isBetween(y, line.y1, line.y2, "round")) {
        emscripten::val js_result = emscripten::val::object();
        js_result.set("x", x);
        js_result.set("y", y);
        return js_result;
      }

      return emscripten::val::undefined();
    }
  }
}

EMSCRIPTEN_BINDINGS(geometry_line_module) {
  emscripten::function("geometry_line_getX",
    &Geometry::Line::getX);
  emscripten::function("geometry_line_getY",
    &Geometry::Line::getY);
  emscripten::function("geometry_line_hasPoint",
    &Geometry::Line::hasPoint);
  emscripten::function("geometry_line_boundsHavePoint",
    &Geometry::Line::boundsHavePoint);
  emscripten::function("geometry_line_getLineIntersection",
    &Geometry::Line::getLineIntersection);
}