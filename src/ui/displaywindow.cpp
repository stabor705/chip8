#include "displaywindow.h"

#include <iostream>

#include <glad/glad.h>
#include <imgui.h>

OpenGLPixelDisplay::OpenGLPixelDisplay(unsigned int scale) :
res_x(DISPLAY_WIDTH * scale), res_y(DISPLAY_HEIGHT * scale) {}

void OpenGLPixelDisplay::initialize() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    initialize_vertices();
    initialize_indices();
    initialize_rendering();

}

void OpenGLPixelDisplay::clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, res_x, res_y);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLPixelDisplay::fill_pixel(unsigned int y, unsigned int x) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, res_x, res_y);
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    int offset = sizeof(unsigned int) * (y * DISPLAY_WIDTH + x) * 6;
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)offset);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLPixelDisplay::initialize_vertices() {
    glGenBuffers(1, &VBO);
    int v_h = DISPLAY_HEIGHT + 1;
    int v_w = DISPLAY_WIDTH + 1;
    float vertices[v_h * v_w][2];
    for (int y = 0; y < v_h; y++) {
        for (int x = 0; x < v_w; x++) {
            int idx = y * v_w + x;
            vertices[idx][0] = -1.0f + (2.0f / DISPLAY_WIDTH) * x;
            vertices[idx][1] = 1.0f - (2.0f / DISPLAY_HEIGHT) * y;
            std::cout << idx << ": " << vertices[idx][0] << " " << vertices[idx][1]
                      << std::endl;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(0);
}

void OpenGLPixelDisplay::initialize_indices() {
    glGenBuffers(1, &EBO);
    unsigned int indices[DISPLAY_WIDTH * DISPLAY_HEIGHT][6]; // 6 indices for every rectangle
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int idx = y * DISPLAY_WIDTH + x;
            indices[idx][0] = idx; // upper left corner
            indices[idx][1] = idx + 1; // upper right corner
            indices[idx][2] = idx + DISPLAY_WIDTH + 1; // lower left corner
            indices[idx][3] = idx + DISPLAY_WIDTH + 1; // lower left corner
            indices[idx][4]	= idx + DISPLAY_WIDTH + 2; // lower right corner
            indices[idx][5] = idx + 1; // upper right corner
            for (int i = 0; i < 6; i++) {
                std::cout << indices[idx][i] << ", ";
            }
            std::cout << std::endl;
        }
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void OpenGLPixelDisplay::initialize_rendering() {
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader,  1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,  1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res_x, res_y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DisplayWindow::show() {
    ImGui::Begin("Display");
    ImGui::Image((void*)(intptr_t)display.get_texture(), ImVec2(400, 300));
    ImGui::End();
}

void DisplayWindow::update(const std::array<std::array<bool, 64>, 32> &pixels) {
    display.clear();
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (pixels[y][x])
                display.fill_pixel(y, x);
        }
    }
}
