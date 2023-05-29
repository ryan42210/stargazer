#include "image_generator.h"
#include "core.h"
#include "math_util.h"
#include "navi_star_filter.h"

#include <random>
#include <fstream>

void ImageGenerator::initialize(const Config &cfg, const std::vector<CatEntry> &cata) {
  fov_x = cfg.fov_x;
  fov_y = cfg.fov_y;
  resolution_x = cfg.resolution_x;
  resolution_y = cfg.resolution_y;
  mag_threshold = cfg.mag_threshold;
  catalogue = cata;
}

std::vector<ImgStarEntry> ImageGenerator::genImg(const std::optional<std::string>& filepath) {
  std::random_device r;
  std::default_random_engine e(r());
  std::uniform_real_distribution<float> dis(0, 1);
  // step 1: generate a random attitude.
  Vec3f forward = sphereToCartesian(dis(e) * 360, dis(e) * 180 - 90);
  Vec3f up = sphereToCartesian(dis(e) * 360, dis(e) * 180 - 90);
  Vec3f right = forward.cross(up);
  // step 2: set rotation matrix from attitude.
  Mat3f rot_w2l;
  rot_w2l(0, 0) = right.x();
  rot_w2l(0, 1) = right.y();
  rot_w2l(0, 2) = right.z();

  rot_w2l(1, 0) = up.x();
  rot_w2l(1, 1) = up.y();
  rot_w2l(1, 2) = up.z();

  rot_w2l(2, 0) = -forward.x();
  rot_w2l(2, 1) = -forward.y();
  rot_w2l(2, 2) = -forward.z();

  // step 3: scan catalogue with camera parameter and pick stars.
  std::vector<SpatialStarEntry> visible_stars_in_local;
  for (const auto& star : catalogue) {
    float mag = computeMagForMatch(star.B_magnitude, star.T_magnitude);
    if (mag_threshold >= mag) {
      auto world_pos = sphereToCartesian(star.right_ascension, star.declination);
      auto local_pos = rot_w2l * world_pos;
      visible_stars_in_local.push_back(SpatialStarEntry{local_pos.x(), local_pos.y(), local_pos.z(), mag});
    }
  }
  std::vector<SpatialStarEntry> stars_in_img_spatial_coord;
  auto low = sphereToCartesian(-fov_x/2, -fov_y/2);
  auto hi = sphereToCartesian(fov_x/2, fov_y/2);
  for (auto &star : visible_stars_in_local) {
    bool not_in_view = false;
    if (star.x < low.x() || star.y < low.y() || star.z < low.z()) not_in_view = true;
    if (star.x > hi.x() || star.y > hi.y() || star.z > hi.z()) not_in_view = true;
    if (!not_in_view) stars_in_img_spatial_coord.push_back(star);
  }
  // step 4: project selected stars on img plane.
  std::vector<ImgStarEntry> star_point_in_img;
  for (auto & star : stars_in_img_spatial_coord) {
    float x_normalized = (star.x / star.z / std::tanf(degToRad(fov_x / 2.f)) + 1) / 2;
    float y_normalized = (star.y / star.z / std::tanf(degToRad(fov_y / 2.f)) + 1) / 2;
    float coord_img_x = x_normalized * static_cast<float>(resolution_x);
    float coord_img_y = y_normalized * static_cast<float>(resolution_y);
    star_point_in_img.push_back(ImgStarEntry{coord_img_x, coord_img_y, star.magnitude});
  }
  // step 5: add random noise.
  // TODO

  // step 6: write to file
  if (filepath.has_value()) {
    std::ofstream fout(filepath.value());
    if (!fout.is_open()) {
      exit(-1);
    }
    fout << fov_y << ',' << fov_y << ',' << mag_threshold << std::endl;
    fout << resolution_x << ',' << resolution_y << std::endl;
    for (const auto &star : star_point_in_img) {
      fout << star.x << ',' << star.y << ',' << star.magnitude << std::endl;
    }
  }
  return star_point_in_img;
}
