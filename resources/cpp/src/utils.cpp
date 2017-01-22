#include <cfloat>
#include <cmath>
#include <string>
#include <emscripten/bind.h>
#include "utils.h"

namespace utils {
  int mod(double context, double num) {
    return std::fmod((std::fmod(context, num) + num), num);
  }

  double trim(double context, int decimals, const std::string mode) {
    double accumulator = context * std::pow(10, decimals);

    if (mode.compare(literals::ceil) == 0)
      accumulator = std::ceil(accumulator);
    else if (mode.compare(literals::floor) == 0)
      accumulator = std::floor(accumulator);
    else
      accumulator = std::round(accumulator);

    return accumulator / std::pow(10, decimals);
  }

  int isBetween(double context, double num1, double num2, const std::string precision) {
    return compare(context, std::min(num1, num2), literals::gte, precision) &&
           compare(context, std::max(num1, num2), literals::lte, precision);
  }

  int compare(double context, double num, const std::string method, const std::string precision) {
    // Fixed precision, "almost equal" with a deviation of ε
    if (precision.compare(literals::fixed)) {
      if (method.compare(literals::lt) == 0 ||
          method.compare(literals::lte) == 0)
        return context <= num + DBL_EPSILON;
      if (method.compare(literals::gt) == 0 ||
          method.compare(literals::gte) == 0)
        return context >= num - DBL_EPSILON;
      return std::abs(context - num) <= DBL_EPSILON;
    }
    // Pixel precision, round comparison
    else if (precision.compare(literals::pixel)) {
      if (method.compare(literals::lt) == 0 ||
          method.compare(literals::lte) == 0)
        return std::round(context) <= std::round(num);
      if (method.compare(literals::gt) == 0 ||
          method.compare(literals::gte) == 0)
        return std::round(context) >= std::round(num);
      return std::round(context) == std::round(num);
    }
    // Exact precision
    else {
      if (method.compare(literals::lt) == 0) return context < num;
      if (method.compare(literals::lte) == 0) return context <= num;
      if (method.compare(literals::gt) == 0) return context > num;
      if (method.compare(literals::gte) == 0) return context >= num;
      return context == num;
    }
  }
}

EMSCRIPTEN_BINDINGS(utils_module) {
  emscripten::function("utils_mod", &utils::mod);
  emscripten::function("utils_trim", &utils::trim);
  emscripten::function("utils_isBetween", &utils::isBetween);
  emscripten::function("utils_compare", &utils::compare);
}