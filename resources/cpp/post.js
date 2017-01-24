return {
  Utils: {
    mod: Module.utils_mod,
    trim: Module.utils_trim,
    isBetween: Module.utils_isBetween,
    compare: Module.utils_compare
  },

  Geometry: {
    Line: {
      shape: Module.geometry_line_shape,
      getX: Module.geometry_line_getX,
      getY: Module.geometry_line_getY,
      hasPoint: Module.geometry_line_hasPoint,
      boundsHavePoint: Module.geometry_line_boundsHavePoint,
      getLineIntersection: Module.geometry_line_getLineIntersection
    }
  }
};

})();