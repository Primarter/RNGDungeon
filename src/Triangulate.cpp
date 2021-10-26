// #include <stdlib.h>
// #include <unistd.h>
#include <math.h>
#include <vector>
#include <stdexcept>
#include "raylib.h"
#include "stdInclude.hpp"
#include "lib/Lib.hpp"

/*
     Triangulation subroutine
     Takes as input NV vertices in array pxyz
     Returned is a list of ntri triangular faces in the array v
     These triangles are arranged in a consistent clockwise order.
     The triangle array 'v' should be malloced to 3 * nv
     The vertex array pxyz must be big enough to hold 3 more points
     The vertex array must be sorted in increasing x values say

     qsort(p,nv,sizeof(Point),XYZCompare);
          :
     int XYZCompare(void *v1,void *v2)
     {
          Point *p1,*p2;
          p1 = v1;
          p2 = v2;
          if (p1->x < p2->x)
                return(-1);
          else if (p1->x > p2->x)
                return(1);
          else
                return(0);
     }
*/

// int CircumCircle(Vector2 p, Triangle t,
//    float &xc,float &yc,float &rsqr);

bool CircumCircle(Point p, Triangle t, Circle &c);

std::ostream &operator<<(std::ostream &lhs, ITriangle &tri)
{
    lhs << "{p1: " << tri.p1 << ", p2: " << tri.p2 << ", p3: " << tri.p3 << "}";
    return lhs;
}

void drawTrianglesFromIndices(const std::vector<Point> &pts, const std::vector<ITriangle> &triIndices)
{
    for (auto &&t : triIndices) {
        if (t.p1 == 0 && t.p2 == 0 && t.p3 == 0)
            break;
        DrawTriangleLines(pts[t.p1], pts[t.p2], pts[t.p3], DARKGREEN);
    }
}

