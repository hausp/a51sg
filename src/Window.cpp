/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <climits>
#include <cmath>
#include <unordered_map>
#include "Drawer.hpp"
#include "BicubicSurface.hpp"
#include "Line.hpp"
#include "Polygon.hpp"
#include "Window.hpp"
#include "Wireframe.hpp"

#define XMIN -1
#define YMIN -1
#define XMAX 1
#define YMAX 1

Window::Window(const Point<2>& min, const Point<2>& max)
: min(-(max/2)), max(max/2), angle(0), currentZoom(1), lcAlgorithm(2) {
  //vpn(new Line<3>({0, 0, 0}, {0, 0, 10})) {
    auto c = center3D();
    vpn.reset(new Line<3>(center3D(), center3D() + Point<3>(0,0,1)));
    cop = {c[0], c[1], -10};
    TRACE(cop);
    defaultWidth  = max[0] - min[0];
    defaultHeight = max[1] - min[1];
}

Matrix<3,3> Window::normalizerMatrix() {
    Matrix<3,3> normalizer = utils::translationMatrix((-center()).toArray());
    normalizer *= utils::rotationMatrix<2>(-angle);
    std::array<double, 2> s = {2/(max[0] - min[0]), 2/(max[1] - min[1])};
    normalizer *= utils::scalingMatrix(s);
    return normalizer;
}

void Window::moveHorizontal(double displacement) {
    // auto m = utils::rotationMatrix<2>(-angle);
    // m *= utils::translationMatrix(std::array<double, 2>{displacement, 0});
    // m *= utils::rotationMatrix<2>(angle);
    // min *= m;
    // max *= m;
    // auto c = center3D();
    // cop = {c[0], c[1], -50};
    min[0] += displacement;
    max[0] += displacement;
    cop[0] += displacement;
}

void Window::moveVertical(double displacement) {
    // auto m = utils::rotationMatrix<2>(-angle);
    // m *= utils::translationMatrix(std::array<double, 2>{0, displacement});
    // m *= utils::rotationMatrix<2>(angle);
    // min *= m;
    // max *= m;
    // auto c = center3D();
    // cop = {c[0], c[1], -50};
    min[1] += displacement;
    max[1] += displacement;
    cop[1] += displacement;
}

void Window::rotate(double _angle) {
    angle = fmod(angle + _angle + 360, 360);
}

Point<2> Window::toViewport(const Viewport& viewport, const Point<2>& p) {
    double width  = viewport.second[0] - viewport.first[0];
    double height = viewport.second[1] - viewport.first[1];
    // auto& pn = drawer->getNDC(p);
    auto& pn = p;
    double x = (pn[0] + 1) / 2 * width + viewport.first[0];
    double y = (1 - (pn[1] + 1)/ 2) * height + viewport.first[1];
    return Point<2>(x, y);
}

Point<2> Window::center() const {
    return (min + max)/2;
}

Point<3> Window::center3D() const {
    auto c = center();
    return {c[0], c[1], 0};
}

void Window::zoom(double zoomRate) {
    if (currentZoom + zoomRate > 0) {
        currentZoom += zoomRate;
        double factor = 1 / (2 * currentZoom);
        Point<2> delta(defaultWidth * factor, defaultHeight * factor);
        auto c = center();
        min = c - delta;
        max = c + delta;
    }
}

double Window::getAngle() {
    return angle;
}

double Window::getZoomLevel() {
    return currentZoom;
}

void Window::setClippingAlgorithm(int algorithm) {
    lcAlgorithm = algorithm;
}

Point<2> Window::parallelProjection(const Point<2>& p) const {
    return p;
}

Point<2> Window::parallelProjection(const Point<3>& p) const {
    return projection(p);
}

Point<2> Window::perspectiveProjection(const Point<2>& p) const {
    return p;
}

