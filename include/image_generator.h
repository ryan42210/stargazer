#ifndef IMG_GENERATOR_H_
#define IMG_GENERATOR_H_

#include "config.h"
#include "catalogue.h"

#include <vector>
#include <optional>
#include <string>

class ImageGenerator {
 public:
  void initialize(const Config &cfg, const std::vector<CatEntry> &cata);
  std::vector<ImgStarEntry> genImg(const std::optional<std::string> &filepath);
 private:
  float fov_x, fov_y;
  int resolution_x, resolution_y;
  float mag_threshold;

  std::vector<ImgStarEntry> img_stars;
  std::vector<CatEntry> catalogue;
};

#endif //IMG_GENERATOR_H_
