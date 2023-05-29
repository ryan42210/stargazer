#include "catalogue_io.h"
#include "cam_input.h"
#include <cstdio>

int main() {
  printf("Start testing IO...\n");

  printf("1. Read raw catalogue...\n\n");
  std::vector<CatEntry> catalogue;
  readCatalogue("D:/workspace/stargazer/Tycho-2/data/tyc2.dat", catalogue);
  printf("Read %lld entries. Top 5 lines:\n", catalogue.size());
  for (int i = 0; i < 5; i++) {
    printf("ra = %12.8f, dec = %12.8f, B_mag = %6.3f, T_mag = %6.3f\n",
           catalogue[i].right_ascension,
           catalogue[i].declination,
           catalogue[i].B_magnitude,
           catalogue[i].T_magnitude);
  }

  printf("\n2. Write navigation star list...\n\nTODO\n\n");

  printf("\n3. Read existing navigation star list...\n\nTODO\n\n");

  printf("\n4. Read image star point from camera input...\n\n");
  Config cfg{};
  std::vector<ImgStarPoint> img;
  readCamInput("D:/workspace/stargazer/data/img_test.txt", cfg, img);
  printf("Cam Config:\n");
  printf("fov_x: %5.3f, fov_y: %5.3f, mag_threshold: %4.1f, resolution_x: %d, resolution_y: %d\n\n",
         cfg.fov_x,
         cfg.fov_y,
         cfg.mag_threshold,
         cfg.resolution_x,
         cfg.resolution_x);
  for (const auto& star : img) {
    printf("Star img coordinate: (%8.3f, %8.3f), magnitude: %3.1f\n", star.x, star.y, star.magnitude);
  }

  printf("\nAll IO test has been finished.\n");
  return 0;
}