Point<2> Window::perspectiveProjection(const Point<3>& p) const {
    auto projected = projection(p);
    double d = projected[2] - cop[2];
    double correctionFactor = projected[2] / d;
    projected[0] /= correctionFactor;
    projected[1] /= correctionFactor;
    return projected;
}

Point<3> Window::projection(Point<3> p) const {
    auto transformation = utils::translationMatrix((-cop).toArray());

    Point<3> xAxis(1, 0, 0);
    Point<3> yAxis(0, 1, 0);
    auto vector = (*vpn)[1] - (*vpn)[0];
    double tx = acos((xAxis * vector) / vector.norm()) * 180 / M_PI;
    double ty = acos((yAxis * vector) / vector.norm()) * 180 / M_PI;
    transformation *= utils::rotationMatrix<3>(90 - tx, utils::RotationPlane::X);
    transformation *= utils::rotationMatrix<3>(90 - ty, utils::RotationPlane::Y);
    p *= transformation;
    ECHO(p);
    return p;
}

void Window::clip(Point<2>& p) {
    auto pn = drawer->getNDC(p);
    p.setVisible(pn[0] >= -1 && pn[0] <= 1 && pn[1] >= -1 && pn[1] <= 1);
}

void Window::clip(Line<2>& ln) {
    switch(lcAlgorithm) {
        case 0:
            // std::cout << "Cohen-Sutherland clipping..." << std::endl;
            clipCS(ln);
            break;
        case 1:
            // std::cout << "Liang-Barsky clipping..." << std::endl;
            clipLB(ln);
            break;
        case 2:
            // std::cout << "Nicholl-Lee-Nicholl clipping..." << std::endl;
            clipNLN(ln);
            break;
    }
}

void Window::setDrawer(Drawer& _drawer) {
    drawer = &_drawer;
}

