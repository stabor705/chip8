#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ui/disassemblywindow.h"
#include "ui/displaywindow.h"
#include "ui/chipstatewindow.h"
#include "ui/controlswindow.h"
#include "ui/messagingwindow.h"
#include "ui/keyboardwindow.h"
#include "ui/filewindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <exception>

class GlfwError : public std::exception {
public:
    GlfwError() {
        glfwGetError(&msg);
    }

    const char *what() const noexcept override {
        return msg;
    }
private:
    const char *msg;
};

class GladError : public std::exception {
    const char *what() const noexcept override {
        return "Failed to initialize glad";
    }
};

class UserInterface {
public:
    UserInterface();

    bool user_quit() { return glfwWindowShouldClose(window); }
    void show();
    void process_input();

    void run_disassembler(const std::vector<uint8_t> &program);

    void update_display(const Pixels &pixels);
    void update_chip_state(const Chip8 &chip);;
    void add_message(const std::string &msg);
    void chip_halted();

    bool should_run() const { return !controls_window.is_halted(); }
    bool should_step() const { return controls_window.is_halted() && controls_window.should_step(); }
    int get_key_pressed() const { return keyboard_window.get_key(); }
    bool should_reset_chip() { return controls_window.should_reset(); }
    bool file_should_be_loaded() { return file_window.new_file(); }
    fs::path get_file_to_load() const { return file_window.get_path(); }
    void set_file_loaded(const fs::path &filepath) { file_window.update(filepath); }


    ~UserInterface();
private:
    static constexpr char const* glsl_version = "#version 330";

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static void handle_input(GLFWwindow *window, int key, int scancode, int action, int mods);

    void initialize_glfw();
    void initialize_imgui();
    void free_glfw();
    void free_imgui();

    DisassemblyWindow disassembly_window;
    DisplayWindow display_window;
    ChipStateWindow chip_state_window;
    ControlsWindow controls_window;
    MessagingWindow messaging_window;
    KeyboardWindow keyboard_window;
    FileWindow file_window;

    GLFWwindow *window;
    int key_pressed {};
};

#endif // USERINTERFACE_H
