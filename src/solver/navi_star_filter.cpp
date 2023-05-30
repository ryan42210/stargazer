#include "navi_star_filter.h"
#include "math_util.h"

#include <algorithm>
#include <iostream>
#include <omp.h>

// TODO: correctly set lower bound of dist by resolution an field of view

float computeMagForMatch(float B_mag, float T_mag) {
  // TODO: find a proper way to compute mag for CCD/CMOS sensor.
  return B_mag;
}
std::vector<SpatialStarEntry> filterNaviStars(const Config &cfg, const std::vector<CatEntry> &catalogue) {
  std::vector<CatEntry> short_catalogue;
  for (const auto &star: catalogue) {
    // TODO: add more condition to filtering
    auto eqv_mag = computeMagForMatch(star.B_magnitude, star.T_magnitude);
    if (eqv_mag <= cfg.mag_threshold) {
      short_catalogue.push_back(star);
      short_catalogue.rbegin()->mag = eqv_mag;
    }
  }

  std::vector<SpatialStarEntry> navi_star_list;
  // merge stars that are too close

  float dx = cfg.fov_x / static_cast<float>(cfg.resolution_x);
  float dy = cfg.fov_y / static_cast<float>(cfg.resolution_y);
  float dist_lower_bound = (sphereToCartesian(0, 0) - sphereToCartesian(dx, dy)).norm();
  for (auto & a : short_catalogue) {
    auto pos = sphereToCartesian(a.right_ascension, a.declination);
    float mag = a.mag;
    bool skip = false;
    // for (auto & b : navi_star_list) {
    //   Vec3f pos_b(b.x, b.y, b.z);
    //   if ((pos - pos_b).norm() < dist_lower_bound) {
    //     b.x = (pos.x() + pos_b.x()) * 0.5f;
    //     b.y = (pos.y() + pos_b.y()) * 0.5f;
    //     b.z = (pos.z() + pos_b.z()) * 0.5f;
    //     b.magnitude = std::min(mag, b.magnitude);
    //     skip = true;
    //     break;
    //   }
    // }
    for (auto & b : navi_star_list) {
      Vec3f pos_b(b.x, b.y, b.z);
      if ((pos - pos_b).norm() < dist_lower_bound) {
        b.magnitude = std::min(mag, b.magnitude);
        skip = true;
        break;
      }
    }
    if (!skip) {
      navi_star_list.push_back(SpatialStarEntry{pos.x(), pos.y(), pos.z(), mag});
    }
  }

  std::sort(navi_star_list.begin(), navi_star_list.end(), [](SpatialStarEntry a, SpatialStarEntry b) {
    return a.magnitude < b.magnitude;
  });
  return navi_star_list;
}
