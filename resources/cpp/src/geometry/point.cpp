#include <emscripten/bind.h>
#include "point.h"

using namespace Geometry;

EMSCRIPTEN_BINDINGS(geometry_point_module) {
  emscripten::value_object<Point::Shape>("geometry_point_shape")
    .field("x", &Point::Shape::x)
    .field("y", &Point::Shape::y);
}