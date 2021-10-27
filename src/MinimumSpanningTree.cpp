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

std::vector<IEdge> Kruskal(std::vector<Point> &points, std::vector<IEdge> edges)
{
    std::vector<int> treeIds;
    std::vector<IEdge> res;
    for (size_t i = 0; i < points.size(); i++) {
        treeIds.push_back(i);
    }
    std::sort(edges.begin(), edges.end(), [](IEdge &e1, IEdge &e2) { return e1.weight < e2.weight; });
    for (auto it = edges.begin(); it != edges.end() && res.size() < points.size() - 1; it++) {
        if (treeIds[it->p1] != treeIds[it->p2]) {
            res.push_back(*it);
            int treeIdToMerge = treeIds[it->p2];
            int idToMergeTo = treeIds[it->p1];
            for (size_t i = 0; i < treeIds.size(); i++)
                if (treeIds[i] == treeIdToMerge)
                    treeIds[i] = idToMergeTo;
        }
    }
    size_t added = 0;
    for (auto &&it = edges.end() - 1; it != edges.begin() && added < (edges.size() / 10); it--) {
        std::cout << it->p1 << " " << it->p2 << " " << it->weight << std::endl;
        if (std::find(res.begin(), res.end(), *it) == res.end()) {
            res.push_back(*it);
            added++;
        }
    }
    return res;
}

