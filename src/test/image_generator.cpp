#include "image_generator.h"
#include "core.h"
#include "math_util.h"
#include "navi_star_filter.h"

#include <random>
#include <fstream>
#include <iostream>

void ImageGenerator::initialize(const Config &cfg, const std::vector<CatEntry> &cata) {
  fov_x = cfg.fov_x;
  fov_y = cfg.fov_y;
  resolution_x = cfg.resolution_x;
  resolution_y = cfg.resolution_y;
  mag_threshold = cfg.mag_threshold;
  catalogue = cata;
}

std::vector<ImgStarEntry> ImageGenerator::genImg(const std::optional<std::string> &filepath, Attitude &gen_attitude) {
  std::random_device r;
  std::default_random_engine e(r());
  std::uniform_real_distribution<float> dis(0, 1);
  // step 1: generate a random attitude.
  Vec3f forward = sphereToCartesian(dis(e) * 360, dis(e) * 180 - 90);
  Vec3f ref_up = sphereToCartesian(dis(e) * 360, dis(e) * 180 - 90);
  // Vec3f forward = Vec3f(0, 0, 1);
  Vec3f up = Vec3f(0, 1, 0);
  Vec3f right = forward.cross(ref_up).normalized();
  up = right.cross(forward).normalized();
  gen_attitude.forward = forward;
  gen_attitude.up = up;
  gen_attitude.right = right;

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
  auto visible_stars_in_world =
      filterNaviStars(Config{fov_x, fov_y, resolution_x, resolution_y, mag_threshold}, catalogue);

  // for (auto star : visible_stars_in_world) {
  //   std::cout << "Star world coord (" << star.x << ", " << star.y << ", " << star.z << ")" << std::endl;
  // }

  std::cout << "Visible star number by magnitude: " << visible_stars_in_world.size() << std::endl;
  std::vector<SpatialStarEntry> stars_in_img_spatial_coord;
  auto low = sphereToCartesian(-fov_x / 2, -fov_y / 2);
  auto hi = sphereToCartesian(fov_x / 2, fov_y / 2);
  for (const auto &star: visible_stars_in_world) {
    Vec3f local_coord = rot_w2l * Vec3f(star.x, star.y, star.z);
    SpatialStarEntry local_star{local_coord.x(), local_coord.y(), local_coord.z(), star.magnitude};
    bool in_view = true;
    if (local_star.x < low.x() || local_star.y < low.y() || local_star.x > hi.x() || local_star.y > hi.y()
        || local_star.z > 0) {
      in_view = false;
    }
    if (in_view) {
      stars_in_img_spatial_coord.push_back(local_star);
      // std::cout << "star in world spatial coord (" << star.x << ", " << star.y << ", " << star.z
      //           << ")" << std::endl;
      // std::cout << "star in local spatial coord (" << local_star.x << ", " << local_star.y << ", " << local_star.z
      //           << ")" << std::endl;
    }
  }
  // step 4: project selected stars on img plane.
  std::vector<ImgStarEntry> star_point_in_img;
  for (auto &star: stars_in_img_spatial_coord) {
    float x_normalized = (star.x / std::abs(star.z) / std::tanf(degToRad(fov_x / 2.f)) + 1) / 2;
    float y_normalized = (star.y / std::abs(star.z) / std::tanf(degToRad(fov_y / 2.f)) + 1) / 2;
    float coord_img_x = x_normalized * static_cast<float>(resolution_x);
    float coord_img_y = y_normalized * static_cast<float>(resolution_y);
    // std::cout << "Img coord (" << x_normalized << ", " << y_normalized << ")" << std::endl;
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
    for (const auto &star: star_point_in_img) {
      fout << star.x << ',' << star.y << ',' << star.magnitude << std::endl;
    }
  }
  std::cout << "Generate img done, " << star_point_in_img.size() << " stars in img." << std::endl;
  return star_point_in_img;
}
