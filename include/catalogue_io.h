#ifndef CATALOGUE_IO_H_
#define CATALOGUE_IO_H_

#include "catalogue.h"

#include <vector>
#include <string>

struct CatEntryText{
  char data[207];
};

CatEntry entryTextToData(CatEntryText &line);

void readCatalogue(std::string filepath, std::vector<CatEntry>& catalogue);

#endif