void Window::clip(Polygon<2>& polygon) {
    clockwiseSort(polygon);
    std::list<Point<2>> win = {{-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
    std::vector<Point<2>> incomingList;
    std::vector<Point<2>> auxList;
    std::vector<Point<2>> artifVert;
    buildLists(polygon, win, incomingList, auxList, artifVert);

    std::vector<Point<2>> winVector;
    winVector.reserve(win.size());
    for (auto point : win) {
        winVector.push_back(point);
    }

    // std::cout << "-----------------------------" << std::endl;
    std::vector<Point<2>> result;
    // std::cout << "Size of incoming list: " << incomingList.size() << std::endl;
    for (auto point : incomingList) {
        // std::cout << "Before athertonStep" << std::endl;
        //athertonStepPolygon(win, auxList, artifVert, point, point, result);
        athertonStep(winVector, auxList, artifVert, point, result);
        // std::cout << "After athertonStep" << std::endl;
    }

    if (incomingList.size() > 0) {
        drawer->getNDC(polygon).clear();
        for (auto point : result) {
            drawer->getNDC(polygon).push_back(point);
        }        
    } else {
        auto point = drawer->getNDC(polygon)[0];
        if (point[0] < -1 || point[0] > 1 || point[1] < -1 || point[1] > 1) {
            drawer->getNDC(polygon).clear();
        }
    }
}

void Window::clip(SimpleCurve<2>& c) {
    for (auto& line : c) {
        clip(line);
    }
}

void Window::clip(Curve<2>& curve) {
    for (auto& c : curve) {
        clip(c);
    }
}

void Window::clip(Point<3>& p) {
    Point<2> flatPoint(drawer->getNDC(p));
    drawer->getNDC(flatPoint) = flatPoint;
    clip(flatPoint);
    p.setVisible(flatPoint.isVisible() && p[2] >= 1 && p[2] <= 1000);
}

void Window::clip(Line<3>& ln) {
    Line<2> flatLine(drawer->getNDC(ln[0]), drawer->getNDC(ln[1]));
    drawer->getNDC(flatLine[0]) = flatLine[0];
    drawer->getNDC(flatLine[1]) = flatLine[1];
    clip(flatLine);
    ln.setVisible(flatLine.isVisible());
    drawer->getNDC(ln[0]) = drawer->getNDC(flatLine[0]);
    drawer->getNDC(ln[1]) = drawer->getNDC(flatLine[1]);
}

void Window::clip(Polygon<3>& polygon) {
    std::unordered_map<double, std::unordered_map<double, double>> coordinateMap;
    std::vector<Point<2>> flatPoints;
    auto points = polygon.points();
    for (auto& point : polygon) {
        coordinateMap[point[0]][point[1]] = point[2];
        flatPoints.push_back(drawer->getNDC(point));
    }

    Polygon<2> flatPolygon(flatPoints);
    drawer->getNDC(flatPolygon) = flatPoints;
    clip(flatPolygon);

    polygon.setVisible(flatPolygon.isVisible());
    for (unsigned i = 0; i < points.size(); i++) {
        drawer->getNDC(polygon[i]) = flatPolygon[i];
    }

    drawer->getNDC(polygon).clear();
    for (auto& flatPoint : drawer->getNDC(flatPolygon)) {
        double x = flatPoint[0];
        double y = flatPoint[1];
        Point<3> point(x, y, coordinateMap[x][y]);
        drawer->getNDC(polygon).push_back(point);
    }

    // std::vector<Point<2>> newPoints;
    // auto points = polygon.points();
    // for (auto& point : points) {
    //     newPoints.push_back(drawer->getNDC(point));
    // }
    // Polygon<2> flatPolygon(newPoints);
    // drawer->getNDC(flatPolygon) = newPoints;
    // clip(flatPolygon);
    // polygon.setVisible(flatPolygon.isVisible());
    // for (unsigned i = 0; i < points.size(); i++) {
    //     drawer->getNDC(polygon[i]) = flatPolygon[i];
    // }

    // drawer->getNDC(polygon).clear();
    // for (auto& flatPoint : drawer->getNDC(flatPolygon)) {
    //     Point<3> point(flatPoint[0], flatPoint[1], 1);
    //     drawer->getNDC(polygon).push_back(point);
    // }
}

void Window::clip(SimpleCurve<3>& c) {
    auto& lines = c.lines();
    for (auto& line : lines) {
        clip(line);
    }
}

void Window::clip(Curve<3>& curve) {
    for (auto& c : curve) {
        clip(c);
    }
}

void Window::clip(Wireframe<3>& wireframe) {
    auto& edges = wireframe.edges();
    for (auto& edge : edges) {
        clip(edge);
    }
}

void Window::clip(BicubicSurface& surface) {
    for (auto& c : surface) {
        clip(c);
    }
}



void Window::athertonStep(const std::vector<Point<2>>& win,
    const std::vector<Point<2>>& auxList, const std::vector<Point<2>>& artifVert,
    const Point<2>& point, std::vector<Point<2>>& result) {

    std::vector<std::vector<Point<2>>> lists = {auxList, win};
    int memory[] = {-1, -1};
    unsigned index = 0;
    unsigned i = 0;
    bool process = false;
    bool ignore = false;
    bool recentRotation = false;
    while (true) {
        if (lists[index][i] == point) {
            if (process) {
                result.push_back(point);
                // std::cout << "(" << point[0] << "," << point[1] << ")" << std::endl;
                break;
            }
            process = true;
            ignore = true;
            memory[index] = i;
        }

        if (!process) {
            i = (i + 1) % lists[index].size();
            continue;
        }

        if (!recentRotation) {
            result.push_back(lists[index][i]);
            // std::cout << "(" << lists[index][i][0] << "," << lists[index][i][1] << ")" << std::endl;
        } else {
            recentRotation = false;
        }

        if (!ignore) {
            bool found = false;
            for (auto vertex : artifVert) {
                if (vertex == lists[index][i]) {
                    found = true;
                    break;
                }
            }

            if (found) {
                // the last point visited
                auto& last = lists[index][i];

                // swaps the current list
                index = 1 - index;

                // finds the starting point for the new list if it's our first time
                if (memory[index] == -1) {
                    for (unsigned j = 0; j < lists[index].size(); j++) {
                        if (lists[index][j] == last) {
                            memory[index] = j;
                            break;
                        }
                    }
                }

                // sets the "recent rotation" flag
                recentRotation = true;

                // tells the algorithm to ignore the next artificial vertex
                ignore = true;

                // goes to the new starting point
                i = memory[index];
                continue;
            }
        }

        ignore = false;

        // rotates our iterator
        i = (i + 1) % lists[index].size();
    }
}

void Window::buildLists(Polygon<2>& polygon, std::list<Point<2>>& win,
    std::vector<Point<2>>& incomingList, std::vector<Point<2>>& auxList,
    std::vector<Point<2>>& artificialVertices) {
    unsigned size = polygon.numberOfPoints();
    int INVALID_VALUE = 2;
    std::vector<Point<2>> pn = drawer->getNDC(polygon);
    for (unsigned i = 0; i < size; i++) {
        auto& previous = pn[(i - 1 + size) % size];
        auto& current = pn[i];
        auto& next = pn[(i + 1) % size];
        double slope = utils::slope(Line<2>(current, next));
        double y1 = slope * (-1 - current[0]) + current[1];
        double y2 = slope * (1 - current[0]) + current[1];
        double x1 = (-1 - current[1])/slope + current[0];
        double x2 = (1 - current[1])/slope + current[0];
        if (y1 == y2) {
            // only happens if slope == 0
            if ((current[0] < -1 && next[0] >= -1 && next[0] < 1)
                || (current[0] >= -1 && next[0] < -1 && current[0] < 1)) {
                // invalidates the right intersection
                y2 = INVALID_VALUE;
            } else if ((current[0] <= 1 && next[0] > 1 && current[0] > -1)
                || (current[0] > 1 && next[0] <= 1 && next[0] > -1)) {
                // invalidates the left intersection
                y1 = INVALID_VALUE;
            }
        }
        if (x1 == x2) {
            // only happens if slope == +-inf
            if ((current[1] < -1 && next[1] >= -1 && next[1] < 1)
                || (current[1] >= -1 && next[1] < -1 && current[1] < 1)) {
                // invalidates the upper intersection
                x2 = INVALID_VALUE;
            } else if ((current[1] <= 1 && next[1] > 1 && current[1] > -1)
                || (current[1] > 1 && next[1] <= 1 && next[1] > -1)) {
                // invalidates the lower intersection
                x1 = INVALID_VALUE;
            }            
        }
        // std::cout << "line: ";
        // std::cout << "(" << current[0] << "," << current[1] << ") to";
        // std::cout << "(" << next[0] << "," << next[1] << ")" << std::endl;
        // std::cout << "slope = " << slope << std::endl;
        // std::cout << "x1 = " << x1 << std::endl;
        // std::cout << "y1 = " << y1 << std::endl;
        // std::cout << "x2 = " << x2 << std::endl;
        // std::cout << "y2 = " << y2 << std::endl;
        std::unordered_map<unsigned, Point<2>> intersections;
        if (-1 <= x2 && x2 <= 1) intersections[0] = Point<2>(x2, 1);  // top
        if (-1 <= y2 && y2 <= 1) intersections[1] = Point<2>(1, y2);  // right
        if (-1 <= x1 && x1 <= 1) intersections[2] = Point<2>(x1, -1); // bottom
        if (-1 <= y1 && y1 <= 1) intersections[3] = Point<2>(-1, y1); // left
        while (intersections.size() > 0) {
            bool stop = true;
            for (auto pair : intersections) {
                double u1 = (pair.second[0] - current[0]) / (next[0] - current[0]);
                double u2 = (pair.second[1] - current[1]) / (next[1] - current[1]);
                // std::cout << "u1 = " << u1 << std::endl;
                // std::cout << "u2 = " << u2 << std::endl;
                if (u1 < 0 || u1 > 1 || u2 < 0 || u2 > 1 || std::isinf(u1)
                    || std::isinf(u2)) {
                    intersections.erase(pair.first);
                    stop = false;
                    break;
                }
            }
            if (stop) {
                break;
            }
        }
        // std::cout << "intersections.size() = " << intersections.size() << std::endl;
        if (intersections.size() == 0) {
            if (auxList.size() > 0 && auxList.back() != current) {
                auxList.push_back(current);
            }
            auxList.push_back(next);
        } else {
            if (auxList.size() > 0 && auxList.back() != current) {
                auxList.push_back(current);
            }

            bool doubleIntersection = (intersections.size() == 2);
            std::pair<unsigned, double> lowest = {INT_MAX, INVALID_VALUE};
            if (doubleIntersection) {
                for (auto pair : intersections) {
                    double u = (pair.second[0] - current[0]) / (next[0] - current[0]);
                    //double u2 = (pair.second[1] - current[1]) / (next[1] - current[1]);
                    if (u < lowest.second) {
                        lowest.first = pair.first;
                        lowest.second = u;
                    }
                }
            }

            bool isIncoming = (current[0] < -1 || current[0] > 1
                || current[1] < -1 || current[1] > 1);
            unsigned incoming = INT_MAX;
            unsigned outcoming = INT_MAX;
            for (auto pair : intersections) {
                if (doubleIntersection) {
                    isIncoming = (lowest.first == pair.first);
                }
                if (current == pair.second) {
                    // point over border
                    if (next[0] >= -1 && next[0] <= 1 && next[1] >= -1 && next[1] <= 1) {
                        // next inside window, current may be incoming
                        isIncoming = false;
                        for (unsigned coord = 0; coord <= 1; coord++) {
                            auto magnitude = abs(previous[coord]);
                            isIncoming = isIncoming || (magnitude > 1 && current[coord] == magnitude/previous[coord]);
                        }
                    } else {
                        // next outside window, current may be outcoming
                        if (!doubleIntersection) {
                            // current IS outcoming
                            artificialVertices.push_back(pair.second);
                            listInsert(win, pair.first, pair.second);
                        }
                        // Prevents doubleIntersection doin' shit
                        isIncoming = false;
                    }
                }
                if (pair.second != next) {
                    if (isIncoming) {
                        // incomingList.push_back(pair.second);
                        // auxList.push_back(pair.second);
                        // artificialVertices.push_back(pair.second);
                        incoming = pair.first;
                        listInsert(win, pair.first, pair.second);
                    } else if (pair.second != current) {
                        // auxList.push_back(pair.second);
                        // artificialVertices.push_back(pair.second);
                        outcoming = pair.first;
                        listInsert(win, pair.first, pair.second);
                    }
                }
            }
            if (incoming != INT_MAX) {
                if (intersections[incoming] != next) {
                    incomingList.push_back(intersections[incoming]);
                    auxList.push_back(intersections[incoming]);
                    artificialVertices.push_back(intersections[incoming]);
                    //listInsert(win, incoming, intersections[incoming]);
                }
            }

            if (outcoming != INT_MAX) {
                if (intersections[outcoming] != next) {
                    // std::cout << "adding outcoming: (" << intersections[outcoming][0] << ", " << intersections[outcoming][1] << ")" << std::endl;
                    auxList.push_back(intersections[outcoming]);
                    artificialVertices.push_back(intersections[outcoming]);
                    //listInsert(win, outcoming, intersections[outcoming]);
                }
            }
            auxList.push_back(next);
        }
    }

    // std::cout << "Polygon:" << std::endl;
    // for (auto point : auxList) {
    //     std::cout << "(" << point[0] << "," << point[1] << ")" << std::endl;
    // }

    // std::cout << "Window:" << std::endl;
    // for (auto point : win) {
    //     std::cout << "(" << point[0] << "," << point[1] << ")" << std::endl;
    // }

    // std::cout << "Incoming:" << std::endl;
    // for (auto point : incomingList) {
    //     std::cout << "(" << point[0] << "," << point[1] << ")" << std::endl;
    // }
}

void Window::listInsert(std::list<Point<2>>& list, unsigned reference,
    const Point<2>& intersection) {
    std::vector<Point<2>> corners = {{-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
    for (auto it = list.begin(); it != list.end(); it++) {
        if (*it == corners[reference]) {
            it++;
            switch(reference) {
                case 0:
                    while (intersection[0] > (*it)[0]) it++;
                    break;
                case 1:
                    while (intersection[1] < (*it)[1]) it++;
                    break;
                case 2:
                    while (intersection[0] < (*it)[0]) it++;
                    break;
                case 3:
                    while (intersection[1] > (*it)[1]) it++;
                    break;
            }
            list.insert(it, intersection);
            break;
        }
    }
}

void Window::clockwiseSort(Polygon<2>& p) {
    unsigned size = p.numberOfPoints();
    double order = 0;
    for (unsigned i = 0; i < size; i++) {
        auto& next = p[(i + 1) % size];
        auto& current = p[i];
        order += (next[0] - current[0]) * (next[1] + current[1]);
    }

    if (order < 0) {
        for (unsigned i = 0; i < size/2; i++) {
            auto aux = p[i];
            p[i] = p[size - 1 - i];
            p[size - 1 - i] = aux;
        }
    }
}

void Window::clipCS(Line<2>& ln) {
    auto& p1 = drawer->getNDC(ln[0]);
    auto& p2 = drawer->getNDC(ln[1]);
    int rc1 = 0;
    int rc2 = 0;

    rc1 |= (p1[1] > 1);         // acima
    rc1 |= (p1[1] < -1) << 1;   // abaixo
    rc1 |= (p1[0] > 1) << 2;    // direita
    rc1 |= (p1[0] < -1) << 3;   // esquerda
    rc2 |= (p2[1] > 1);
    rc2 |= (p2[1] < -1) << 1;
    rc2 |= (p2[0] > 1) << 2;
    rc2 |= (p2[0] < -1) << 3;

    if (rc1 == 0 && rc2 == 0) {
        ln.setVisible(true);
        return;
    }

    if ((rc1 & rc2) != 0) {
        ln.setVisible(false);
        return;
    }

    ln.setVisible(true);
    int rc = (rc1 == 0) ? rc2 : rc1;
    double x, y;
    double m = utils::slope(Line<2>(p1, p2));
    if ((rc & 1) != 0) {
        x = p1[0] + (1 - p1[1]) / m;
        y = 1;
    } else if ((rc & 2) != 0) {
        x = p1[0] + (-1 - p1[1]) / m;
        y = -1;
    } else if ((rc & 4) != 0) {
        x = 1;
        y = p1[1] + m * (1 - p1[0]);
    } else {
        x = -1;
        y = p1[1] + m * (-1 - p1[0]);
    }

    if (rc == rc1) {
        std::cout << "(" << p1[0] << "," << p1[1] << ") -> (" << x << "," << y << ")" << std::endl;
        p1 = Point<2>(x, y);
    } else {
        std::cout << "(" << p2[0] << "," << p2[1] << ") -> (" << x << "," << y << ")" << std::endl;
        p2 = Point<2>(x, y);
    }

    clipCS(ln);
}

void Window::clipLB(Line<2>& ln) {
    auto& p1 = drawer->getNDC(ln[0]);
    auto& p2 = drawer->getNDC(ln[1]);
    double dx = p2[0] - p1[0];
    double dy = p2[1] - p1[1];
    double u1 = 0;
    double u2 = 1;
    double p[4], q[4];

    p[0] = -(p2[0] - p1[0]);
    p[1] = -p[0];
    p[2] = -(p2[1] - p1[1]);
    p[3] = -p[2];
    q[0] = p1[0] + 1;
    q[1] = 1 - p1[0];
    q[2] = p1[1] + 1;
    q[3] = 1 - p1[1];

    for (unsigned i = 0; i < 4; i++) {
        double r = q[i] / p[i];
        if (p[i] < 0) {
            if (r > u1) {
                u1 = r;
            }
        } else {
            if (r < u2) {
                u2 = r;
            }
        }
    }

    if (u1 > u2) {
        ln.setVisible(false);
        return;
    }

    ln.setVisible(true);

    if (u2 != 0 && u2 != 1) {
        p2[0] = p1[0] + u2 * dx;
        p2[1] = p1[1] + u2 * dy;
    }

    if (u1 != 0 && u1 != 1) {
        p1[0] += u1 * dx;
        p1[1] += u1 * dy;
    }
}

void Window::clipNLN(Line<2>& ln) {
    auto& p1 = drawer->getNDC(ln[0]);
    auto& p2 = drawer->getNDC(ln[1]);

    if (p1[0] < XMIN) {
        // P1 in left column
        ln.setVisible(leftcolumn(p1, p2));
    } else if (p1[0] > XMAX) {
        // P1 in right column, rotate 180°,
        // moving it to left column
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
        ln.setVisible(leftcolumn(p1, p2));
        // Rotate back
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
    } else {
        // P1 is in center column
        ln.setVisible(centercolumn(p1, p2));
    }
}

bool Window::leftcolumn(Point<2>& p1, Point<2>& p2) {
    if (p2[0] < XMIN) {
        // Line is out of window
        return false;
    }
    if (p1[1] > YMAX) {
        // P1 is in top left corner
        return topleftcorner(p1, p2);
    }
    if (p1[1] < YMIN) {
        // P1 is in bottom left corner, reflect in x axis,
        // moving to top left corner
        utils::nln_reflect_xaxis(p1);
        utils::nln_reflect_xaxis(p2);
        bool visible = topleftcorner(p1, p2);
        // reflect back
        utils::nln_reflect_xaxis(p1);

        utils::nln_reflect_xaxis(p2);
        return visible;
    }
    return leftedge(p1, p2);
}

bool Window::topleftcorner(Point<2>& p1, Point<2>& p2) {
    if (p2[1] > YMAX) {
        // Line is out of window
        return false;
    }

    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    double m1 = (YMAX - p1[1]) / (XMIN - p1[0]);

    if (m0 <= m1) {
        return leftbottomregion(p1, p2, m0);
    } else {
        utils::nln_reflect_xmy(p1);
        utils::nln_reflect_xmy(p2);
        bool visible = leftbottomregion(p1, p2, 1/m0);
        utils::nln_reflect_xmy(p1);
        utils::nln_reflect_xmy(p2);
        return visible;
    }
}

bool Window::leftbottomregion(Point<2>& p1, Point<2>& p2, double m0) {
    if (p2[1] >= YMIN) {
        if (p2[0] > XMAX) {
            p2[0] = XMAX;
            p2[1] = p1[1] + (XMAX - p1[0]) * m0;
        }
    } else {
        double m2 = (YMIN - p1[1]) / (XMIN - p1[0]);
        if (m2 >= m0) {
            return false;
        }
        if (p2[0] > XMAX) {
            double m3 = (YMIN - p1[1]) / (XMAX - p1[0]);
            if (m3 > m0) {
                p2[0] = p1[0] + (YMIN - p1[1]) / m0;
                p2[1] = YMIN;
            } else {
                p2[0] = XMAX;
                p2[1] = p1[1] + (XMAX - p1[0]) * m0;
            }
        } else {
            p2[0] = p1[0] + (YMIN - p1[1]) / m0;
            p2[1] = YMIN;
        }
    }

    p1[1] += (XMIN - p1[0]) * m0;
    p1[0] = XMIN;
    return true;
}

bool Window::leftedge(Point<2>& p1, Point<2>& p2) {
    if (p2[0] < XMIN) {
        return false;
    }

    if (p2[1] < YMIN) {
        return p2bottom(p1, p2);
    }

    if (p2[1] > YMAX) {
        utils::nln_reflect_xaxis(p1);
        utils::nln_reflect_xaxis(p2);
        bool visible = p2bottom(p1, p2);
        utils::nln_reflect_xaxis(p1);
        utils::nln_reflect_xaxis(p2);
        return visible;
    }

    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    if (p2[0] > XMAX) {
        p2[0] = XMAX;
        p2[1] = p1[1] + (XMAX - p1[0]) * m0;
    }
    p1[1] += (XMIN - p1[0]) * m0;
    p1[0] = XMIN;
    return true;
}

bool Window::p2bottom(Point<2>& p1, Point<2>& p2) {
    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    double m2 = (YMIN - p1[1]) / (XMIN - p1[0]);

    if (m2 >= m0) {
        return false;
    }
    if (p2[0] <= XMAX) {
        p2[0] = p1[0] + (YMIN - p1[1]) / m0;
        p2[1] = YMIN;
    } else {
        double m3 = (YMIN - p1[1]) / (XMAX - p1[0]);
        if (m3 > m0) {
            p2[0] = p1[0] + (YMIN - p1[1]) / m0;
            p2[1] = YMIN;
        } else {
            p2[0] = XMAX;
            p2[1] = p1[1] + (XMAX - p1[0]) * m0;
        }
    }
    p1[1] += (XMIN - p1[0]) * m0;
    p1[0] = XMIN;
    return true;
}

bool Window::centercolumn(Point<2>& p1, Point<2>& p2) {
    if (p1[1] > YMAX) {
        utils::nln_rotate270c(p1);
        utils::nln_rotate270c(p2);
        bool visible = leftedge(p1, p2);
        utils::nln_rotate90c(p1);
        utils::nln_rotate90c(p2);
        return visible;
    }

    if (p1[1] < YMIN) {
        utils::nln_rotate90c(p1);
        utils::nln_rotate90c(p2);
        bool visible = leftedge(p1, p2);
        utils::nln_rotate270c(p1);
        utils::nln_rotate270c(p2);
        return visible;
    }
    return inside(p1, p2);
}

bool Window::inside(Point<2>& p1, Point<2>& p2) {
    if (p2[0] < XMIN) {
        p2left(p1, p2);
    } else if (p2[0] > XMAX) {
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
        p2left(p1, p2);
        utils::nln_rotate180c(p1);
        utils::nln_rotate180c(p2);
    } else if (p2[1] > YMAX) {
        p2[0] = p1[0] + (YMAX - p1[1]) * (p2[0] - p1[0]) / (p2[1] - p1[1]);
        p2[1] = YMAX;
    } else if (p2[1] < YMIN) {
        p2[0] = p1[0] + (YMIN - p1[1]) * (p2[0] - p1[0]) / (p2[1] - p1[1]);
        p2[1] = YMIN;
    }
    return true;
}

void Window::p2left(Point<2>& p1, Point<2>& p2) {
    if (p2[1] > YMAX) {
        p2lefttop(p1, p2);
    } else if (p2[1] < YMIN) {
        utils::nln_rotate90c(p1);
        utils::nln_rotate90c(p2);
        p2lefttop(p1, p2);
        utils::nln_rotate270c(p1);
        utils::nln_rotate270c(p2);
    } else {
        p2[1] = p1[1] + (XMIN - p1[0]) * (p2[1] - p1[1]) / (p2[0] - p1[0]);
        p2[0] = XMIN;
    }
}

void Window::p2lefttop(Point<2>& p1, Point<2>& p2) {
    double m0 = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    double m1 = (YMAX - p1[1]) / (XMIN - p1[0]);

    if (m0 < m1) {
        p2[0] = p1[0] + (YMAX - p1[1]) / m0;
        p2[1] = YMAX;
    } else {
        p2[0] = XMIN;
        p2[1] = p1[1] + (XMIN - p1[0]) * m0;
    }
}