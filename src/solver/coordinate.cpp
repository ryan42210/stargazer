#include "coordinate.h"
#include "math_util.h"

#include <cmath>
#include <map>
#include <algorithm>
#include <iostream>
#include <utility>

const std::vector<SpatialStarEntry> &CoordMatch::getNaviStarList() const {
  return navi_star_list;
}
const std::vector<Triangle> &CoordMatch::getTriangleList() const {
  return triangle_list;
}

void CoordMatch::initialize(const std::vector<SpatialStarEntry> &star_list, const Config &config) {
  initFromConfig(config);
  setNaviStarList(star_list);
  buildTriangleLinksForNaviStars();
  std::cout << "Build " << triangle_list.size() << " triangles for matching..." << std::endl;
  std::cout << "Takes " << triangle_list.size() * sizeof(TriangleEdge) * 3 / 1024 << "KB." << std::endl;
}

bool CoordMatch::match(std::vector<ImgStarEntry> &stars_in_view, Attitude &out_attitude) {
  // step 1: convert img coordinate to local Cartesian coordinate.
  std::vector<SpatialStarEntry> local_stars_pos;
  for (auto &star: stars_in_view) {
    auto normalized_img_coord = imgPxCoordToImgNormalCoord(star.x, star.y);
    auto local_coord = imgNormalCoordToCamLocalCoord(normalized_img_coord.x(), normalized_img_coord.y());
    // std::cout << "recoverd local coord (" << local_coord.x()
    //           << ", " << local_coord.y()
    //           << ", " << local_coord.z()
    //           << ")" << std::endl;
    SpatialStarEntry local_star{local_coord.x(), local_coord.y(), local_coord.z(), star.magnitude};
    if (star.magnitude <= mag_threshold) {
      local_stars_pos.push_back(local_star);
    }
  }
  // step 2: compute dist in local triangle;
  std::vector<Triangle> local_triangles;
  buildTriangleLinksForImgStars(local_stars_pos, local_triangles);

  // step 3: find corresponding (d1, d2, d3) tuple in stored triangle_list.
  // build a map from star (idx) in local coord to star (idx) in navi_star_list;
  std::map<int, int> result;
  for (auto &t_l: local_triangles) {
    for (auto &t_w: triangle_list) {
      if (TriangleEqual(t_l, t_w)) {
        result[t_l[0].idx] = t_w[0].idx;
        result[t_l[1].idx] = t_w[1].idx;
        result[t_l[2].idx] = t_w[2].idx;
      }
    }
  }
  // for (auto p : result) {
  //   std::cout << p.first << "->" << p.second << std::endl;
  // }
  if (result.size() < 3) {
    std::cerr << "No enough matching of stars..." << std::endl;
    return false;
  }

  // for (auto &item: result) {
  //   std::cout << "Star by local coord (" << local_stars_pos[item.first].x
  //             << ", " << local_stars_pos[item.first].y
  //             << ", " << local_stars_pos[item.first].z << ")" << std::endl;
  // }
  //
  // for (auto &item: result) {
  //   std::cout << "Star by world coord (" << navi_star_list[item.second].x
  //             << ", " << navi_star_list[item.second].y
  //             << ", " << navi_star_list[item.second].z << ")" << std::endl;
  // }

  // step 4: solve attitude from map
  // Kabsch-Umeyama algorithm
  MatXf M_V = Eigen::MatrixXf::Zero(result.size(), 3);
  MatXf M_W = Eigen::MatrixXf::Zero(result.size(), 3);
  int i = 0;
  for (const auto &pair : result) {
    M_V(i, 0) = local_stars_pos[pair.first].x;
    M_V(i, 1) = local_stars_pos[pair.first].y;
    M_V(i, 2) = local_stars_pos[pair.first].z;

    M_W(i, 0) = navi_star_list[pair.second].x;
    M_W(i, 1) = navi_star_list[pair.second].y;
    M_W(i, 2) = navi_star_list[pair.second].z;
    i++;
  }

  MatXf H = M_V.transpose() * M_W;

  auto svd_res = H.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
  MatXf U = svd_res.matrixU();
  MatXf V = svd_res.matrixV();
  int d = 1;
  if ((V * U.transpose()).determinant() < 0) d = -1;
  MatXf I = MatXf::Identity(3,3);
  I(2,2) = d;
  MatXf R_l2w = V * I * U.transpose();

  out_attitude.forward = -Vec3f(R_l2w(0, 2), R_l2w(1, 2), R_l2w(2, 2));
  out_attitude.up = Vec3f(R_l2w(0, 1), R_l2w(1, 1), R_l2w(2, 1));
  out_attitude.right = Vec3f(R_l2w(0, 0), R_l2w(1, 0), R_l2w(2, 0));
  return true;
}

void CoordMatch::setNaviStarList(const std::vector<SpatialStarEntry> &star_list) {
  navi_star_list = star_list;
  // std::cout << "----------------\n";
  // for (auto star : navi_star_list) {
  //   std::cout << "(" << star.x << ", " << star.y << ", " << star.z << ")" << std::endl;
  // }
  // std::cout << "----------------" << std::endl;
}

Vec2f CoordMatch::imgPxCoordToImgNormalCoord(float dx, float dy) const {
  return {dx / static_cast<float>(resolution.x()), dy / static_cast<float>(resolution.y())};
}

// local coordinate: origin at (0, 0, 0), forward: (0, 0, -1), up: (0, 1, 0), right: (1, 0, 0)
Vec3f CoordMatch::imgNormalCoordToCamLocalCoord(float dx, float dy) const {
  float x_offset = (2 * dx - 1) * std::tanf(degToRad(fov_x / 2.f));
  float y_offset = (2 * dy - 1) * std::tanf(degToRad(fov_y / 2.f));
  Vec3f direction = x_offset * Vec3f(1, 0, 0) + y_offset * Vec3f(0, 1, 0) + Vec3f(0, 0, -1);
  return direction.normalized();
}

