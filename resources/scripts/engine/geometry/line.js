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

  // Draws the line on the given context
  draw(context) {
    context.moveTo(this.x1, this.y1);
    context.lineTo(this.x2, this.y2);
  }

  // Gets the matching x value for a given y value
  getX(y) {
    return CPP.Geometry.Line.getX(this, y);
  }

  // Gets the matching y value for a given x value
  getY(x) {
    return CPP.Geometry.Line.getY(this, x);
  }

  // Returns if line has given point
  hasPoint(x, y) {
    return CPP.Geometry.Line.hasPoint(this, x, y);
  }

  // Returns if given point is contained by the bounds aka cage of line
  boundsHavePoint(x, y) {
    return CPP.Geometry.Line.boundsHavePoint(this, x, y);
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
    return CPP.Geometry.Line.getLineIntersection(this, line);
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