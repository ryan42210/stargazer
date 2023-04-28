#ifndef CATALOGUE_IO_H_
#define CATALOGUE_IO_H_

#include <vector>
#include <string>

struct CatEntryText{
  char data[207];
};

struct CatShortEntry {
  float right_ascension;
  float declination;
  float B_magnitude;
  float T_magnitude;
};

CatShortEntry entryTextToData(CatEntryText &line);

void readCatalogue(std::string filepath, std::vector<CatShortEntry>& catalogue);

#endif