#include <algorithm>
#include <cmath>
#include <vector>
#include "point.h"
#include "line.h"
#include "../nullable.h"
#include "../utils.h"

namespace geometry {
  Circle::Circle(double x, double y, double r, double rad1, double rad2) {
    _x = utils::trim(x, 9);
    _y = utils::trim(y, 9);
    _r = utils::trim(r, 9);

    // Trimming mode is done based on which radian represents the ending and which radian
    // represents the ending
    if (rad1 > rad2) {
      _rad1 = utils::trim(rad1, 9, "floor");
      _rad2 = utils::trim(rad2, 9, "ceil");
    }
    else {
      _rad1 = utils::trim(rad1, 9, "ceil");
      _rad2 = utils::trim(rad2, 9, "floor");
    }
  }

  // Gets the matching x value for the given radian
  Nullable<double> Circle::getMatchingX(double rad) {
    if (!utils::chain(rad).trim(9).isBetween(_rad1, _rad2).result()) {
      return Nullable<double>();
    }

    return Nullable<double>(utils::trim((_r * std::cos(rad)) + _x, 9));
  }

  // Gets the matching y value for the given radian
  Nullable<double> Circle::getMatchingX(double rad) {
    if (!utils::chain(rad).trim(9).isBetween(_rad1, _rad2).result()) {
      return Nullable<double>();
    }

    return Nullable<double>(utils::trim((_r * std::sin(rad)) + _y, 9));
  }

  // Gets the matching point for the given radian
  Nullable<Point> Circle::getMatchingPoint(double rad) {
    if (!utils::isBetween(rad, _rad1, _rad2)) {
      return Nullable<Point>();
    }

    return Nullable<Point>({
      utils::trim((_r * std::cos(rad)) + _x, 9),
      utils::trim((_r * std::sin(rad)) + _y, 9)
    });
  }

  // Gets the matching radian for the given point
  Nullable<double> Circle::getMatchingRad(double x, double y) {
    double rad = std::atan2(y - _y, x - _x);

    // If calculated radian is in circle's radian range, return it
    if (!std::isnan(rad) && utils::isBetween(rad, _rad1, _rad2)) {
      return Nullable<double>(rad);
    }

    // The calculated radian can still be in the circle's radian range in case one
    // of the radians is greater than 2 PIEs
    double greatestRad = std::abs(_rad1) > std::abs(_rad2) ? _rad1 : _rad2;

    // Check if the absolute radian is in the circle's radian range
    if (utils::chain(rad + (2 * M_PI * std::floor(greatestRad / (2 * M_PI)))).trim(9).isBetween(_rad1, _rad2).result() ||
        utils::chain(rad + (2 * M_PI * std::ceil(greatestRad / (2 * M_PI)))).trim(9).isBetween(_rad1, _rad2).result()) {
      return Nullable<double>(rad);
    }

    return Nullable<double>();
  }

  // Returns if circle has given points
  bool Circle::hasPoint(double x, double y) {
    return getRad(x, y).hasValue();
  }

  // circle - circle intersection method
  Nullable<std::vector<Point>> getIntersection(Circle circle) {
    double dx = circle._x - _x;
    double dy = circle._y - _y;
    double d = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

    if (d > _r + circle._r ||
       d < std::abs(_r - circle._r)) {
      return Nullable<std::vector<Point>>();
    }

    double a = ((std::pow(_r, 2) - std::pow(circle._r, 2)) + std::pow(d, 2)) / (2 * d);
    double x = _x + ((dx * a) / d);
    double y = _y + ((dy * a) / d);
    double h = std::sqrt(std::pow(_r, 2) - std::pow(a, 2));
    double rx = (- dy * h) / d;
    double ry = (dx * h) / d;

    std::vector<Point> interPoints = {
      { x + rx, y + ry },
      { x - rx, y - ry }
    };

    std::transform(interPoints.begin(), interPoints.end(), interPoints.begin(),
      [](std::vector<Point> point) {
        return { utils::trim(point.x, 9), utils::trim(point.y, 9) }
      }
    );

    auto end = std::unique(interPoints.begin(), interPoints.end(),
      [](std::vector<Point> pointA, std::vector<Point> pointB) {
        return pointA.x == pointB.x && pointA.y == pointB.y;
      }
    );

    interPoints.erase(interPoints.begin(), end);

    std::vector<Circle> circles = { *this, circle };

    std::for_each(circles.begin(), circles.end(), [](Circle circle) {
      auto begin = std::remove(interPoints.begin(), interPoints.end(),
        [&circle](Point point) {
          return !circle.hasPoint(point.x, point.y);
        }
      );

      interPoints.erase(begin, interPoints.end());
    });

    if (interPoints.size) {
      return Nullable<std::vector<Point>>(interPoints);
    }

    return Nullable<std::vector<Point>>();
  }

  // circle - line intersection method
  Nullable<std::vector<Point>> getIntersection(Line line) {
    double x1 = line.x1 - _x;
    double x2 = line.x2 - _x;
    double y1 = line.y1 - _y;
    double y2 = line.y2 - _y;
    double dx = x2 - x1;
    double dy = y2 - y1;
    double d = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    double h = (x1 * y2) - (x2 * y1);
    double delta = (std::pow(_r, 2) * std::pow(d, 2)) - std::pow(h, 2);

    if (delta < 0) Nullable<std::vector<Point>>();

    std::vector<Point> interPoints = {
      {
        (((h * dy) + (((dy / std::abs(dy)) || 1) * dx * std::sqrt(delta))) / std::pow(d, 2)) + _x,
        (((-h * dx) + (std::abs(dy) * std::sqrt(delta))) / std::pow(d, 2)) + _y
      },
      {
        (((h * dy) - (((dy / std::abs(dy)) || 1) * dx * std::sqrt(delta))) / std::pow(d, 2)) + _x,
        (((-h * dx) - (std::abs(dy) * std::sqrt(delta))) / std::pow(d, 2)) + _y
      }
    };

    std::transform(interPoints.begin(), interPoints.end(), interPoints.begin(),
      [](std::vector<Point> point) {
        return { utils::trim(point.x, 9), utils::trim(point.y, 9) }
      }
    );

    auto begin = std::remove(interPoints.begin(), interPoints.end(),
      [&line](Point point) {
        return !line.boundsHavePoint(point.x, point.y);
      }
    );

    interPoints.erase(begin, interPoints.end());

    auto end = std::unique(interPoints.begin(), interPoints.end(),
      [](std::vector<Point> pointA, std::vector<Point> pointB) {
        return pointA.x == pointB.x && pointA.y == pointB.y;
      }
    );

    interPoints.erase(interPoints.begin(), end);

    if (interPoints.size) {
      return Nullable<std::vector<Point>>(interPoints);
    }

    return Nullable<std::vector<Point>>();
  }
}