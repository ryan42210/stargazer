#include "coordinate.h"
#include "math_util.h"

#include <cmath>
#include <map>
#include <algorithm>
#include <iostream>

const std::vector<SpatialStarEntry> &CoordMatch::getNaviStarList() const {
  return navi_star_list;
}
const std::vector<DistTriangle> &CoordMatch::getTriangleList() const {
  return triangle_list;
}

void CoordMatch::initialize(const std::vector<SpatialStarEntry> &star_list, const Config &config) {
  initFromConfig(config);
  setNaviStarList(star_list);
  buildTriangleLinks(this->navi_star_list, this->triangle_list);
  std::cout << "Build " << triangle_list.size() << " triangles for matching..." << std::endl;
  std::cout << "Takes " << triangle_list.size() * sizeof(DistTriangle) / 1024 << "KB." << std::endl;
}

bool CoordMatch::match(std::vector<ImgStarEntry> &stars_in_view, Attitude &out_attitude) {
  // step 1: convert img coordinate to local Cartesian coordinate.
  std::vector<SpatialStarEntry> local_stars_pos;
  for (auto &star: stars_in_view) {
    auto normalized_img_coord = imgPxCoordToImgNormalCoord(star.x, star.y);
    auto local_coord = imgNormalCoordToCamLocalCoord(normalized_img_coord.x(), normalized_img_coord.y());
    SpatialStarEntry local_star{local_coord.x(), local_coord.y(), local_coord.z(), star.magnitude};
    if (star.magnitude <= mag_threshold) {
      local_stars_pos.push_back(local_star);
    }
  }
  // step 2: compute dist in local triangle;
  std::vector<DistTriangle> local_triangles;
  buildTriangleLinks(local_stars_pos, local_triangles);

  // step 3: find corresponding (d1, d2, d3) tuple in stored triangle_list.
  // build a map from star (idx) in local coord to star (idx) in navi_star_list;
  std::map<int, int> result;
  for (auto &t_l: local_triangles) {
    Vec3i idx_in_navi_star_list;
    for (auto &t_w: triangle_list) {
      if (distTupleEqual(t_l, t_w, idx_in_navi_star_list)) {
        // TODO: may need to fix conflict match
        result[t_l.idx1] = idx_in_navi_star_list[0];
        result[t_l.idx2] = idx_in_navi_star_list[1];
        result[t_l.idx3] = idx_in_navi_star_list[2];
        break;
      }
    }
  }
  if (result.size() < 3) {
    return false;
  }

  // step 4: solve attitude from map
  // build liner system Ax = b.
  MatXf A = Eigen::MatrixXf::Zero(result.size(), 3);
  Vecf b = Eigen::VectorXf::Zero(result.size());
  int i = 0;
  // for x
  i = 0;
  for (auto item: result) {
    A(i, 0) = local_stars_pos[item.first].x;
    A(i, 1) = local_stars_pos[item.first].y;
    A(i, 2) = local_stars_pos[item.first].z;
    b[i] = local_stars_pos[item.second].x;
    i++;
  }
  auto r1 = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
  // for y
  i = 0;
  for (auto item: result) {
    A(i, 0) = local_stars_pos[item.first].x;
    A(i, 1) = local_stars_pos[item.first].y;
    A(i, 2) = local_stars_pos[item.first].z;
    b[i] = local_stars_pos[item.second].y;
    i++;
  }
  auto r2 = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
  // for z
  i = 0;
  for (auto item: result) {
    A(i, 0) = local_stars_pos[item.first].x;
    A(i, 1) = local_stars_pos[item.first].y;
    A(i, 2) = local_stars_pos[item.first].z;
    b[i] = local_stars_pos[item.second].z;
    i++;
  }
  auto r3 = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

  out_attitude.forward = -Vec3f(r1.z(), r2.z(), r3.z());
  out_attitude.up = Vec3f(r1.y(), r2.y(), r3.y());
  out_attitude.right = Vec3f(r1.x(), r2.x(), r3.x());
  return true;
}

void CoordMatch::setNaviStarList(const std::vector<SpatialStarEntry> &star_list) {
  navi_star_list = star_list;
}

Vec2f CoordMatch::imgPxCoordToImgNormalCoord(float dx, float dy) const {
  return {dx / static_cast<float>(resolution.x()), dy / static_cast<float>(resolution.y())};
}

