#ifndef CATALOGUE_H_
#define CATALOGUE_H_

struct CatEntry {
  float right_ascension;
  float declination;
  // TODO: add more info in entry for filtering;
  float B_magnitude;
  float T_magnitude;
};

struct CatShortEntry {
  float ra;
  float dec;
  float magnitude;
};

#endif //CATALOGUE_H_
