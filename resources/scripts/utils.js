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
  compare(arg0, arg1, arg2, arg3) {
    let compare = CPP.Utils.compare;

    switch (arguments.length) {
      case 2: return compare(arg0, arg1);
      case 3: return compare(arg0, arg1, arg3);
      case 4: return compare(arg0, arg1, arg2, arg3);
    }
  },

  proxy(Class) {
    return class extends Class {
      constructor() {
        // Initialize original class
        let that = new Class(...arguments);
        // Inject caller's prototype into the prototype chain
        Object.setPrototypeOf(that, new.target.prototype);
        // Will original instance will be the substitute for 'this'
        return that;
      }
    }
  }
});