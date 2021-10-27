#include "stdInclude.hpp"

Corridor::Corridor(Edge e) {
    this->p1 = e.p1;
    this->p2 = {e.p1.x + (e.p2.x - e.p1.x), e.p1.y};
    this->p3 = e.p2;
}

std::vector<Edge> getEdgeVal(std::vector<Point> &points, std::vector<IEdge> iedges)
{
    std::vector<Edge> res;
    for (auto &&ie : iedges) {
        res.push_back({points[ie.p1], points[ie.p2]});
    }
    return res;
}

std::vector<Corridor> getCorridors(std::vector<Edge> edges)
{
    return std::vector<Corridor>(edges.begin(), edges.end());
}

void drawCorridors(std::vector<Corridor> corridors)
{
    for (auto &&c : corridors) {
        DrawLineV(c.p1, c.p2, GREEN);
        DrawLineV(c.p2, c.p3, GREEN);
    }
}

