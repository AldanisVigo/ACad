/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/

#pragma once
#include <cairomm/context.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <stdexcept>

using json = nlohmann::json;

class Resistor;
class Capacitor;
class Coil;
class Transistor;

class CircuitComponent {
public:
    CircuitComponent(double x, double y, double w = 40, double h = 20)
        : x(x), y(y), width(w), height(h), rotation(0) {}

    virtual ~CircuitComponent() = default;
    virtual void draw(const Cairo::RefPtr<Cairo::Context>& cr) = 0;
    virtual bool contains_point(double px, double py) = 0;
    virtual std::string get_type() const = 0;
    virtual json serialize() const {
        json j;
        j["type"] = get_type();
        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;
        j["rotation"] = rotation;
        return j;
    }

    void set_rotation(double r) { rotation = r; }
    double get_rotation() const { return rotation; }
    double x, y;
    double width, height;

protected:
    double rotation;

public:
    static std::shared_ptr<CircuitComponent> deserialize(const json& j);
};
