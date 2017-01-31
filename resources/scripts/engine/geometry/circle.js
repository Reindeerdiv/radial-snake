Engine.Geometry.Circle = class Circle {
  // x - The x value of the circle's center
  // y - The y value of the circle's center
  // r - The radius of the center
  // rad1 - The first radian of the circle, not necessarily its beginning
  // rad2 - The second radian of the circle, not necessarily its beginning
  constructor(x, y, r, rad1, rad2) {
    this.x = Utils.trim(x, 9);
    this.y = Utils.trim(y, 9);
    this.r = Utils.trim(r, 9);

    // Trimming mode is done based on which radian represents the ending and which radian
    // represents the ending
    if (rad1 > rad2) {
      this.rad1 = Utils.trim(rad1, 9, "floor");
      this.rad2 = Utils.trim(rad2, 9, "ceil");
    }
    else {
      this.rad1 = Utils.trim(rad1, 9, "ceil");
      this.rad2 = Utils.trim(rad2, 9, "floor");
    }
  }

  _ccall(method, ...args) {
    let circle = new CPP.Geometry.Circle(this.x, this.y, this.r, this.rad1, this.rad2);
    let result = circle[method](...args);
    circle.delete();
    return result;
  }

  // Draws the circle on the given context
  draw(context) {
    context.arc(this.x, this.y, this.r, this.rad1, this.rad2);
  }

  // Gets the matching x value for the given radian
  getX(rad) {
    return this._ccall("getMatchingX", rad);
  }

  // Gets the matching y value for the given radian
  getY(rad) {
    return this._ccall("getMatchingY", rad);
  }

  // Gets the matching point for the given radian
  getPoint(rad) {
    return this._ccall("getMatchingPoint", rad);
  }

  // Gets the matching radian for the given point
  getRad(x, y) {
    return this._ccall("getMatchingRad", x, y);
  }

  // Returns if circle has given points
  hasPoint(x, y) {
    return this._ccall("hasPoint", x, y);
  }

  getIntersection(shape) {
    if (shape instanceof Engine.Geometry.Line)
      return this.getLineIntersection(shape);
    if (shape instanceof Engine.Geometry.Circle)
      return this.getCircleIntersection(shape);
    if (shape instanceof Engine.Geometry.Polygon)
      return this.getPolygonIntersection(shape);
  }

  // circle - circle intersection method
  getCircleIntersection(circle) {
    return this._ccall("getCircleIntersection", circle);
  }

  // circle - line intersection method
  getLineIntersection(line) {
    return this._ccall("getLineIntersection", line);
  }

  // circle - polygon intersection method
  getPolygonIntersection(polygon) {
    return polygon.getCircleIntersection(this);
  }
};