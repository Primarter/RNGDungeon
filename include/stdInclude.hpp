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
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "imgui.h"
#include "rlImGui.h"

#define N 40
#define DIST_FROM_CENTER 50
#define MIN_SIZE 20
#define MAX_SIZE 80

#define WIDTH 1280
#define HEIGHT 720

#define THICKNESS 3

#define RANGE_RAND(min, max) (rand() % (max - min) + min)

#ifndef WIN32
	#define UNUSED __attribute__((unused))
#else
	#define UNUSED /* blank */
#endif

typedef unsigned int id_t;

template <typename T>
using ref = std::reference_wrapper<T>;

#define EPSILON 0.001

#define GRIDRES 8

typedef Vector2 Point;

struct IEdge {
    int p1,p2;
    float weight;
    bool operator==(const IEdge &rhs) {
        return ((this->p1 == rhs.p1 && this->p2 == rhs.p2) || (this->p1 == rhs.p2 && this->p2 == rhs.p1));
    }
};

struct ITriangle {
    int p1,p2,p3;
};

void drawTrianglesFromIndices(const std::vector<Point> &pts, const std::vector<ITriangle> &triIndices);
std::vector<ITriangle> Triangulate(std::vector<Point> &pxyz);



std::vector<IEdge> Kruskal(std::vector<Point> &points, std::vector<IEdge> edges);
std::vector<IEdge> getEdges(const std::vector<Point> &pts, const std::vector<ITriangle> &triIndices);
void drawEdgesFromIndices(const std::vector<Point> &pts, const std::vector<IEdge> &edges);

struct Triangle {
    Point p1;
    Point p2;
    Point p3;
};

struct Edge {
    Point p1, p2;
};

/*
    p1 -> p2 is horizontal
    p2 -> p3 is vertical
 */
struct Corridor {
    Corridor(Edge e);
    Point p1, p2, p3;
};

enum CellType {
    EMPTY,
    ROOM,
    CORRIDOR
};

struct Cell {
    CellType type;
    int parentId;
};

struct Grid {
    std::vector<std::vector<Cell>> cells;
    int width;
    int height;
};

std::vector<Edge> getEdgeVal(std::vector<Point> &points, std::vector<IEdge> iedges);
std::vector<Corridor> getCorridors(std::vector<Edge> edges);
void drawCorridors(std::vector<Corridor> corridors);

struct Circle {
    float x;
    float y;
    float r;
};