#include "navi_star_filter.h"
#include "math_util.h"

const float mag_threshold = 6.0f;
// TODO: correctly set lower bound of dist by resolution an field of view
const float dist_lower_bound = 1e-4f;

float computeMagForMatch(float B_mag, float T_mag) {
  // TODO: find a proper way to compute mag for CCD/CMOS sensor.
  return T_mag;
}
std::vector<NaviStarEntry> filterNaviStars(const std::vector<CatEntry> &catalogue) {
  std::vector<CatEntry> short_catalogue;
  for (const auto &star: catalogue) {
    // TODO: add more condition to filtering
    auto eqv_mag = computeMagForMatch(star.B_magnitude, star.T_magnitude);
    if (eqv_mag <= mag_threshold) {
      short_catalogue.push_back(star);
      short_catalogue.rbegin()->mag = eqv_mag;
    }
  }

  std::vector<NaviStarEntry> navi_star_list;
  // merge stars that are too close
  for (int i = 0; i < short_catalogue.size() - 1; i++) {
    for (int j = i + 1; j < short_catalogue.size(); j++) {
      auto &a = short_catalogue[i];
      auto &b = short_catalogue[j];
      auto pos_a = sphereToCartesian(a.right_ascension, a.declination);
      auto pos_b = sphereToCartesian(b.right_ascension, b.declination);
      if ((pos_a - pos_b).norm() > dist_lower_bound) {
        navi_star_list.push_back(NaviStarEntry{pos_a.x(), pos_a.y(), pos_a.z(), a.mag});
        navi_star_list.push_back(NaviStarEntry{pos_b.x(), pos_b.y(), pos_b.z(), b.mag});
      } else {
        // TODO: implement merge method for two stars that are too close
        float m_x = (pos_a.x() + pos_b.x()) * 0.5f;
        float m_y = (pos_a.y() + pos_b.y()) * 0.5f;
        float m_z = (pos_a.z() + pos_b.z()) * 0.5f;
        float m_mag = std::min(a.mag, b.mag);
        navi_star_list.push_back(NaviStarEntry{m_x, m_y, m_z, m_mag});
      }
    }
  }
  return navi_star_list;
}
