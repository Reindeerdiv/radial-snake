Engine.Geometry.CPPShape = function CPPShape(shapeName) {
  let CPPShape = CPP.Geometry[shapeName];
  // This is a weakly held set which will store all the instances
  let shapes = new WeakSet();

  return class JSShape extends CPPShape {
    static [Symbol.hasInstance](shape) {
      // If the shape is in the instances set, 'insatnceof' will return true
      return shapes.has(shape);
    }

    constructor() {
      // Will be the substitute for 'this'
      let shape = new CPPShape(...arguments);
      // Inject the shape into the inheritance chain
      let proto = Engine.Geometry[shapeName].prototype;
      Object.setPrototypeOf(shape, proto);
      // This will make the shape an instance of this class
      shapes.add(shape);
      return shape;
    }
  }
};