#pragma once

#include <string>

namespace Utils {
  template <class T>
  class Chain {
    private:
      T accumulator;

      template <class Fn, class... Args>
      Chain<T> link(Fn fn, Args&&... args);

    public:
      Chain(T accumulator);

      template <class... Args>
      Chain<T> mod(Args&&... args);

      template <class... Args>
      Chain<T> trim(Args&&... args);

      template <class... Args>
      Chain<T> isBetween(Args&&... args);

      template <class... Args>
      Chain<T> compare(Args&&... args);

      T result();
  };

  template <class T>
  Chain<T> chain(T accumulator);

  double mod(double context, double num);

  double trim(double context, int decimals);

  double trim(double context, int decimals, const std::string mode);

  bool isBetween(double context, double num1, double num2, const std::string precision);

  bool compare(double context, double num, const std::string method, const std::string precision);
}