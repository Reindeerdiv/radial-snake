#include <cfloat>
#include <cmath>
#include <string>
#include <utility>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "utils.h"

namespace utils {
  template<typename T>
  template<typename U, typename Fn, typename... Args>
  Chain<U> Chain<T>::link(Fn fn, Args&&... args) {
    U result = fn(_accumulator, std::forward<Args>(args)...);
    Chain<U> chain = new Chain<U>(result);
    delete this;
    return chain;
  }

  template<typename T>
  Chain<T>::Chain(T accumulator): _accumulator(accumulator) {
  }

  template<>
  template<typename... Args>
  Chain<double> Chain<double>::mod(Args&&... args) {
    return link<double>(&mod, std::forward<Args>(args)...);
  }

  template<>
  template<typename... Args>
  Chain<double> Chain<double>::trim(Args&&... args) {
    return link<double>(&trim, std::forward<Args>(args)...);
  }

  template<>
  template<typename... Args>
  Chain<bool> Chain<double>::isBetween(Args&&... args) {
    return link<bool>(&isBetween, std::forward<Args>(args)...);
  }

  template<>
  template<typename... Args>
  Chain<bool> Chain<double>::compare(Args&&... args) {
    return link<bool>(&compare, std::forward<Args>(args)...);
  }

  template<typename T>
  T Chain<T>::result() {
    delete this;
    return _accumulator;
  }

  template<typename T>
  Chain<T> chain(T accumulator) {
    return new Chain<T>(accumulator);
  }

  double mod(double context, double num) {
    return std::fmod((std::fmod(context, num) + num), num);
  }

  double trim(double context, int decimals, const std::string mode = "round") {
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
  emscripten::function("utils_mod", &utils::mod);
  emscripten::function("utils_trim", &utils::trim);
  emscripten::function("utils_isBetween", &utils::isBetween);
  emscripten::function("utils_compare", &utils::compare);
}