std::vector<ITriangle> Triangulate(std::vector<Point> &pxyz)
{
    int emax = 200;
    int nv = pxyz.size();
    std::sort(pxyz.begin(), pxyz.end(), [](Point &p1, Point &p2) { return p1.x < p2.x; });
    pxyz.resize(nv + 3);

    std::vector<ITriangle> v(nv * 3);

    /* Allocate memory for the completeness list, flag for each triangle */
    int trimax = 4 * nv;
    std::vector<int> complete(trimax);

    /* Allocate memory for the edge list */
    std::vector<IEdge> edges(emax);

    /*
        Find the maximum and minimum vertex bounds.
        This is to allow calculation of the bounding triangle
    */
    float xmin = pxyz[0].x;
    float ymin = pxyz[0].y;
    float xmax = xmin;
    float ymax = ymin;
    for (int i=1;i<nv;i++) {
        if (pxyz[i].x < xmin) xmin = pxyz[i].x;
        if (pxyz[i].x > xmax) xmax = pxyz[i].x;
        if (pxyz[i].y < ymin) ymin = pxyz[i].y;
        if (pxyz[i].y > ymax) ymax = pxyz[i].y;
    }
    float dx = xmax - xmin;
    float dy = ymax - ymin;
    float dmax = (dx > dy) ? dx : dy;
    float xmid = (xmax + xmin) / 2.0;
    float ymid = (ymax + ymin) / 2.0;

    /*
        Set up the supertriangle
        This is a triangle which encompasses all the sample points.
        The supertriangle coordinates are added to the end of the
        vertex list. The supertriangle is the first triangle in
        the triangle list.
    */
    pxyz[nv+0].x = xmid - 20 * dmax;
    pxyz[nv+0].y = ymid - dmax;
    pxyz[nv+1].x = xmid;
    pxyz[nv+1].y = ymid + 20 * dmax;
    pxyz[nv+2].x = xmid + 20 * dmax;
    pxyz[nv+2].y = ymid - dmax;
    v[0].p1 = nv;
    v[0].p2 = nv+1;
    v[0].p3 = nv+2;
    complete[0] = false;
    int ntri = 1;

    /*
        Include each point one at a time into the existing mesh
    */
    for (int i=0;i<nv;i++) {

        // float xp = pxyz[i].x;
        // float yp = pxyz[i].y;
        Point p = pxyz[i];
        int nedge = 0;

        /*
            Set up the edge buffer.
            If the point (xp,yp) lies inside the circumcircle then the
            three edges of that triangle are added to the edge buffer
            and that triangle is removed.
        */
        for (int j=0;j<ntri;j++) {
            if (complete[j])
                continue;
            Triangle t = {
                pxyz[v[j].p1],
                pxyz[v[j].p2],
                pxyz[v[j].p3]
            };
            Circle c;
            bool inside = CircumCircle(p,t,c);
            if (c.x < p.x && ((p.x-c.x)*(p.x-c.x)) > c.r)
                complete[j] = true;
            if (inside) {
                /* Check that we haven't exceeded the edge list size */
                if (nedge+3 >= emax) {
                    emax += 100;
                    edges.resize(emax);
                }
                edges[nedge+0].p1 = v[j].p1;
                edges[nedge+0].p2 = v[j].p2;
                edges[nedge+1].p1 = v[j].p2;
                edges[nedge+1].p2 = v[j].p3;
                edges[nedge+2].p1 = v[j].p3;
                edges[nedge+2].p2 = v[j].p1;
                nedge += 3;
                v[j] = v[ntri-1];
                complete[j] = complete[ntri-1];
                ntri--;
                j--;
            }
        }

        /*
            Tag multiple edges
            Note: if all triangles are specified anticlockwise then all
                    interior edges are opposite pointing in direction.
        */
        for (int j=0;j<nedge-1;j++) {
            for (int k=j+1;k<nedge;k++) {
                if ((edges[j].p1 == edges[k].p2) && (edges[j].p2 == edges[k].p1)) {
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
                /* Shouldn't need the following, see note above */
                if ((edges[j].p1 == edges[k].p1) && (edges[j].p2 == edges[k].p2)) {
                    edges[j].p1 = -1;
                    edges[j].p2 = -1;
                    edges[k].p1 = -1;
                    edges[k].p2 = -1;
                }
            }
        }

        /*
            Form new triangles for the current point
            Skipping over any tagged edges.
            All edges are arranged in clockwise order.
        */
        for (int j=0;j<nedge;j++) {
            if (edges[j].p1 < 0 || edges[j].p2 < 0)
                continue;
            if (ntri >= trimax) {
                throw std::runtime_error("ntri >= trimax");
            }
            v[ntri].p1 = edges[j].p1;
            v[ntri].p2 = edges[j].p2;
            v[ntri].p3 = i;
            complete[ntri] = false;
            ntri++;
        }
    }

    /*
        Remove triangles with supertriangle vertices
        These are triangles which have a vertex number greater than nv
    */
    for (int i=0;i<ntri;i++) {
        if (v[i].p1 >= nv || v[i].p2 >= nv || v[i].p3 >= nv) {
            v[i] = v[ntri-1];
            ntri--;
            i--;
        }
    }

    v.resize(ntri);
    return(v);
}

/*
    Return true if a point (xp,yp) is inside the circumcircle made up
    of the points (x1,y1), (x2,y2), (x3,y3)
    The circumcircle centre is returned in (xc,yc) and the radius r
    NOTE: A point on the edge is inside the circumcircle
*/
bool CircumCircle(Point p, Triangle t,
    Circle &c)
{
    float m1,m2,mx1,mx2,my1,my2 = 0;
    float fabsy1y2 = fabs(t.p1.y-t.p2.y);
    float fabsy2y3 = fabs(t.p2.y-t.p3.y);

    /* Check for coincident points */
    if (fabsy1y2 < EPSILON && fabsy2y3 < EPSILON)
         return(false);

    if (fabsy1y2 < EPSILON) {
        m2 = - (t.p3.x-t.p2.x) / (t.p3.y-t.p2.y);
        mx2 = (t.p2.x + t.p3.x) / 2.0;
        my2 = (t.p2.y + t.p3.y) / 2.0;
        c.x = (t.p2.x + t.p1.x) / 2.0;
        c.y = m2 * (c.x - mx2) + my2;
    } else if (fabsy2y3 < EPSILON) {
        m1 = - (t.p2.x-t.p1.x) / (t.p2.y-t.p1.y);
        mx1 = (t.p1.x + t.p2.x) / 2.0;
        my1 = (t.p1.y + t.p2.y) / 2.0;
        c.x = (t.p3.x + t.p2.x) / 2.0;
        c.y = m1 * (c.x - mx1) + my1;
    } else {
        m1 = - (t.p2.x-t.p1.x) / (t.p2.y-t.p1.y);
        m2 = - (t.p3.x-t.p2.x) / (t.p3.y-t.p2.y);
        mx1 = (t.p1.x + t.p2.x) / 2.0;
        mx2 = (t.p2.x + t.p3.x) / 2.0;
        my1 = (t.p1.y + t.p2.y) / 2.0;
        my2 = (t.p2.y + t.p3.y) / 2.0;
        c.x = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        if (fabsy1y2 > fabsy2y3) {
            c.y = m1 * (c.x - mx1) + my1;
        } else {
            c.y = m2 * (c.x - mx2) + my2;
        }
    }

    float dx = t.p2.x - c.x;
    float dy = t.p2.y - c.y;
    c.r = dx*dx + dy*dy;

    dx = p.x - c.x;
    dy = p.y - c.y;
    float drsqr = dx*dx + dy*dy;

    // Original
    //return((drsqr <= *rsqr) ? true : false);
    // Proposed by Chuck Morris
    return((drsqr - c.r) <= EPSILON ? true : false);
}

