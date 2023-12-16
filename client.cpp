#include <SDL3/SDL.h>
#include <netput.hpp>
#include <sigfn.hpp>

#include <iostream>

static void send_keyboard_event(std::unique_ptr<netput::client> &client, const SDL_KeyboardEvent *keyboard)
{
    const netput::input_state key_state = (keyboard->state == SDL_RELEASED) ? netput::input_state::pressed : netput::input_state::released;
    client->send_keyboard(keyboard->timestamp, keyboard->windowID, key_state, keyboard->repeat > 0, static_cast<uint32_t>(keyboard->keysym.scancode));
}

static void client_main()
{
    std::unique_ptr<netput::client> client;
    sigfn::handler_function signal_handler;
    std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>> window;
    bool loop;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
    {
        throw std::runtime_error("SDL_Init: " + std::string(SDL_GetError()));
    }

    signal_handler = [&](int signum)
    {
        std::cerr << "received signal: " << signum << std::endl;
        loop = false;
    };

    sigfn::handle(SIGINT, signal_handler);
    sigfn::handle(SIGTERM, signal_handler);

    window = std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>>(
        SDL_CreateWindow("netput!", 640, 480, 0),
        [](SDL_Window *window)
        {
            SDL_DestroyWindow(window);
        });

    loop = true;
    while (loop)
    {
        if (SDL_PollEvent(&event) == SDL_TRUE)
        {
            if (event.type == SDL_EVENT_KEY_UP || event.type == SDL_EVENT_KEY_DOWN)
            {
                send_keyboard_event(client, &event.key);
            }

            if (event.type == SDL_EVENT_QUIT)
            {
                std::cerr << "breaking loop." << std::endl;
                loop = false;
            }
        }
    }
}

int main(int arg, char **argv)
{
    int status;

    status = 0;

    try
    {
        client_main();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        status = 1;
    }
    SDL_Quit();
    return status;
}

static void client_main();