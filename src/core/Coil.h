/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/

#pragma once
#include "CircuitComponent.h"
#include <cmath>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Coil : public CircuitComponent {
public:
    Coil(double x, double y, double w = 40, double h = 20)
        : CircuitComponent(x, y, w, h) {}

    void draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        cr->save();
        cr->translate(x + width/2, y + height/2);
        cr->rotate(rotation * M_PI / 180.0);
        cr->translate(-width/2, -height/2);
        cr->set_source_rgb(0, 0, 0); 
        int num_loops = 3;
        double loop_radius = height / 4.0;
        double total_width = 2 * num_loops * loop_radius;
        double x_offset = (width - total_width) / 2.0;
        x_offset -= loop_radius / 2.0;
        double center_y = height / 2;
        cr->set_line_width(4.0);
        cr->move_to(-5, center_y);
        cr->line_to(x_offset, center_y);
        cr->stroke();
        cr->set_line_width(2.0);
        double x_pos = x_offset;
        for (int i = 0; i < num_loops; ++i) {
            cr->arc(x_pos + loop_radius, center_y, loop_radius, M_PI, 0);
            x_pos += 2 * loop_radius;
        }
        cr->stroke();
        cr->set_line_width(4.0);
        cr->move_to(x_pos, center_y);
        cr->line_to(width + 5, center_y);
        cr->stroke();
        cr->restore();
    }

    bool contains_point(double px, double py) override {
        double cx = x + width/2;
        double cy = y + height/2;
        double dx = px - cx;
        double dy = py - cy;
        double rad = -rotation * M_PI / 180.0;
        double rx = dx * cos(rad) - dy * sin(rad);
        double ry = dx * sin(rad) + dy * cos(rad);
        return rx >= -width/2 && rx <= width/2 && ry >= -height/2 && ry <= height/2;
    }

    std::string get_type() const override {
        return "Coil";
    }

    json serialize() const override {
        json j;
        j["type"] = "Coil";
        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;
        j["rotation"] = rotation;
        return j;
    }
};
