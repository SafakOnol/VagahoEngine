#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>

int main(int argc, char* argv[]) 
{
    // CREATE LUA VIRTUAL MACHINE
    sol::state lua;
    lua.open_libraries(sol::lib::base);


    // Test GLM
    glm::vec2 velocity = glm::vec2(2.0f, -1.0f);

    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    std::cout << "Vagaho Engine is running!" << std::endl;
    return 0;
}
