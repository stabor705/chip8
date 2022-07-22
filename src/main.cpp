#include "emulation.h"

#include <fstream>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    Emulation emulation;
    spdlog::set_level(spdlog::level::debug);
    std::ifstream file(argv[1], std::fstream::binary);
    emulation.load_program(file);
    emulation.run();

    std::this_thread::sleep_for(10s);
    SDL_Quit();
    return 0;
}