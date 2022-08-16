#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <array>

constexpr unsigned int DISPLAY_WIDTH = 64;
constexpr unsigned int DISPLAY_HEIGHT = 32;
typedef std::array<std::array<bool, 64>, 32> Pixels;

class OpenGLPixelDisplay {
public:
    OpenGLPixelDisplay(unsigned int scale);
    void initialize();

    void clear();
    void fill_pixel(unsigned int y, unsigned int x);
    unsigned int get_texture() { return texture; }

private:
    const char* vertex_shader_source = "#version 330 core\n"
                                       "layout (location = 0) in vec2 aPos;\n"
                                       "void main() {\n"
                                       "gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
                                       "}\n";
    const char* fragment_shader_source= "#version 330 core\n"
                                        "out vec4 FragColor;\n"
                                        "void main() {\n"
                                        "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
                                        "}\n";
    void initialize_vertices();
    void initialize_indices();
    void initialize_rendering();

    unsigned int VAO, VBO, EBO;
    unsigned int framebuffer;
    unsigned int texture;
    unsigned int res_x, res_y;
    unsigned int shader_program;
};

class DisplayWindow {
public:
    DisplayWindow() : display(10) {}
    void initialize() { display.initialize(); }

    void show();
    void update(const Pixels &pixels);
private:
    OpenGLPixelDisplay display;
};

#endif // DISPLAYWINDOW_H
