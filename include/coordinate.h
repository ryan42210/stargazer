#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "core.h"
#include "math_util.h"
#include "catalogue.h"
#include "config.h"

#include <vector>

struct Attitude {
  Vec3f forward;
  Vec3f up;
  Vec3f right;
};

struct DistTriangle {
  int idx1, idx2, idx3;
  float d1, d2, d3;
};

class CoordMatch {
 public:
  [[nodiscard]] const std::vector<NaviStarEntry> &getNaviStarList() const;
  [[nodiscard]] const std::vector<DistTriangle> &getTriangleList() const;

  void initialize(std::vector<NaviStarEntry>& star_list, Config& config);
  bool match(std::vector<ImgStarPoint> &stars_in_view, Attitude& out_attitude);
 private:
  // dx, dy are scaled from [0, resolution] to [0, 1]
  [[nodiscard]] Vec2f imgPxCoordToImgNormalCoord(float dx, float dy) const;
  // local coordinate basis: forward: (0, 0 ,-1), up: (0, 1, 0)
  [[nodiscard]] Vec3f imgNormalCoordToCamLocalCoord(float dx, float dy) const;
  void setNaviStarList(std::vector<NaviStarEntry>& star_list);
  void buildTriangleLinks(std::vector<DistTriangle>& triangle_dist_list);
  void initDistThreshold();
  void initMagThreshold();
  static bool distTupleEqual(const DistTriangle& k, const DistTriangle& u, Vec3i& out_idx);


  float dist_threshold;
  float mag_threshold;

  float fov_x;
  float fov_y;
  Vec2i resolution;

  std::vector<NaviStarEntry> navi_star_list;
  std::vector<DistTriangle> triangle_list;
};

#endif