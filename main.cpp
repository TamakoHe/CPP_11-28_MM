#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include "nlohmann/json.hpp"
#include "kd_tree.h"
using json=nlohmann::json;
using LD=long double;
using T_Point_VEC=std::vector<Point>;
std::vector<T_Point_VEC> read_json(json &data) {
    std::vector<T_Point_VEC> time_stamp_info;
    for(auto &[timestamp_idx, data]:data.items()) {
        auto current_time_x=data["x"].get<std::vector<LD>>();
        auto current_time_y=data["y"].get<std::vector<LD>>();
        auto current_time_z=data["z"].get<std::vector<LD>>();
        std::vector<Point> temp;
        for(size_t sensor_idx=0; sensor_idx<current_time_x.size(); ++sensor_idx) {
            temp.emplace_back(current_time_x[sensor_idx],current_time_y[sensor_idx],current_time_z[sensor_idx],sensor_idx);
        }
        time_stamp_info.emplace_back(temp);
    }
    return time_stamp_info;
}

Point get_center(std::vector<Point> &all_points) {
    LD sum_x=0,sum_y=0,sum_z=0;
    for(auto & all_point : all_points) {
        sum_x+=all_point.x;
        sum_y+=all_point.y;
        sum_z+=all_point.z;
    }
    sum_x/=all_points.size();
    sum_y/=all_points.size();
    sum_z/=all_points.size();
    return {sum_x,sum_y,sum_z, -1};
}
auto get_near(std::vector<Point> &all_points) {
    Point center=get_center(all_points);
    KdTree kd_tree;
    kd_tree.build(all_points);
    auto nearest=kd_tree.find_nearest(center);
    return nearest;
}
int main() {
    std::string json_path=R"(..\dumped_full.json)";
    std::ifstream json_stream(json_path);
    json full_data=json::parse(json_stream);
    auto data=read_json(full_data);
    std::cout<<data.size()<<" "<<data[0].size()<<std::endl;
    json output;
    for(auto &time_stamp:data) {
        auto near_res=get_near(time_stamp);
        std::cout<<std::format("From sensor:{},x:{},y:{},z:{},dist:{}\n", near_res.best_point.index_in_sensor+1, near_res.best_point.x, near_res.best_point.y, near_res.best_point.z,
            near_res.best_dist);

    }
    return 0;
}
