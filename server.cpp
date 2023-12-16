#include <netput.hpp>
#include <sigfn.hpp>

#include <iostream>

static void server_main()
{
    std::unique_ptr<netput::server> server;
    sigfn::handler_function signal_handler;
    server = std::unique_ptr<netput::server>(new netput::server("0.0.0.0", 12345));

    signal_handler = [&](int signum)
    {
        server->shutdown();
    };

    server->handle_keyboard(
        [&](
            const std::string &session_id,
            uint64_t timestamp,
            uint32_t window_id,
            netput::input_state state,
            bool repeat,
            uint32_t key_code)
        {
            std::string state_string("released");
            if (!repeat)
            {
                if (state == netput::input_state::pressed)
                {
                    state_string = "pressed";
                }
                std::cerr << "key " << key_code << " " << state_string << std::endl;
            }
        });

    server->serve();
}

int main(int arg, char **argv)
{
    int status;

    status = 0;

    try
    {
        server_main();
    }
    catch (const std::exception &exception)
    {
        std::cerr << exception.what() << std::endl;
        status = 1;
    }

    return status;
}