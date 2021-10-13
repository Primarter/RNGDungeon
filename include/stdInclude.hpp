/*
** EPITECH PROJECT, 2020
** B-CCP-400-MPL-4-1-theplazza-alec.ferrari
** File description:
** stdInclude.hpp
*/

#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <memory>
#include <fstream>
#include <exception>
#include <regex>
#include <sstream>
#include <string.h>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <typeinfo>
#include <set>
#include <filesystem>
#include <bitset>
#include <array>
#include <random>
#include <raylib.h>
#include <raymath.h>
#include "imgui.h"
#include "rlImGui.h"

#define N 40
#define DIST_FROM_CENTER 50
#define MIN_SIZE 20
#define MAX_SIZE 80

#define WIDTH 1280
#define HEIGHT 720

#define RANGE_RAND(nb, min, max) nb % (max - min) + min

#ifndef WIN32
	#define UNUSED __attribute__((unused))
#else
	#define UNUSED /* blank */
#endif

typedef unsigned int id_t;

template <typename T>
using ref = std::reference_wrapper<T>;