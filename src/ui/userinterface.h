#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ui/disassemblywindow.h"
#include "ui/displaywindow.h"
#include "ui/chipstatewindow.h"

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
    void render_frame();
    void process_input();

    void run_disassembler(const std::vector<uint8_t> &program);
    void update_display(const std::array<std::array<bool, DISPLAY_WIDTH>, DISPLAY_HEIGHT> &pixels);
    void update_chip_state(const Chip8 &chip);

    ~UserInterface();
private:
    static constexpr char const* glsl_version = "#version 330";

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void initialize_glfw();
    void initialize_imgui();
    void free_glfw();
    void free_imgui();

    DisassemblyWindow disassembly_window;
    DisplayWindow display_window;
    ChipStateWindow chip_state_window;

    GLFWwindow *window;
};

#endif // USERINTERFACE_H
