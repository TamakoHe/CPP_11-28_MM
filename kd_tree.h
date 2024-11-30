//
// Created by googl on 24-11-30.
//

#ifndef KD_TREE_H
#define KD_TREE_H
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
using LD=long double;
struct Point {
    LD x{},y{},z{};
    int index_in_sensor{};
};
using T_Point_VEC=std::vector<Point>;
class KdTree {
public:
    struct KDNode {
        Point point;
        KDNode* left;
        KDNode* right;
        explicit KDNode(const Point& point) : point(point), left(nullptr), right(nullptr) {}
    };

private:
    static LD distance(const Point &p1, const Point &p2) {
        return sqrt(pow((p1.x - p2.x),2) +
              pow(abs(p1.y - p2.y),2)  +
              pow(abs(p1.z - p2.z),2));
    }
    static bool compare(const Point &p1, const Point &p2, int depth) {
        if(depth%3==0) {
            return p1.x<p2.x;
        }
        if(depth%3==1) {
            return p1.y<p2.y;
        }
        return p1.z<p2.z;
    }
    KDNode *root{nullptr};
    static KDNode* r_build_tree(std::vector<Point> &points, int depth=0) {
        if(points.empty()) {
            return nullptr;
        }
        int axis=depth%3;
        std::ranges::sort(points, [depth](const Point &p1, const Point &p2) {
            return compare(p1,p2,depth);
        });
        int median=points.size()/2;
        auto *new_node=new KDNode(points[median]);
        std::vector left(points.begin(), points.begin()+median);
        std::vector right(points.begin()+median+1, points.end());
        new_node->left=r_build_tree(left, depth+1);
        new_node->right=r_build_tree(right, depth+1);
        return new_node;
    }
    static void r_find_nearest(KDNode *root, const Point &query, KDNode*& best, LD &bestDist, int depth=0) {
        if (root == nullptr) {
            return;
        }

        // Use squared distance for more precision
        LD dist = distance(query, root->point);
        if (dist < bestDist) {
            bestDist = dist;
            best = root;
        }

        int axis = depth % 3;
        KDNode *next_node = nullptr;
        KDNode *other_node = nullptr;

        // Decide which side to search first
        if ((axis == 0 && query.x < root->point.x) || (axis == 1 && query.y < root->point.y) || (axis == 2 && query.z < root->point.z)) {
            next_node = root->left;
            other_node = root->right;
        } else {
            next_node = root->right;
            other_node = root->left;
        }

        // Explore the next node (recursive search)
        r_find_nearest(next_node, query, best, bestDist, depth + 1);

        // Check if we need to explore the other side
        if ((axis == 0 && abs(query.x - root->point.x) < bestDist) ||
            (axis == 1 && abs(query.y - root->point.y) < bestDist) ||
            (axis == 2 && abs(query.z - root->point.z) < bestDist)) {
            r_find_nearest(other_node, query, best, bestDist, depth + 1);
            }
    }


public:
    void build(std::vector<Point> &points) {
        root = r_build_tree(points);
    }
    struct NearResult {
        Point best_point;
        LD best_dist{};
    };
    [[nodiscard]] NearResult find_nearest(const Point &query) const {
        LD best_dist{std::numeric_limits<LD>::infinity()};
        KDNode *best_node=nullptr;
        r_find_nearest(root, query, best_node, best_dist, 0);
        if(best_node!=nullptr) {
            return {best_node->point, best_dist};
        }
        return {Point(0,0,0),-1};
    }
};


#endif //KD_TREE_H
