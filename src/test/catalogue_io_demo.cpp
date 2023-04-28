#include "catalogue_io.h"
#include <cstdio>

int main() {
  std::vector<CatEntry> catalogue;
  readCatalogue("D:/workspace/stargazer/Tycho-2/data/tyc2.dat", catalogue);
  printf("Read %lld entries. Top 10 lines:\n", catalogue.size());
  for (int i = 0; i < 10; i++) {
    printf("ra = %12.8f, dec = %12.8f, B_mag = %6.3f, T_mag = %6.3f\n",
           catalogue[i].right_ascension,
           catalogue[i].declination,
           catalogue[i].B_magnitude,
           catalogue[i].T_magnitude);
  }
  return 0;
}