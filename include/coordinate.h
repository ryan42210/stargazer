#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "core.h"
#include "math_util.h"
#include "catalogue.h"

#include <vector>

class LocalCoord {
 public:
  void imgPxCoordToImgNormalCoord();
  void imgNormalCoordToCamLocalCoord();
 private:
  float f;
  float fov_x;
  float fov_y;
  Vec2i resolution;
  std::vector<CatShortEntry> stars_in_view;
};

class CelestialCoord {
 public:
 private:
  std::vector<CatShortEntry> nav_star_list;
};

#endif