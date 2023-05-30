#include "catalogue_io.h"
#include "cam_input.h"
#include "coordinate.h"
#include "navi_star_filter.h"
#include "image_generator.h"

#include "math_util.h"

#include <iostream>

int main() {
  // read raw catalogue
  std::string catalogue_path("D:/workspace/stargazer/Tycho-2/data/tyc2.dat");
  std::vector<CatEntry> catalogue;
  std::cout << "Start reading catalogue..." << std::endl;
  readCatalogue(catalogue_path, catalogue);
  std::cout << "Read catalogue done." << std::endl;

  // read camera input, or randomly generate one;
  bool use_external_cam_input = false;
  Config config{};
  std::vector<std::vector<ImgStarEntry>> img_series;
  std::vector<SpatialStarEntry> navi_star;
  std::vector<Attitude> attitude_series;
  const int img_num = 3;
  if (use_external_cam_input) {
    std::vector<ImgStarEntry> img;
    img_series.push_back(img);
    std::cout << "Start reading image data..." << std::endl;
    std::string img_path("D:/workspace/stargazer/data/img_test.txt");
    readCamInput(img_path, config, img_series[0]);
    std::cout << "Read image data done." << std::endl;
  } else {
    std::cout << "Gen input from simulator..." << std::endl;
    config = Config{15, 15, 800, 800, 6};
    ImageGenerator img_gen;
    img_gen.initialize(config, catalogue);
    for (int i = 0; i < img_num; i++) {
      Attitude att;
      img_series.push_back(img_gen.genImg(std::nullopt, att));
      attitude_series.push_back(att);
    }
  }

  // build navi star list;
  std::cout << "Start building navigations star list." << std::endl;
  auto navi_star_list = filterNaviStars(config, catalogue);
  std::cout << "In total " << navi_star_list.size() << " stars for navigation." << std::endl;

  CoordMatch solver;
  std::cout << "Initialize solver..." << std::endl;
  solver.initialize(navi_star_list, config);
  std::cout << "Start solving attitude..." << std::endl;

  // matching loops
  for (int i = 0; i < img_num; i++) {
    auto img_ = img_series[i];
    Attitude output = attitude_series[i];
    std::cout << "------------------- New Img ---------------" << std::endl;
    std::cout << "Expect attitude: " << output.forward.x() << ", " << output.forward.y() << ", " << output.forward.z() << std::endl;
    std::cout << "Expect attitude: " << output.up.x() << ", " << output.up.y() << ", " << output.up.z() << std::endl;
    std::cout << "Expect attitude: " << output.right.x() << ", " << output.right.y() << ", " << output.right.z() << std::endl;
    std::cout << "-- -- -- -- -- -- -- -- -- -- -- -- -- -- --" << std::endl;
    if (solver.match(img_, output)) {
      std::cout << "Attitude solved: " << output.forward.x() << ", " << output.forward.y() << ", " << output.forward.z() << std::endl;
      std::cout << "Attitude solved: " << output.up.x() << ", " << output.up.y() << ", " << output.up.z() << std::endl;
      std::cout << "Attitude solved: " << output.right.x() << ", " << output.right.y() << ", " << output.right.z() << std::endl;
    } else {
      std::cerr << "Err: Fail to match attitude..." << std::endl;
      break;
    }
  }

  return 0;
}