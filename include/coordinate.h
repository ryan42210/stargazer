#ifndef COORDINATE_H_
#define COORDINATE_H_

#include "core.h"
#include "catalogue.h"
#include "config.h"

#include <vector>

struct Attitude {
  Vec3f forward;
  Vec3f up;
  Vec3f right;
};

struct TriangleEdge {
  int idx;
  float mag;
  float d;
};

using Triangle = std::vector<TriangleEdge>;

class CoordMatch {
 public:
  [[nodiscard]] const std::vector<SpatialStarEntry> &getNaviStarList() const;
  [[nodiscard]] const std::vector<Triangle> &getTriangleList() const;

  void initialize(const std::vector<SpatialStarEntry> &star_list, const Config &config);
  bool match(std::vector<ImgStarEntry> &stars_in_view, Attitude &out_attitude);
 private:
  // dx, dy are scaled from [0, resolution] to [0, 1]
  [[nodiscard]] Vec2f imgPxCoordToImgNormalCoord(float dx, float dy) const;
  // local coordinate basis: forward: (0, 0 ,-1), up: (0, 1, 0)
  [[nodiscard]] Vec3f imgNormalCoordToCamLocalCoord(float dx, float dy) const;
  void setNaviStarList(const std::vector<SpatialStarEntry> &star_list);
  void buildTriangleLinksForNaviStars();
  static void buildTriangleLinksForImgStars(std::vector<SpatialStarEntry> &star_list,
                                     std::vector<Triangle> &triangle_dist_list) ;
  void initFromConfig(const Config &config);
  void initDistThreshold();
  static bool TriangleEqual(const Triangle &k, const Triangle &u);

  float dist_threshold;
  float mag_threshold;

  float fov_x;
  float fov_y;
  Vec2i resolution;

  std::vector<SpatialStarEntry> navi_star_list;
  std::vector<Triangle> triangle_list;
};

#endif