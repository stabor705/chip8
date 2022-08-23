#include "userinterface.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <sstream>

UserInterface::UserInterface() : disassembly_window(0x200) {
    initialize_glfw();
    initialize_imgui();
    display_window.initialize();
}

void UserInterface::show() {
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);
    float unit_w = window_width / 32.0f;
    float unit_h = window_height / 22.0f;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImVec2 item_spacing = ImGui::GetStyle().ItemSpacing;
    ImVec2 window_padding = ImGui::GetStyle().WindowPadding;
    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::Begin("Main", NULL, window_flags);

    display_window.show(unit_w * 20, unit_h * 10);
    ImGui::SameLine();
    disassembly_window.show(unit_w * 6, unit_h * 10);
    ImGui::SameLine();
    chip_state_window.show(0, unit_h * 10);
    ImGui::BeginChild("2nd line", ImVec2(unit_w * 32, unit_h * 10));
    ImGui::BeginChild("Input", ImVec2(unit_w * 8, 0), false, ImGuiWindowFlags_NoScrollbar);
    controls_window.show(unit_w * 8, unit_h * 2);
    keyboard_window.show(unit_w * 8, 0);
    ImGui::EndChild();
    ImGui::SameLine();
    messaging_window.show(0, 0);
    ImGui::EndChild();
    file_window.show(unit_w * 32, unit_h * 2 - item_spacing.y * 4);
    ImGui::End();
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
    display_window.update(chip.get_screen());
}

void UserInterface::add_message(const std::string &msg) {
        messaging_window.add_message(msg);
}

void UserInterface::chip_halted() {
    controls_window.halt();
}

//void UserInterface::handle_input(GLFWwindow *window, int key, int scancode, int action, int mods) {
//    UserInterface *ui = (UserInterface *)glfwGetWindowUserPointer(window);
//    if (action == GLFW_PRESS)
//        ui->key_pressed = key;
//    else if (action == GLFW_RELEASE && key == ui->key_pressed)
//        ui->key_pressed = 0;
//}

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
//    glfwSetKeyCallback(window, handle_input);
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
