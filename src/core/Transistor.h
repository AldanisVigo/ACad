/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/

#pragma once
#include "CircuitComponent.h"
#include <cmath>
#include <nlohmann/json.hpp>
#include "../util/Constants.h"

using json = nlohmann::json;

class Transistor : public CircuitComponent {
public:
    Transistor(double x, double y, double w = 40, double h = 40)
        : CircuitComponent(x, y, w, h) {}

    void draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        cr->save();
        cr->translate(x + width/2, y + height/2 + GRID_SIZE / 2.0);
        cr->rotate(rotation * M_PI / 180.0);
        cr->translate(-width/2, -height/2);
        cr->set_line_width(2.0);
        cr->set_source_rgb(0, 0, 0); 
        cr->move_to(width/2, 0); cr->line_to(width/2, height/4);     
        cr->move_to(width/2, height); cr->line_to(width/2, 3*height/4); 
        cr->move_to(0, height/2); cr->line_to(width/4, height/2);   
        cr->stroke();
        double center_x = width / 2;
        double center_y = height / 2;
        double radius = std::min(width, height) / 4.0;
        cr->arc(center_x, center_y, radius, 0, 2*M_PI);
        cr->stroke();
        double left_x = center_x - radius;
        double left_y = center_y;
        double top_x = center_x;
        double top_y = center_y - radius;
        double bottom_x = center_x;
        double bottom_y = center_y + radius;
        cr->move_to(left_x, left_y); cr->line_to(top_x, top_y);
        cr->move_to(left_x, left_y); cr->line_to(bottom_x, bottom_y);
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
        return "Transistor";
    }

    json serialize() const override {
        json j;
        j["type"] = "Transistor";
        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;
        j["rotation"] = rotation;
        return j;
    }
};
