#include <gtkmm.h>
#include "ui/CircuitCanvas.h"
#include <iostream>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.vigo.ACad");

    Gtk::Window window;
    window.set_default_size(800, 600);
    window.set_title("ACad");

    // Vertical box to hold menu + canvas
    Gtk::Box vbox(Gtk::ORIENTATION_VERTICAL, 0);

    // --- Menu Bar ---
    Gtk::MenuBar menubar;

    // File menu
    Gtk::MenuItem file_menu_item("_File", true);
    Gtk::Menu file_menu;

    Gtk::MenuItem open_item("_Open", true);
    Gtk::MenuItem save_item("_Save", true);
    
    file_menu.append(open_item);
    file_menu.append(save_item);
    file_menu_item.set_submenu(file_menu);

    menubar.append(file_menu_item);
    menubar.show_all();

    // --- Canvas ---
    CircuitCanvas canvas;

    // Pack menu + canvas into the vbox
    vbox.pack_start(menubar, Gtk::PACK_SHRINK);
    vbox.pack_start(canvas);

    window.add(vbox);


    // Connect signals
    open_item.signal_activate().connect([&]() {
        Gtk::FileChooserDialog dialog(window, "Open File", Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Open", Gtk::RESPONSE_OK);

        if (dialog.run() == Gtk::RESPONSE_OK) {
            std::string filename = dialog.get_filename();
            if (!canvas.load_from_file(filename)) {
                std::cerr << "Failed to load file: " << filename << std::endl;
            } else {
                std::cout << "Loaded " << filename << std::endl;
            }
        }
    });

    save_item.signal_activate().connect([&]() {
        Gtk::FileChooserDialog dialog(window, "Save File", Gtk::FILE_CHOOSER_ACTION_SAVE);
        dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
        dialog.add_button("_Save", Gtk::RESPONSE_OK);
        dialog.set_do_overwrite_confirmation(true);

        if (dialog.run() == Gtk::RESPONSE_OK) {
            std::string filename = dialog.get_filename();
            if (!canvas.save_to_file(filename)) {
                std::cerr << "Failed to save file: " << filename << std::endl;
            } else {
                std::cout << "Saved to " << filename << std::endl;
            }
        }
    });

    window.show_all();

    return app->run(window);
}
