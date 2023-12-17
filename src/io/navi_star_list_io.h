#ifndef NAVI_STAR_LIST_IO_H_
#define NAVI_STAR_LIST_IO_H_

#include "catalogue.h"

#include <string>
#include <vector>
#include <optional>

void readNaviStarList(const std::string &filename);
void writeNaviStarList(const std::vector<SpatialStarEntry> &navi_star_list, std::optional<std::string> filepath);

#endif //NAVI_STAR_LIST_IO_H_
