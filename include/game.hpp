#pragma once

#include <iostream>
#include <vector>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

//#include "myshader.h"

#include "rlgl.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#define HERE std::cout << "HEEEEEEEEEEEEEEEEEEEEEERE !!!!" << std::endl;

typedef struct game_s {
    Camera camera;
    Model wall;
    Model ground;

    Shader shader_material;

    Light light;
    std::vector<bool> map;
    int size_x;
    int size_z;
} game_t;