/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/
#pragma once
#include "CircuitComponent.h"
#include <cmath>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Capacitor : public CircuitComponent {
public:
    Capacitor(double x, double y, double w = 40, double h = 20)
        : CircuitComponent(x, y, w, h) {}

    void draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
        cr->save();
        cr->translate(x + width/2, y + height/2);
        cr->rotate(rotation * M_PI / 180.0);
        cr->translate(-width/2, -height/2);
        cr->set_source_rgb(0.0, 0.0, 0.0);
        cr->set_line_width(3.0);
        double gap = width / 4.0;
        cr->move_to(width/2 - gap, 0);
        cr->line_to(width/2 - gap, height);
        cr->stroke();
        cr->move_to(width/2 + gap, 0);
        cr->line_to(width/2 + gap, height);
        cr->stroke();
        cr->set_line_width(2.0);
        cr->move_to(0, height/2);
        cr->line_to(width/2 - gap, height/2);
        cr->move_to(width/2 + gap, height/2);
        cr->line_to(width, height/2);
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
        return "Capacitor";
    }

    json serialize() const override {
        json j;
        j["type"] = "Capacitor";
        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;
        j["rotation"] = rotation;
        return j;
    }
};
