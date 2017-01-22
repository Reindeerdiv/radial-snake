#include <string>

namespace utils {
  namespace literals {
    const std::string ceil = "ceil";
    const std::string floor = "floor";
    const std::string gt = ">";
    const std::string gte = ">=";
    const std::string lt = "<";
    const std::string lte = "<=";
    const std::string fixed = "f";
    const std::string pixel = "px";
  }

  int mod(double context, double num);
  double trim(double context, int decimals, const std::string mode);
  int isBetween(double context, double num1, double num2, const std::string precision);
  int compare(double context, double num, const std::string method, const std::string precision);
}