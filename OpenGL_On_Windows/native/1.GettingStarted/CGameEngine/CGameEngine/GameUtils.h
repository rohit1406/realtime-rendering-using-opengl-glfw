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

uint64_t timeSinceEpochMillisec();
const char* readShader(const char* fileName); // read the shader file
#endif // !H_GAME_UTILS
