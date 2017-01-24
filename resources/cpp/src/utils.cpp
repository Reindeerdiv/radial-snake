#include <cfloat>
#include <cmath>
#include <string>
#include <emscripten/bind.h>
#include "utils.h"

namespace Utils {
  double mod(double context, double num) {
    return std::fmod((std::fmod(context, num) + num), num);
  }

  double trim(double context, int decimals, const std::string mode) {
    double accumulator = context * std::pow(10, decimals);

    if (mode.compare("ceil") == 0)
      accumulator = std::ceil(accumulator);
    else if (mode.compare("floor") == 0)
      accumulator = std::floor(accumulator);
    else
      accumulator = std::round(accumulator);

    return accumulator / std::pow(10, decimals);
  }

  bool isBetween(double context, double num1, double num2, const std::string precision) {
    return compare(context, std::min(num1, num2), ">=", precision) &&
           compare(context, std::max(num1, num2), "<=", precision);
  }

  bool compare(double context, double num, const std::string method, const std::string precision) {
    // Fixed precision, "almost equal" with a deviation of ε
    if (precision.compare("fixed")) {
      if (method.compare("<") == 0 ||
          method.compare("<=") == 0)
        return context <= num + DBL_EPSILON;
      if (method.compare(">") == 0 ||
          method.compare(">=") == 0)
        return context >= num - DBL_EPSILON;
      return std::abs(context - num) <= DBL_EPSILON;
    }
    // Pixel precision, round comparison
    else if (precision.compare("pixel")) {
      if (method.compare("<") == 0 ||
          method.compare("<=") == 0)
        return std::round(context) <= std::round(num);
      if (method.compare(">") == 0 ||
          method.compare(">=") == 0)
        return std::round(context) >= std::round(num);
      return std::round(context) == std::round(num);
    }
    // Exact precision
    else {
      if (method.compare("<") == 0) return context < num;
      if (method.compare("<=") == 0) return context <= num;
      if (method.compare(">") == 0) return context > num;
      if (method.compare(">=") == 0) return context >= num;
      return context == num;
    }
  }
}

EMSCRIPTEN_BINDINGS(utils_module) {
  emscripten::function("utils_mod", &Utils::mod);
  emscripten::function("utils_trim", &Utils::trim);
  emscripten::function("utils_isBetween", &Utils::isBetween);
  emscripten::function("utils_compare", &Utils::compare);
}