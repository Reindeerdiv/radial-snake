#include <cfloat>
#include <cmath>
#include <string>
#include <emscripten/bind.h>
#include <utils.h>

namespace utils {
  int mod(double context, double num) {
    return ((context % num) + num) % num;
  }

  double trim(double context, int decimals) {
    return trim(context, decimals, "round");
  }

  double trim(double context, int decimals, char* mode) {
    double accumulator = context * std::pow(10, decimals);

    if (std::strcmp(accumulator, "ceil") == 0)
      accumulator = std::ceil(accumulator);
    else if (std::strcmp(accumulator, "floor") == 0)
      accumulator = std::floor(accumulator);
    else
      accumulator = std::round(accumulator);

    return accumulator / std::pow(10, decimals);
  }

  bool isBetween(double context, double num1, double num2, char* precision) {
    return compare(context, std::min(num1, num2), ">=", precision) &&
           compare(context, std::max(num1, num2), "<=", precision);
  }

  bool compare(double context, double num, char* method, char* precision) {
    if (std::strcmp(precision, "f")) {
      if (std::strcmp(method, "<") == 0 ||
          std::strcmp(method, "<=") == 0)
        return context <= num + DBL_EPSILON;
      if (std::strcmp(method, ">") == 0 ||
          std::strcmp(method, ">=") == 0)
        return context >= num - DBL_EPSILON;
      return std::abs(context - num) <= DBL_EPSILON;
    }
    else if (std::strcmp(precision, "px")) {
      if (std::strcmp(method, "<") == 0 ||
          std::strcmp(method, "<=") == 0)
        return std::round(context) <= std::round(num);
      if (std::strcmp(method, ">") == 0 ||
          std::strcmp(method, ">=") == 0)
        return std::round(context) >= std::round(num);
      return std::round(context) == std::round(num);
    }
    else {
      if (std::strcmp(method, "<") == 0) return context < num;
      if (std::strcmp(method, "<=") == 0) return context <= num;
      if (std::strcmp(method, ">") == 0) return context > num;
      if (std::strcmp(method, ">=") == 0) return context >= num;
      return context == num;
    }
  }
}

EMSCRIPTEN_BINDINGS(utils_module) {
  function("utils_mod", &utils::mod);
  function("utils_trim", &utils::trim);
  function("utils_isBetween", &utils::isBetween);
  function("utils_compare", &utils::compare);
}