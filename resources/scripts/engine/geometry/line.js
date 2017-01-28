Engine.Geometry.Line = class Line {
  // x1 - The first point's x value
  // y1 - The first point's y value
  // x1 - The second point's x value
  // y2 - The second point's y value
  constructor(x1, y1, x2, y2) {
    this.x1 = Utils.trim(x1, 9);
    this.y1 = Utils.trim(y1, 9);
    this.x2 = Utils.trim(x2, 9);
    this.y2 = Utils.trim(y2, 9);
  }

  _ccall(method, ...args) {
    let line = new CPP.Geometry.Line(this.x1, this.y1, this.x2, this.y2);
    let result = line[method](...args);
    line.delete();
    return result;
  }

  // Draws the line on the given context
  draw(context) {
    context.moveTo(this.x1, this.y1);
    context.lineTo(this.x2, this.y2);
  }

  // Gets the matching x value for a given y value
  getX(y) {
    return this._ccall("getMatchingX", y);
  }

  // Gets the matching y value for a given x value
  getY(x) {
    return this._ccall("getMatchingY", x);
  }

  // Returns if line has given point
  hasPoint(x, y) {
    return this._ccall("hasPoint", x, y);
  }

  // Returns if given point is contained by the bounds aka cage of line
  boundsHavePoint(x, y) {
    return this._ccall("boundsHavePoint", x, y);
  }

  getIntersection(shape) {
    if (shape instanceof Engine.Geometry.Line)
      return this.getLineIntersection(shape);
    if (shape instanceof Engine.Geometry.Circle)
      return this.getCircleIntersection(shape);
    if (shape instanceof Engine.Geometry.Polygon)
      return this.getPolygonIntersection(shape);
  }

  // line - line intersection method
  getLineIntersection(line) {
    let cline = new CPP.Geometry.Line(line.x1, line.y1, line.x2, line.y2);
    let result = this._ccall("getLineIntersection", cline);
    cline.delete();
    return result;
  }

  // line - circle intersection method
  getCircleIntersection(circle) {
    return circle.getLineIntersection(this);
  }

  // line - polygon intersection method
  getPolygonIntersection(polygon) {
    return polygon.getLineIntersection(this);
  }
};