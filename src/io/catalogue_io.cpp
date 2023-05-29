#include "catalogue_io.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

CatEntry entryTextToData(CatEntryText &line) {
  char *end;
  CatEntry out{};
  out.right_ascension = std::strtof(line.data + 15, &end);
  out.declination = std::strtof(line.data + 28, &end);
  out.B_magnitude = std::strtof(line.data + 110, &end);
  out.T_magnitude = std::strtof(line.data + 123, &end);
  return out;
}

void readCatalogue(std::string filename, std::vector<CatEntry> &catalogue) {
  if (!catalogue.empty()) {
    std::cerr << "Catalogue NOT empty! Attempt covering old data!" << std::endl;
    catalogue.clear();
  }

  std::ifstream fin(filename, std::ios::in | std::ios::binary);
  if (!fin.is_open()) {
    std::cerr << "Can not open catalogue file. Exit." << std::endl;
    exit(0);
  }

  CatEntryText record_entry{};
  while (fin.read((char *) &record_entry, sizeof(record_entry))) {
    catalogue.push_back(entryTextToData(record_entry));
  }
  fin.close();
}
