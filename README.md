# ACad - Simple 2D Circuit CAD Tool / Viewer 

ACad is a lightweight 2D circuit design tool built in **C++** using **GTKmm** for the GUI and **Cairo** for rendering. It allows you to create, move, and connect electronic components such as resistors, capacitors, coils, and transistors, and save/load your circuit designs as JSON files.  

---

## Features

- **Component Placement**
  - Add resistors, capacitors, transistors, and coils.
  - Snap components to a grid for neat layouts.
  
- **Wire Drawing**
  - Connect components with wires.
  - Wires can be selected, hovered, or deleted.

- **Move Mode**
  - Drag and reposition components.
  - Wires automatically remain connected to endpoints.

- **Serialization**
  - Save and load designs in JSON format.
  - Both components and wires are preserved.

- **Keyboard Shortcuts & Mouse Interaction**
  - Click to place or select components.
  - Drag to move components.
  - Intuitive GUI navigation.

---

## Installation

### Prerequisites

- **C++17** or later
- **GTKmm 3** (GTK+ C++ bindings)
- **Cairo / cairomm**
- **nlohmann/json** (for JSON serialization)

On macOS, you can install dependencies via **Homebrew**:

```bash
brew install gtkmm3 cairomm nlohmann-json
