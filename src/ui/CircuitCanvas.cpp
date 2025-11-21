/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/

#include <fstream>   
#include <nlohmann/json.hpp>
#include "CircuitCanvas.h"
#include <cairomm/context.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "../util/Constants.h"


CircuitCanvas::CircuitCanvas()
: drawing_wire(false), drawing_mode(ComponentMode), current_component(ResistorType)
{
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK |
               Gdk::POINTER_MOTION_MASK | Gdk::KEY_PRESS_MASK);

    set_can_focus(true);
    grab_focus();
}

void CircuitCanvas::add_component(std::shared_ptr<CircuitComponent> comp) {
    components.push_back(comp);
    queue_draw();
}

inline double snap_to_grid(double val) {
    return GRID_SIZE * std::round(val / GRID_SIZE);
}

inline double snap_to_grid_center(double val, double size) {
    return GRID_SIZE * std::round(val / GRID_SIZE) - size / 2.0;
}

bool CircuitCanvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    cr->set_source_rgb(0.9, 0.9, 0.9);
    cr->set_line_width(1.0);
    for(int gx = 0; gx < width; gx += GRID_SIZE) {
        cr->move_to(gx, 0);
        cr->line_to(gx, height);
    }
    for(int gy = 0; gy < height; gy += GRID_SIZE) {
        cr->move_to(0, gy);
        cr->line_to(width, gy);
    }
    cr->stroke();

    if (hovered_component) {
        cr->set_source_rgba(1, 0, 0, 0.3);

        double draw_x = hovered_component->x;
        double draw_y = hovered_component->y;

        if (std::dynamic_pointer_cast<Transistor>(hovered_component))
            draw_y += GRID_SIZE / 2.0;

        cr->rectangle(draw_x, draw_y,
                    hovered_component->width, hovered_component->height);
        cr->fill();
    }


    if (hovered_wire) {
        cr->set_source_rgba(1, 0, 0, 0.3);
        cr->set_line_width(3.0);
        cr->move_to(hovered_wire->get_x1(), hovered_wire->get_y1());
        cr->line_to(hovered_wire->get_x2(), hovered_wire->get_y2());
        cr->stroke();
    }

    for(auto& wire : wires)
        wire->draw(cr);

    for(auto& comp : components)
        comp->draw(cr);

    if(drawing_wire && temp_wire)
        temp_wire->draw(cr);

    std::ostringstream label;
    switch(drawing_mode) {
        case WireMode: label << "Wire Mode"; break;
        case ComponentMode:
            label << (current_component == ResistorType ? "Resistor Mode" :
                current_component == CapacitorType ? "Capacitor Mode" :
                current_component == TransistorType ? "Transistor Mode" :
                current_component == CoilType ? "Coil Mode" :
                "Unknown");
            break;
        case MoveMode: label << "Move Mode"; break;
    }
    cr->set_source_rgb(0, 0, 0);
    cr->move_to(mouse_x + 10, mouse_y + 10);
    cr->show_text(label.str());
    cr->stroke();

    return true;
}

bool CircuitCanvas::on_button_press_event(GdkEventButton* event) {
    if(event->button != 1) return false;

    if(drawing_mode == ComponentMode) {
        double comp_width = 40;
        double comp_height = 20;
        double x = snap_to_grid_center(event->x, comp_width);
        double y = snap_to_grid_center(event->y, comp_height);

        switch (current_component) {
            case ResistorType:
                components.push_back(std::make_shared<Resistor>(x, y));
                break;
            case CapacitorType:
                components.push_back(std::make_shared<Capacitor>(x, y));
                break;
            case TransistorType:
                components.push_back(std::make_shared<Transistor>(x, y));
                break;
            case CoilType:
                components.push_back(std::make_shared<Coil>(x, y));
                break;
            default:
                break;
        }

        queue_draw();
    }
    else if(drawing_mode == WireMode) {
        double x = snap_to_grid(event->x);
        double y = snap_to_grid(event->y);
        drawing_wire = true;
        temp_wire = std::make_shared<Wire>(x, y, x, y);
    } 
    else if(drawing_mode == MoveMode) {
        dragged_component = get_component_at(event->x, event->y);
        if(dragged_component) {
            drag_offset_x = event->x - (dragged_component->x + dragged_component->width/2.0);
            drag_offset_y = event->y - (dragged_component->y + dragged_component->height/2.0);
        }
    }
    return true;
}

