/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/

#pragma once
#include <cairomm/context.h>
#include <cmath>
#include <nlohmann/json.hpp>
#include "../util/Constants.h"

using json = nlohmann::json;

class Wire {
public:
    Wire(double x1, double y1, double x2, double y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {}

    void set_end(double nx, double ny) { x2 = nx; y2 = ny; }

    void draw(const Cairo::RefPtr<Cairo::Context>& cr) {
        cr->set_source_rgb(0, 0, 0);
        cr->set_line_width(2.0);
        cr->move_to(x1, y1);
        cr->line_to(x2, y2);
        cr->stroke();
    }

    bool contains_point(double px, double py) {
        const double buffer = 5.0; // pixels
        double dx = x2 - x1;
        double dy = y2 - y1;
        double length_sq = dx*dx + dy*dy;
        if (length_sq == 0) return std::hypot(px-x1, py-y1) <= buffer;

        double t = ((px - x1)*dx + (py - y1)*dy) / length_sq;
        t = std::max(0.0, std::min(1.0, t));
        double closest_x = x1 + t*dx;
        double closest_y = y1 + t*dy;

        return std::hypot(px - closest_x, py - closest_y) <= buffer;
    }

    std::string get_type() const {
        return "Wire";
    }

    json serialize() const {
        json j;
        j["type"] = "Wire";
        j["x1"] = x1;
        j["y1"] = y1;
        j["x2"] = x2;
        j["y2"] = y2;
        return j;
    }

    static std::shared_ptr<Wire> deserialize(const json& j) {
        double x1 = j.at("x1");
        double y1 = j.at("y1");
        double x2 = j.at("x2");
        double y2 = j.at("y2");
        return std::make_shared<Wire>(x1, y1, x2, y2);
    }

    double get_x1() const { return x1; }
    double get_y1() const { return y1; }
    double get_x2() const { return x2; }
    double get_y2() const { return y2; }

private:
    double x1, y1, x2, y2;
};
