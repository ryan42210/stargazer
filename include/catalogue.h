#ifndef CATALOGUE_H_
#define CATALOGUE_H_

#include "core.h"

struct CatEntry {
  float right_ascension;
  float declination;
  // TODO: add more info in entry for filtering;
  float B_magnitude;
  float T_magnitude;
};

struct NaviStarEntry {
  // float ra;
  // float dec;
  Vec3f pos;
  float magnitude;
};

#endif //CATALOGUE_H_
