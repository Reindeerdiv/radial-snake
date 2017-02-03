Engine.Geometry.CPPShape = function CPPShape(shapeName) {
  let CPPShape = CPP.Geometry[shapeName];

  return class extends CPPShape {
    constructor() {
      // Initialize the C++ shape
      let shape = new CPPShape(...arguments);
      // Inject the JS shape into the prototype chain
      Object.setPrototypeOf(shape, new.target.prototype);
      // C++ shape will be the substitute for 'this'
      return shape;
    }
  }
};