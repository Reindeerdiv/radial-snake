#include <cfloat>
#include <cmath>
#include <string>
#include <utility>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "utils.h"

namespace Utils {
  template <class T>
  template <class Fn, class... Args>
  Chain<T> Chain<T>::link(Fn fn, Args&&... args) {
    auto result = fn(accumulator, std::forward<Args>(args)...);
    Chain chain = new Chain(result);
    delete this;
    return chain;
  }

  template <class T>
  Chain<T>::Chain(T accumulator): accumulator(accumulator) {
  }

  template <class T>
  template <class... Args>
  Chain<T> Chain<T>::mod(Args&&... args) {
    return Chain<T>::link(&mod, std::forward<Args>(args)...);
  }

  template <class T>
  template <class... Args>
  Chain<T> Chain<T>::trim(Args&&... args) {
    return Chain<T>::link(&trim, std::forward<Args>(args)...);
  }

  template <class T>
  template <class... Args>
  Chain<T> Chain<T>::isBetween(Args&&... args) {
    return Chain<T>::link(&isBetween, std::forward<Args>(args)...);
  }

  template <class T>
  template <class... Args>
  Chain<T> Chain<T>::compare(Args&&... args) {
    return Chain<T>::link(&compare, std::forward<Args>(args)...);
  }

  template <class T>
  T Chain<T>::result() {
    delete this;
    return accumulator;
  }

  template <class T>
  Chain<T> chain(T accumulator) {
    return new Chain<T>(accumulator);
  }

  double mod(double context, double num) {
    return std::fmod((std::fmod(context, num) + num), num);
  }

  double trim(double context, int decimals) {
    return trim(context, decimals, "round");
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
  emscripten::function("utils_trim",
    emscripten::select_overload<double(double, int, const std::string)>(&Utils::trim));
  emscripten::function("utils_isBetween", &Utils::isBetween);
  emscripten::function("utils_compare", &Utils::compare);
}