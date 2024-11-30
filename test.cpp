#include <iostream>
#include "kd_tree.h"
#include <vector>
#include <format>
using std::vector;
int main() {
    KdTree kd_tree;
    vector<Point> points = {
        Point(2, 3, 4), Point(5, 4, 2), Point(9, 6, 7),
        Point(4, 7, 9), Point(8, 1, 3), Point(7, 2, 5)
    };
    kd_tree.build(points);
    Point query{5,5,5};
    auto res=kd_tree.find_nearest(query);
    std::cout<<std::format("x:{},y:{},z:{} dist:{}\n",res.best_point.x,res.best_point.y,res.best_point.z, res.best_dist);
    return 0;
}