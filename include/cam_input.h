#ifndef CAM_INPUT_H_
#define CAM_INPUT_H_

#include "config.h"
#include "catalogue.h"

#include <string>
#include <vector>

void readCamInput(const std::string& filename, Config& out_cfg, std::vector<ImgStarPoint>& img);

#endif //CAM_INPUT_H_
