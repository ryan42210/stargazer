#ifndef CATALOGUE_H_
#define CATALOGUE_H_

struct CatEntry {
  float right_ascension;
  float declination;
  // TODO: add more info in entry for filtering;
  float B_magnitude;
  float T_magnitude;
  float mag;
};

struct SpatialStarEntry {
  // float ra;
  // float dec;
  float x, y, z;
  float magnitude;
};

struct ImgStarEntry {
  float x, y;
  float magnitude;
};

#endif //CATALOGUE_H_
