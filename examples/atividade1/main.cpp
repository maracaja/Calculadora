#include "window.hpp"

int main(int argc, char **argv)
{
    try
    {
        // Cria instância da aplicação
        abcg::Application app(argc, argv);

        // Cria janela OpenGL (1200 x 750)
        Window window;
        window.setWindowSettings({.width = 1200, .height = 750, .title = "Calculadora"});

        //Roda aplicação
        app.run(window);
    } 
    catch (std::exception const &exception) 
    {
        fmt::print(stderr, "{}\n", exception.what());
        return -1;
    }
    return 0;
}