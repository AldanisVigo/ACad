/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/

#include "CircuitComponent.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Coil.h"
#include "Transistor.h"

std::shared_ptr<CircuitComponent> CircuitComponent::deserialize(const json& j) {
    std::string type = j.at("type");

    double x = j.at("x");
    double y = j.at("y");
    double w = j.at("width");
    double h = j.at("height");
    double rot = j.at("rotation");

    std::shared_ptr<CircuitComponent> obj;

    if (type == "Resistor") {
        obj = std::make_shared<Resistor>(x, y, w, h);
    } else if (type == "Capacitor") {
        obj = std::make_shared<Capacitor>(x, y, w, h);
    } else if (type == "Coil") {
        obj = std::make_shared<Coil>(x, y, w, h);
    } else if (type == "Transistor") {
        obj = std::make_shared<Transistor>(x, y, w, h);
    } else {
        throw std::runtime_error("Unknown component type: " + type);
    }

    obj->set_rotation(rot);
    return obj;
}