bool CircuitCanvas::on_motion_notify_event(GdkEventMotion* event) {
    mouse_x = event->x;
    mouse_y = event->y;

    hovered_component = get_component_at(mouse_x, mouse_y);

    hovered_wire = nullptr;
    if (!drawing_wire) {
        for (auto it = wires.rbegin(); it != wires.rend(); ++it) {
            if ((*it)->contains_point(mouse_x, mouse_y)) {
                hovered_wire = *it;
                break;
            }
        }
    }

    if(drawing_wire && temp_wire) {
        temp_wire->set_end(snap_to_grid(event->x), snap_to_grid(event->y));
    }

    if(drawing_mode == MoveMode && dragged_component) {
        double new_center_x = event->x - drag_offset_x;
        double new_center_y = event->y - drag_offset_y;

        double snapped_x = snap_to_grid(new_center_x) - dragged_component->width/2.0;
        double snapped_y = snap_to_grid(new_center_y) - dragged_component->height/2.0;

        if (std::dynamic_pointer_cast<Transistor>(dragged_component))
            snapped_y += GRID_SIZE / 2.0;

        dragged_component->x = snapped_x;
        dragged_component->y = snapped_y;
    }

    queue_draw();
    return true;
}

bool CircuitCanvas::on_button_release_event(GdkEventButton* event) {
    if(drawing_wire && temp_wire && event->button == 1) {
        temp_wire->set_end(snap_to_grid(event->x), snap_to_grid(event->y));
        wires.push_back(temp_wire);
        temp_wire = nullptr;
        drawing_wire = false;
        queue_draw();
    }

    if(drawing_mode == MoveMode && dragged_component) {
        dragged_component = nullptr;
    }

    return true;
}

bool CircuitCanvas::on_key_press_event(GdkEventKey* event) {
    switch(event->keyval) {
        case GDK_KEY_w: case GDK_KEY_W:
            drawing_mode = WireMode;
            std::cout << "Wire Mode\n"; 
            break;

        case GDK_KEY_r: case GDK_KEY_R:
            if(hovered_component) {
                double new_rotation = hovered_component->get_rotation() + 90.0;
                if(new_rotation >= 360.0) new_rotation -= 360.0;
                hovered_component->set_rotation(new_rotation);
            } else {
                drawing_mode = ComponentMode;
                current_component = ResistorType;
                std::cout << "Resistor Mode\n";
            }
            break;

        case GDK_KEY_c: case GDK_KEY_C:
            drawing_mode = ComponentMode;
            current_component = CapacitorType;
            std::cout << "Capacitor Mode\n"; 
            break;

        case GDK_KEY_t: case GDK_KEY_T:
            drawing_mode = ComponentMode;
            current_component = TransistorType;
            std::cout << "Transistor Mode\n";
            break;
        
        case GDK_KEY_l: case GDK_KEY_L:  
            drawing_mode = ComponentMode;
            current_component = CoilType;
            std::cout << "Coil Mode\n";
            break;

        case GDK_KEY_m: case GDK_KEY_M:
            drawing_mode = MoveMode;
            std::cout << "Move Mode\n";
            break;

        case GDK_KEY_Delete: case GDK_KEY_BackSpace:
            if (hovered_component) {
                components.erase(std::remove(components.begin(), components.end(), hovered_component), components.end());
                hovered_component = nullptr;
                std::cout << "Component deleted\n";
            } else if (hovered_wire) {
                wires.erase(std::remove(wires.begin(), wires.end(), hovered_wire), wires.end());
                hovered_wire = nullptr;
                std::cout << "Wire deleted\n";
            }
            break;
    }
    queue_draw();
    return true;
}


std::shared_ptr<CircuitComponent> CircuitCanvas::get_component_at(double x, double y) {
    for (auto it = components.rbegin(); it != components.rend(); ++it) {
        if ((*it)->contains_point(x, y)) {
            return *it;
        }
    }
    return nullptr;
}

bool CircuitCanvas::save_to_file(const std::string& filename) {
    try {
        nlohmann::json j;
        j["components"] = nlohmann::json::array();
        for (const auto& comp : components) {
            j["components"].push_back(comp->serialize());
        }

        j["wires"] = nlohmann::json::array();
        for (const auto& wire : wires) {
            j["wires"].push_back(wire->serialize());
        }

        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << j.dump(4);  
        return true;
    } catch (...) {
        return false;
    }
}

bool CircuitCanvas::load_from_file(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        nlohmann::json j;
        file >> j;

        components.clear();
        wires.clear();

        for (const auto& jc : j["components"]) {
            components.push_back(CircuitComponent::deserialize(jc));
        }

        for (const auto& jw : j["wires"]) {
            wires.push_back(Wire::deserialize(jw));
        }

        queue_draw();  
        return true;
    } catch (...) {
        return false;
    }
}

