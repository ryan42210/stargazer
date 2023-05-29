#include "match.h"
#include "catalogue_io.h"
#include "cam_input.h"

#include <iostream>

int main() {
  // read raw catalogue
  std::string catalogue_path("../../Tycho-2/data/tyc2.dat");
  std::vector<CatEntry> catalogue;
  readCatalogue(catalogue_path, catalogue);

  // build navi star list;
  std::vector<NaviStarEntry> navi_star_list;

  // read camera input;
  std::vector<std::vector<ImgStarPoint>> img_series;
  readCamInput();

  Config config{};

  CoordMatch solver;
  solver.initialize(navi_star_list, config);

  // matching loops
  for (auto& img : img_series) {
    Attitude output;
    if (solver.match(img, output)) {
      std::cout << output.forward.x() << ", " << output.forward.y() << ", " << output.forward.z() << std::endl;
      std::cout << output.up.x() << ", " << output.up.y() << ", " << output.up.z() << std::endl;
      std::cout << output.right.x() << ", " << output.right.y() << ", " << output.right.z() << std::endl;
    } else {
      std::cerr << "Err: Fail to match attitude..." << std::endl;
      break;
    }
  }

  return 0;
}