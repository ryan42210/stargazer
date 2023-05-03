#include "coordinate.h"

#include <cmath>

Vec2f CoordMatch::imgPxCoordToImgNormalCoord(float dx, float dy) const {
  return {dx / static_cast<float>(resolution.x()), dy / static_cast<float>(resolution.y())};
}

Vec3f CoordMatch::imgNormalCoordToCamLocalCoord(float dx, float dy) const {
  float x_offset = (2 * dx - 1)  / std::tanf(degToRad(fov_x / 2.f));
  float y_offset = (2 * dy - 1)  / std::tanf(degToRad(fov_y / 2.f));
  Vec3f direction = x_offset * Vec3f(1 ,0 ,0) + y_offset * Vec3f(0, 1, 0) + Vec3f(0, 0, -1);
  return direction.normalized();
}
void CoordMatch::filterNaviStars(const std::vector<CatEntry> &catalogue) {

}
void CoordMatch::buildTriangleLinks() {
  if (!triangle_list.empty()) {
    triangle_list.clear();
  }

  for (int i = 0; i < nav_star_list.size() - 2; i++) {
    for (int j = i + 1; j < nav_star_list.size() - 1; j++) {
      for (int k = j + 1; k < nav_star_list.size(); k++) {
        float dist1 = computeDistOnUnitSphere(nav_star_list[i].pos, nav_star_list[j].pos);
        float dist2 = computeDistOnUnitSphere(nav_star_list[i].pos, nav_star_list[k].pos);
        float dist3 = computeDistOnUnitSphere(nav_star_list[j].pos, nav_star_list[k].pos);
        if (dist1 < dist_threshold && dist2 < dist_threshold && dist3 < dist_threshold) {
          triangle_list.push_back(DistTriangle{i, j, k, dist1, dist2, dist3});
        }
      }
    }
  }
}
