namespace utils {
  namespace literals {
    const char* ceil = "ceil";
    const char* floor = "floor";
    const char* round = "round";
    const char* gt = ">";
    const char* gte = ">=";
    const char* lt = "<";
    const char* lte = "<=";
    const char* fixed = "f";
    const char* pixel = "px";
  }

  int mod(double context, double num);
  double trim(double context, int decimals);
  double trim(double context, int decimals, const char* mode);
  int isBetween(double context, double num1, double num2, const char* precision);
  int compare(double context, double num, const char* method, const char* precision);
}