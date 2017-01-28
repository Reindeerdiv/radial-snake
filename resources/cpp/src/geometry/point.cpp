#include <emscripten/bind.h>
#include "point.h"

EMSCRIPTEN_BINDINGS(geometry_point) {
  emscripten::value_object<&geometry::Point>("geometry_point")
    .field("x", &geometry::Point::x)
    .field("y", &geometry::Point::y);
}