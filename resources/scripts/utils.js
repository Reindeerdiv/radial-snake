// A wrapper function for our utilities which will enable chaining
// e.g. Utils().mod().trim().isBetween()...
Utils = function Utils(context) {
  const chain = {};

  Object.keys(Utils).forEach((utilName) => {
    chain[utilName] = (...args) => {
      const result = Utils[utilName](context, ...args);
      return Utils(result);
    };
  });

  // Returns the result of the chaining
  chain.value = () => context;

  return chain;
};

// Fixed modulo method which can calculate modulo of negative numbers properly
// e.g. (-803).mod(800) returns 797
Utils.mod = function (context, num) {
  return CPP.Utils.mod(context, num);
};

// Trims number and leaves the number of decimals specified.
// The "mode" argument specifies which math function should be invoked
// right after the number has been trimmed.
// e.g. 12.12345.trim(3, "ceil") returns 12.124
Utils.trim = function (context, decimals, mode = "round") {
  return CPP.Utils.trim(context, decimals, mode);
};

// Tells if number is in specified range based on given precision.
// See the "compare" method for more information about precision
Utils.isBetween = function (context, num1, num2, precision = "exact") {
  return CPP.Utils.isBetween(context, num1, num2, precision);
};

// Initiates comparison operator between context number and a given number, only here
// a precision can be specified
Utils.compare = function (context, num, method, precision) {
  return CPP.Utils.compare(context, num, method, precision);
};