void CoordMatch::buildTriangleLinksForNaviStars() {
  // TODO: sort by magnitude
  std::sort(navi_star_list.begin(), navi_star_list.end(), [](SpatialStarEntry a, SpatialStarEntry b) {
    return a.magnitude < b.magnitude;
  });

  if (!triangle_list.empty()) {
    triangle_list.clear();
  }
// #pragma omp parallel for
  for (int i = 0; i < navi_star_list.size() - 2; i++) {
    const auto &a = navi_star_list[i];

    for (int j = i + 1; j < navi_star_list.size() - 1; j++) {
      const auto &b = navi_star_list[j];
      float dist_ij = (Vec3f(a.x, a.y, a.z) - Vec3f(b.x, b.y, b.z)).norm();
      if (dist_ij >= dist_threshold) continue;

      for (int k = j + 1; k < navi_star_list.size(); k++) {
        const auto &c = navi_star_list[k];
        float dist_ik = (Vec3f(a.x, a.y, a.z) - Vec3f(c.x, c.y, c.z)).norm();
        float dist_jk = (Vec3f(b.x, b.y, b.z) - Vec3f(c.x, c.y, c.z)).norm();
        if (dist_ik > dist_threshold || dist_jk > dist_threshold) continue;
        if (distEqual(dist_ij, dist_ik) || distEqual(dist_ij, dist_jk) || distEqual(dist_jk, dist_ik)) continue;

        Triangle t;
        t.push_back(TriangleEdge{i, a.magnitude, dist_jk});
        t.push_back(TriangleEdge{j, b.magnitude, dist_ik});
        t.push_back(TriangleEdge{k, c.magnitude, dist_ij});
        std::sort(t.begin(), t.end(), [](TriangleEdge e1, TriangleEdge e2) {
          // if (abs(e1.mag - e2.mag) < EPS) return e1.d > e2.d;
          // return e1.mag < e2.mag;
          return e1.d > e2.d;
        });
// #pragma omp critical
        triangle_list.push_back(t);
      }
    }
  }
  std::cout << "Build " << triangle_list.size() << " triangles for navi stars." << std::endl;
  // for (auto tri : triangle_list) {
  //   std::cout << "d1 = " << tri[0].d << ", d2 = " << tri[1].d << ", d3 = " << tri[2].d << std::endl;
  // }
}

void CoordMatch::buildTriangleLinksForImgStars(std::vector<SpatialStarEntry> &star_list,
                                               std::vector<Triangle> &triangle_dist_list) {
  std::sort(star_list.begin(), star_list.end(), [](SpatialStarEntry a, SpatialStarEntry b) {
    return a.magnitude < b.magnitude;
  });

  if (!triangle_dist_list.empty()) {
    triangle_dist_list.clear();
  }
  int img_star_num = std::min(static_cast<int>(star_list.size()), 10);
  if (img_star_num < 3) {
    std::cerr << "No enough stars in img. exit." << std::endl;
    exit(-1);
  }
  for (int i = 0; i < img_star_num - 2; i++) {
    for (int j = i + 1; j < img_star_num - 1; j++) {
      for (int k = j + 1; k < img_star_num; k++) {
        auto &a = star_list[i];
        auto &b = star_list[j];
        auto &c = star_list[k];
        float dist1 = (Vec3f(a.x, a.y, a.z) - Vec3f(b.x, b.y, b.z)).norm();
        float dist2 = (Vec3f(a.x, a.y, a.z) - Vec3f(c.x, c.y, c.z)).norm();
        float dist3 = (Vec3f(b.x, b.y, b.z) - Vec3f(c.x, c.y, c.z)).norm();
        Triangle t;
        t.push_back(TriangleEdge{i, a.magnitude, dist3});
        t.push_back(TriangleEdge{j, b.magnitude, dist2});
        t.push_back(TriangleEdge{k, c.magnitude, dist1});
        std::sort(t.begin(), t.end(), [](TriangleEdge e1, TriangleEdge e2) {
          // if (abs(e1.mag - e2.mag) < EPS) return e1.d > e2.d;
          // return e1.mag < e2.mag;
          return e1.d > e2.d;
        });
        triangle_dist_list.push_back(t);
      }
    }
  }
  // std::cout << "Build " << triangle_dist_list.size() << " triangles for stars in img." << std::endl;
  // for (auto tri : triangle_dist_list) {
  //   std::cout << "d1 = " << tri[0].d << ", d2 = " << tri[1].d << ", d3 = " << tri[2].d << std::endl;
  // }
}

void CoordMatch::initFromConfig(const Config &config) {
  fov_x = config.fov_x;
  fov_y = config.fov_y;
  resolution.x() = config.resolution_x;
  resolution.y() = config.resolution_y;
  mag_threshold = config.mag_threshold;
  initDistThreshold();
}

void CoordMatch::initDistThreshold() {
  auto low = sphereToCartesian(0, 0);
  auto hi = sphereToCartesian(fov_x, fov_y);
  dist_threshold = (low - hi).norm();
}

// Compare dist triangle a and b.
bool CoordMatch::TriangleEqual(const Triangle &a, const Triangle &b) {
  if (a.size() != 3 || b.size() != 3) return false;
  if (distEqual(a[0].d, b[0].d) && distEqual(a[1].d, b[1].d) && distEqual(a[2].d, b[2].d)) {
    return true;
  }
  return false;
}
