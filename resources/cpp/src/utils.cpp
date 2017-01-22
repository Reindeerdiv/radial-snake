#include <cfloat>
#include <cmath>
#include <string>
#include <emscripten/bind.h>
#include "utils.h"

namespace utils {
  int mod(double context, double num) {
    return std::fmod((std::fmod(context, num) + num), num);
  }

  double trim(double context, int decimals) {
    return trim(context, decimals, literals::round);
  }

  double trim(double context, int decimals, const char* mode) {
    double accumulator = context * std::pow(10, decimals);

    if (std::strcmp(mode, literals::ceil) == 0)
      accumulator = std::ceil(accumulator);
    else if (std::strcmp(mode, literals::floor) == 0)
      accumulator = std::floor(accumulator);
    else
      accumulator = std::round(accumulator);

    return accumulator / std::pow(10, decimals);
  }

  int isBetween(double context, double num1, double num2, const char* precision) {
    return compare(context, std::min(num1, num2), literals::gte, precision) &&
           compare(context, std::max(num1, num2), literals::lte, precision);
  }

  int compare(double context, double num, const char* method, const char* precision) {
    if (std::strcmp(precision, literals::fixed)) {
      if (std::strcmp(method, literals::lt) == 0 ||
          std::strcmp(method, literals::lte) == 0)
        return context <= num + DBL_EPSILON;
      if (std::strcmp(method, literals::gt) == 0 ||
          std::strcmp(method, literals::gte) == 0)
        return context >= num - DBL_EPSILON;
      return std::abs(context - num) <= DBL_EPSILON;
    }
    else if (std::strcmp(precision, literals::pixel)) {
      if (std::strcmp(method, literals::lt) == 0 ||
          std::strcmp(method, literals::lte) == 0)
        return std::round(context) <= std::round(num);
      if (std::strcmp(method, literals::gt) == 0 ||
          std::strcmp(method, literals::gte) == 0)
        return std::round(context) >= std::round(num);
      return std::round(context) == std::round(num);
    }
    else {
      if (std::strcmp(method, literals::lt) == 0) return context < num;
      if (std::strcmp(method, literals::lte) == 0) return context <= num;
      if (std::strcmp(method, literals::gt) == 0) return context > num;
      if (std::strcmp(method, literals::gte) == 0) return context >= num;
      return context == num;
    }
  }
}

EMSCRIPTEN_BINDINGS(utils_module) {
  emscripten::function("utils_mod",
    &utils::mod);

  emscripten::function("utils_trim_round",
    emscripten::select_overload<double(double, int)>(&utils::trim));

  emscripten::function("utils_trim",
    emscripten::select_overload<double(double, int, const char*)>(&utils::trim),
    emscripten::allow_raw_pointers());

  emscripten::function("utils_isBetween",
    &utils::isBetween,
    emscripten::allow_raw_pointers());

  emscripten::function("utils_compare",
    &utils::compare,
    emscripten::allow_raw_pointers());
}