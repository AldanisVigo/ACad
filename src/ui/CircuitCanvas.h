/*
    Author: Aldanis Vigo <aldanisvigo@gmail.com>
    Date: Fri Nov 21st 2025
*/

#pragma once
#include <vector>
#include <memory>
#include <gtkmm.h>
#include "../core/CircuitComponent.h"
#include "../core/Resistor.h"
#include "../core/Capacitor.h"
#include "../core/Transistor.h"
#include "../core/Coil.h"
#include "../core/Wire.h"

class CircuitCanvas : public Gtk::DrawingArea {
public:
    enum Mode { ComponentMode, WireMode, MoveMode };
    enum ComponentType { ResistorType, CapacitorType, TransistorType, CoilType };
    CircuitCanvas();
    void add_component(std::shared_ptr<CircuitComponent> comp);
    void set_mode(Mode m) { drawing_mode = m; }
    bool save_to_file(const std::string& filename);
    bool load_from_file(const std::string& filename);

protected:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_button_press_event(GdkEventButton* event) override;
    bool on_button_release_event(GdkEventButton* event) override;
    bool on_motion_notify_event(GdkEventMotion* event) override;
    bool on_key_press_event(GdkEventKey* event) override;

private:
    std::shared_ptr<CircuitComponent> get_component_at(double x, double y);
    std::vector<std::shared_ptr<CircuitComponent>> components;
    std::vector<std::shared_ptr<Wire>> wires;
    bool drawing_wire = false;
    std::shared_ptr<Wire> temp_wire;
    Mode drawing_mode = ComponentMode;
    ComponentType current_component = ResistorType;
    double mouse_x = 0;
    double mouse_y = 0;
    std::shared_ptr<CircuitComponent> hovered_component = nullptr;
    std::shared_ptr<Wire> hovered_wire = nullptr;
    std::shared_ptr<CircuitComponent> dragged_component = nullptr;
    double drag_offset_x = 0;
    double drag_offset_y = 0;
    static constexpr int GRID_SIZE = 20;
    double snap_to_grid(double val) { return std::round(val / GRID_SIZE) * GRID_SIZE; }
};
