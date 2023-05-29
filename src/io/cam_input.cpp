#include "cam_input.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

void readCamInput(const std::string &filename, Config &out_cfg, std::vector<ImgStarEntry> &img) {
  std::ifstream fin(filename);
  if (!fin.is_open()) {
    std::cerr << "Fail to open camera input..." << std::endl;
    exit(-1);
  }

  std::istringstream line;
  std::string buffer;
  std::string data;
  char *end;

  // read line 1: fov and mag
  std::getline(fin, buffer);
  line.str(buffer);
  std::getline(line, data, ',');
  out_cfg.fov_x = std::strtof(data.c_str(), &end);
  std::getline(line, data, ',');
  out_cfg.fov_y = std::strtof(data.c_str(), &end);
  std::getline(line, data, ',');
  out_cfg.mag_threshold = std::strtof(data.c_str(), &end);
  std::getline(line, data, ',');
  out_cfg.resolution_x = std::strtol(data.c_str(), &end, 10);
  std::getline(line, data);
  out_cfg.resolution_y = std::strtol(data.c_str(), &end, 10);

  // read star point data
  img.clear();
  while (std::getline(fin, buffer)) {
    line.clear();
    line.str(buffer);
    ImgStarEntry star{};
    std::getline(line, data, ',');
    star.x = std::strtof(data.c_str(), &end);
    std::getline(line, data, ',');
    star.y = std::strtof(data.c_str(), &end);
    std::getline(line, data);
    star.magnitude = std::strtof(data.c_str(), &end);

    img.push_back(star);
  }
  fin.close();
}