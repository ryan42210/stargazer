#ifndef NAVI_STAR_FILTER_H_
#define NAVI_STAR_FILTER_H_

#include "math_definition.h"
#include "catalogue.h"
#include "config.h"

#include <vector>

float computeMagForMatch(float B_mag, float T_mag);

std::vector<SpatialStarEntry> filterNaviStars(const Config &cfg, const std::vector<CatEntry> &catalogue);

#endif //NAVI_STAR_FILTER_H_
