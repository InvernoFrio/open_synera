#pragma once

#include <iostream>

#define ENGINE_LOG_INFO(x)  std::cout << "[Info] " << x << std::endl
#define ENGINE_LOG_WARN(x)  std::cout << "[Warn] " << x << std::endl
#define ENGINE_LOG_ERROR(x) std::cerr << "[Error] " << x << std::endl