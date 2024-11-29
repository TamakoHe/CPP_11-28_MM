#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <format>
#include "nlohmann/json.hpp"
using json=nlohmann::json;
struct Point {
    double x,y,z;
};
using T_Point_VEC=std::vector<Point>;
std::vector<T_Point_VEC> read_json(json &data) {
    int sensor_idx=0;
    std::vector<T_Point_VEC> sensors_info;
    for(auto &[key,value]:data.items()) {
        auto this_x=value["x"].get<std::vector<double>>();
        auto this_y=value["y"].get<std::vector<double>>();
        auto this_z=value["z"].get<std::vector<double>>();
        std::vector<Point> this_points;
        for(int i=0;i<this_x.size();i++) {
            this_points.push_back(Point{this_x[i],this_y[i],this_z[i]});
        }
        sensors_info.push_back(this_points);
        std::cout<<std::format("Sensor {} loaded!\n", sensor_idx);
        sensor_idx++;
    }
    return sensors_info;
}
int main() {
    std::string json_path=R"(..\appendix1_full.json)";
    std::ifstream json_stream(json_path);
    json full_data=json::parse(json_stream);
    read_json(full_data);
    return 0;
}
