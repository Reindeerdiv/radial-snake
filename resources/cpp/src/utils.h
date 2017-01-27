#pragma once

#include <string>

namespace Utils {
  template<typename T>
  class Chain {
    private:
      T _accumulator;

      template<typename U, typename Fn, class... Args>
      Chain<U> link(Fn fn, Args&&... args);

    public:
      Chain(T accumulator);

      template<typename... Args>
      Chain<double> mod(Args&&... args);

      template<typename... Args>
      Chain<double> trim(Args&&... args);

      template<typename... Args>
      Chain<bool> isBetween(Args&&... args);

      template<typename... Args>
      Chain<bool> compare(Args&&... args);

      T result();
  };

  template<typename T>
  Chain<T> chain(T accumulator);

  double mod(double context, double num);

  double trim(double context, int decimals, const std::string mode);

  bool isBetween(double context, double num1, double num2, const std::string precision);

  bool compare(double context, double num, const std::string method, const std::string precision);
}