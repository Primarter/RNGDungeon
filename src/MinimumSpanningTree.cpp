#include "stdInclude.hpp"

std::vector<IEdge> getEdges(const std::vector<Point> &pts, const std::vector<ITriangle> &triIndices)
{
    std::vector<IEdge> edges;
    for (auto &&i : triIndices) {
        IEdge e1 = {i.p1, i.p2, Vector2Distance(pts[i.p1], pts[i.p2])};
        IEdge e2 = {i.p2, i.p3, Vector2Distance(pts[i.p2], pts[i.p3])};
        IEdge e3 = {i.p3, i.p1, Vector2Distance(pts[i.p3], pts[i.p1])};
        if (std::find(edges.begin(), edges.end(), e1) == edges.end())
            edges.push_back(e1);
        if (std::find(edges.begin(), edges.end(), e2) == edges.end())
            edges.push_back(e2);
        if (std::find(edges.begin(), edges.end(), e3) == edges.end())
            edges.push_back(e3);
    }
    return edges;
}

void drawEdgesFromIndices(const std::vector<Point> &pts, const std::vector<IEdge> &edges)
{
    for (auto &&e : edges) {
        DrawLineV(pts[e.p1], pts[e.p2], PURPLE);
        DrawCircleV(pts[e.p1], 5, PURPLE);
        DrawCircleV(pts[e.p2], 5, PURPLE);
    }
}

std::vector<IEdge> Kruskal(std::vector<Point> &points, std::vector<IEdge> &edges)
{
    std::vector<int> setIds;
    std::vector<IEdge> res;
    for (size_t i = 0; i < points.size(); i++) {
        setIds.push_back(i);
    }
    std::sort(edges.begin(), edges.end(), [](IEdge &e1, IEdge &e2) { return e1.weight < e2.weight; });
    for (auto &&e : edges) {
        std::cout << "edge { " << e.weight << ", " << e.p1 << ", " << e.p2 << " }" << std::endl;
    }
    for (auto it = edges.begin(); it != edges.end() && res.size() <= points.size() - 1; it++) {
        if (setIds[it->p1] != setIds[it->p2]) {
            res.push_back(*it);
            for (auto &&id : setIds)
                if (id == setIds[it->p2])
                    id = setIds[it->p1];
        }
    }
    return res;
}

