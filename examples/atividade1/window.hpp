#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "abcgOpenGL.hpp"
#include <array>
#include "calculadora.hpp"

using namespace ImGui;
using namespace std;

class Window : public abcg::OpenGLWindow 
{
    protected:
        void onCreate() override;
        void onPaint() override;
        void onPaintUI() override;

    private:
        array<float, 4> fundo{0.500f, 0.500f, 0.500f, 1.0f};
        ImFont *fonteDisplay, *fonteBotao, *fonteIndicador;
        Calculadora *calc;

        /* Dimensões importantes */
        const float altura {725.0f};
        const float largura {800.0f};
        const float alturaBotao {100.0f};
        const float alinhamentoDireita {675.0f};

        /* Cores utilizadas */
        const ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        const ImU32 azul = ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
        const ImU32 azul_click = ColorConvertFloat4ToU32(ImVec4(0.2f, 0.2f, 1.0f, 1.0f));
        const ImU32 vermelho = ColorConvertFloat4ToU32(ImVec4(0.7f, 0.0f, 0.0f, 1.0f));
        const ImU32 vermelho_click = ColorConvertFloat4ToU32(ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
        const ImU32 verde = ColorConvertFloat4ToU32(ImVec4(0.0f, 0.4f, 0.0f, 1.0f));
        const ImU32 verde_display = ColorConvertFloat4ToU32(ImVec4(0.333f, 0.430f, 0.333f, 1.0f));
        const ImU32 verde_click = ColorConvertFloat4ToU32(ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
        const ImU32 preto = ColorConvertFloat4ToU32(black);
        const ImU32 preto_click = ColorConvertFloat4ToU32(ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); 
        const ImU32 cinza = preto_click;
        const ImU32 cinza_click = ColorConvertFloat4ToU32(ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        const ImU32 magenta = ColorConvertFloat4ToU32(ImVec4(0.5f, 0.0f, 0.7f, 1.0f));
        const ImU32 magenta_click = ColorConvertFloat4ToU32(ImVec4(0.7f, 0.2f, 0.9f, 1.0f));
};

#endif