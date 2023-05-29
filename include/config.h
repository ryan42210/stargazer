#ifndef CONFIG_H_
#define CONFIG_H_

struct Config {
  // config for camera
  float fov_x; // in degree
  float fov_y; // in degree
  int resolution_x;
  int resolution_y;

  // config for matching algorithm
  float mag_threshold;
};

#endif //CONFIG_H_
