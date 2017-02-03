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

Object.assign(Utils, CPP.Utils, {
  // Overload handling
  compare: function (arg0, arg1, arg2, arg3) {
    let compare = CPP.Utils.compare;

    switch (arguments.length) {
      case 2: return compare(arg0, arg1);
      case 3: return compare(arg0, arg1, arg3);
      case 4: return compare(arg0, arg1, arg2, arg3);
    }
  }
});