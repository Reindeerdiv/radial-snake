namespace utils {
  int mod(double context, double num);
  double trim(double context, int decimals);
  double trim(double context, int decimals, char* mode);
  bool isBetween(double context, double num1, double num2, char* precision);
  bool compare(double context, double num, char* method, char* precision);
}