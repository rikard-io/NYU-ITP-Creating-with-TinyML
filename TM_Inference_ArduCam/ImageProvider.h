
#ifndef IMAGE_PROVIDER_H_
#define IMAGE_PROVIDER_H_

#include <Arduino.h>
bool GetImage(int image_width, int image_height, int channels, uint8_t* image_data);
bool GetDummyImage(int image_width, int image_height, int channels, uint8_t* image_data);
#endif  // IMAGE_PROVIDER_H_
