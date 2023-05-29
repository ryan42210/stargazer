#ifndef CATALOGUE_IO_H_
#define CATALOGUE_IO_H_

#include "catalogue.h"

#include <vector>
#include <string>

struct CatEntryText{
  char data[207];
};

CatEntry entryTextToData(CatEntryText &line);
void readCatalogue(const std::string& filename, std::vector<CatEntry>& catalogue);

void loadStarInView(std::string filename);
void SaveNaviStarListToFile(std::string output_path);

#endif