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
#pragma omp parallel for
  for (int i = 0; i < short_catalogue.size() - 1; i++) {
    auto &a = short_catalogue[i];
    auto new_pos = sphereToCartesian(a.right_ascension, a.declination);
    float new_mag = a.mag;

    for (int j = i + 1; j < short_catalogue.size(); j++) {
      auto &b = short_catalogue[j];
      auto pos_b = sphereToCartesian(b.right_ascension, b.declination);
      if ((new_pos - pos_b).norm() < dist_lower_bound) {
        // TODO: implement merge method for two stars that are too close
        new_pos.x() += pos_b.x();
        new_pos.y() += pos_b.y();
        new_pos.z() += pos_b.z();
        new_pos.x() *= 0.5f;
        new_pos.y() *= 0.5f;
        new_pos.z() *= 0.5f;
        new_mag = std::min(new_mag, b.mag);
      }
    }
#pragma omp critical
    navi_star_list.push_back(SpatialStarEntry{new_pos.x(), new_pos.y(), new_pos.z(), new_mag});
  }

  std::sort(navi_star_list.begin(), navi_star_list.end(), [](SpatialStarEntry a, SpatialStarEntry b) {
    return a.magnitude < b.magnitude;
  });
  return navi_star_list;
}