// local coordinate: origin at (0, 0, 0), forward: (0, 0, -1), up: (0, 1, 0), right: (1, 0, 0)
Vec3f CoordMatch::imgNormalCoordToCamLocalCoord(float dx, float dy) const {
  float x_offset = (2 * dx - 1) / std::tanf(degToRad(fov_x / 2.f));
  float y_offset = (2 * dy - 1) / std::tanf(degToRad(fov_y / 2.f));
  Vec3f direction = x_offset * Vec3f(1, 0, 0) + y_offset * Vec3f(0, 1, 0) + Vec3f(0, 0, -1);
  return direction.normalized();
}

void CoordMatch::buildTriangleLinks(std::vector<SpatialStarEntry> &star_list,
                                    std::vector<DistTriangle> &triangle_dist_list) const {
  // TODO: sort by magnitude
  std::sort(star_list.begin(), star_list.end(), [](SpatialStarEntry a, SpatialStarEntry b) {
    return a.magnitude < b.magnitude;
  });

  if (!triangle_dist_list.empty()) {
    triangle_dist_list.clear();
  }
#pragma omp parallel for
  for (int i = 0; i < star_list.size() - 2; i++) {
    for (int j = i + 1; j < star_list.size() - 1; j++) {
      for (int k = j + 1; k < star_list.size(); k++) {
        auto &a = star_list[i];
        auto &b = star_list[j];
        auto &c = star_list[k];
        float dist1 = (Vec3f(a.x, a.y, a.z) - Vec3f(b.x, b.y, b.z)).norm();
        float dist2 = (Vec3f(a.x, a.y, a.z) - Vec3f(c.x, c.y, c.z)).norm();
        float dist3 = (Vec3f(b.x, b.y, b.z) - Vec3f(c.x, c.y, c.z)).norm();
        if (dist1 < dist_threshold && dist2 < dist_threshold && dist3 < dist_threshold) {
#pragma omp critical
          triangle_dist_list.push_back(DistTriangle{i, j, k, dist1, dist2, dist3});
        }
      }
    }
  }
}

void CoordMatch::initFromConfig(const Config &config) {
  fov_x = config.fov_x;
  fov_y = config.fov_y;
  resolution.x() = config.resolution_x;
  resolution.y() = config.resolution_y;
  mag_threshold = config.mag_threshold;
  // TODO: set correct mag threshold;
  // mag_threshold = 6.0f;
  initDistThreshold();
}

void CoordMatch::initDistThreshold() {
  auto low = sphereToCartesian(0, 0);
  auto hi = sphereToCartesian(fov_x, fov_y);
  dist_threshold = (low - hi).norm();
}

// Compare dist triangle k and u.
// u is unknown triangle from img. k is known triangle from navi star list
// out_idx: idx of unknown star in navi_star_list, (u1, u2, u3)
bool CoordMatch::distTupleEqual(const DistTriangle &k, const DistTriangle &u, Vec3i &out_idx) {
  if (distEqual(k.d1, u.d1), distEqual(k.d2, u.d2), distEqual(k.d3, u.d3)) {
    out_idx = Vec3i(k.idx1, k.idx2, k.idx3);
    return true;
  }
  if (distEqual(k.d1, u.d1), distEqual(k.d2, u.d3), distEqual(k.d3, u.d2)) {
    out_idx = Vec3i(k.idx1, k.idx3, k.idx2);
    return true;
  }

  if (distEqual(k.d1, u.d2), distEqual(k.d2, u.d3), distEqual(k.d3, u.d1)) {
    out_idx = Vec3i(k.idx3, k.idx1, k.idx2);
    return true;
  }
  if (distEqual(k.d1, u.d2), distEqual(k.d2, u.d1), distEqual(k.d3, u.d3)) {
    out_idx = Vec3i(k.idx2, k.idx1, k.idx3);
    return true;
  }

  if (distEqual(k.d1, u.d3), distEqual(k.d2, u.d2), distEqual(k.d3, u.d1)) {
    out_idx = Vec3i(k.idx3, k.idx2, k.idx1);
    return true;
  }
  if (distEqual(k.d1, u.d3), distEqual(k.d2, u.d1), distEqual(k.d3, u.d2)) {
    out_idx = Vec3i(k.idx2, k.idx3, k.idx2);
    return true;
  }
  return false;
}