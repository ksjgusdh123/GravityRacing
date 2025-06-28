#pragma once
#define NOMINMAX
#include <WS2tcpip.h>
#include <MSWSock.h>
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")

#include <iostream>
#include <vector>
#include <unordered_map>
#include <ranges>
#include <memory>
#include <typeindex>
#include <type_traits>

#include "Logger/Logger.h"
