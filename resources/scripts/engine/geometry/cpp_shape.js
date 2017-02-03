Engine.Geometry.CPPShape = function CPPShape(shapeName) {
  let CPPShape = CPP.Geometry[shapeName];

  return class extends CPPShape {
    constructor() {
      // Will be the substitute for 'this'
      let shape = new CPPShape(...arguments);
      // Inject the shape into the inheritance chain
      let proto = Engine.Geometry[shapeName].prototype;
      Object.setPrototypeOf(shape, proto);
      return shape;
    }
  }
};