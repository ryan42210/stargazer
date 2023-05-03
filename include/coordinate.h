#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "core.h"
#include "math_util.h"
#include "catalogue.h"
#include "config.h"

#include <vector>

struct DistTriangle {
  int idx1, idx2, idx3;
  float d1, d2, d3;
};

class CoordMatch {
 public:
  Vec2f imgPxCoordToImgNormalCoord(float dx, float dy) const;
  // local coordinate basis: forward: (0, 0 ,-1), up: (0, 1, 0)
  Vec3f imgNormalCoordToCamLocalCoord(float dx, float dy) const;
  void filterNaviStars(const std::vector<CatEntry>& catalogue);
  void buildTriangleLinks();


 private:
  float dist_threshold;
  float mag_threshold;

  float fov_x;
  float fov_y;
  Vec2i resolution;

  std::vector<NaviStarEntry> stars_in_view;
  std::vector<NaviStarEntry> nav_star_list;
  std::vector<DistTriangle> triangle_list;
};

#endif