#ifndef H_GAME_UTILS
#define H_GAME_UTILS

// headers
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// for image loading
// the preprocessor modifies the header file such that it only contains 
// the relevant definition source code, effectively turning the header file into a .cpp file
//#define STB_IMAGE_IMPLEMENTATION
//#include"stb_image.h"

uint64_t timeSinceEpochMillisec();
extern std::string SHADER_RESOURCE_FILE_LOC;
extern std::string IMAGES_RESOURCE_FILE_LOC;
std::string readShader(const char* fileName); // read the shader file
#endif // !H_GAME_UTILS
