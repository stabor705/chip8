#include "userinterface.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <sstream>
#include <functional>

using namespace std::placeholders;

UserInterface::UserInterface() : disassembly_window(0x200) {
    initialize_glfw();
    initialize_imgui();
    display_window.initialize();
}

void UserInterface::render_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    disassembly_window.show();
    display_window.show();
    chip_state_window.show();
    controls_window.show();
    messaging_window.show();

    ImGui::Render();
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

void UserInterface::process_input() {
    glfwPollEvents();
}

void UserInterface::run_disassembler(const std::vector<uint8_t> &program) {
    disassembly_window.load_program(program);
}

void UserInterface::update_display(const Pixels &pixels) {
    display_window.update(pixels);
}

void UserInterface::update_chip_state(const Chip8 &chip) {
    chip_state_window.update(chip);
    disassembly_window.update_pc(chip.get_pc());
}

void UserInterface::add_message(const std::string &msg) {
        messaging_window.add_message(msg);
}

void UserInterface::chip_halted() {
    controls_window.halt();
}

bool UserInterface::should_run_instr() const {
    return !controls_window.is_halted() || controls_window.should_run_next_frame();
}

void UserInterface::handle_input(GLFWwindow *window, int key, int scancode, int action, int mods) {
    UserInterface *ui = (UserInterface *)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS)
        ui->key_pressed = key;
    else if (action == GLFW_RELEASE && key == ui->key_pressed)
        ui->key_pressed = 0;
}

UserInterface::~UserInterface() {
    free_glfw();
    free_imgui();
}

void UserInterface::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void UserInterface::initialize_glfw() {
    if (!glfwInit())
        throw GlfwError();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1024, 512, "chip8 emulator", NULL, NULL);
    if (!window)
        throw GlfwError();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw GladError();
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, handle_input);
}

void UserInterface::initialize_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

void UserInterface::free_glfw() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void UserInterface::free_imgui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
