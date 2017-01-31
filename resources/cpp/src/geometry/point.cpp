bool operator==(const geometry::Point& pointA, const geometry::Point& pointB) {
  return pointA.x == pointB.x && pointA.y == pointB.y;
}

bool operator!=(const geometry::Point& pointA, const geometry::Point& pointB) {
  return pointA.x != pointB.x || pointA.y != pointB.y;
}