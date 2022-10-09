#include "window.hpp"
#include "imgui.h"
#include "imgui_internal.h"

/* Criação da janela */
void Window::onCreate()
{
    // Carrega fontes para o display e os botões da calculadora
    auto const disp {abcg::Application::getAssetsPath() + "Seven_Segment.ttf"};
    auto const botao {abcg::Application::getAssetsPath() + "Arial_Black.ttf"};
    fonteDisplay = GetIO().Fonts->AddFontFromFileTTF(disp.c_str(), 72.0f);
    fonteBotao = GetIO().Fonts->AddFontFromFileTTF(botao.c_str(), 72.0f);
    fonteIndicador = GetIO().Fonts->AddFontFromFileTTF(botao.c_str(), 21.0f);
    if (fonteDisplay == nullptr || fonteBotao == nullptr || fonteIndicador == nullptr) 
        throw abcg::RuntimeError{"Arquivo de fonte não pôde ser carregado."};
    calc = new Calculadora(); // Cria o objeto Calculadora
}

/* Plano de fundo */
void Window::onPaint()
{
    abcg::glClearColor(fundo.at(0), fundo.at(1), fundo.at(2), fundo.at(3));
    abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onPaintUI()
{
    abcg::OpenGLWindow::onPaintUI(); // Método herdado da superclasse

    /* Configurações iniciais */
    // Sem barra de rolagem, sem título, com dimensões fixas e menu
    ImGuiWindowFlags wflags = 0;
    wflags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar;

    // Tamanho da janela que corresponde à calculadora
    SetNextWindowSize(ImVec2(largura, altura));
    SetNextWindowPos(ImVec2(0.5 * (getWindowSettings().width - largura), 0.5 * (getWindowSettings().height - altura)));

    /* Janela da aplicação */
    {
        // Início da janela
        Begin("Calculadora", NULL, wflags);
        
        if (BeginMenuBar())
        {   // Menu de configurações (personalizar cor de fundo)
            if (BeginMenu("Configurações"))
            {
                if(BeginMenu("Personalizar cor de fundo"))
                {
                    ColorEdit3("Cor de fundo", fundo.data());
                    EndMenu();
                }   
                EndMenu();
            }
            // Desligar a calculadora
            if (MenuItem("Desligar")) calc->desliga();
            EndMenuBar();
        }

        Spacing();
        SameLine(50.0f); // Elemento que permite centralizar o elemento que contém o display

        // Display da calculadora
        PushStyleColor(ImGuiCol_ChildBg, verde_display);
        if (BeginChild("Display", ImVec2(largura - 100, 136), true, ImGuiWindowFlags_NoScrollbar))
        {
            PushFont(fonteIndicador);
            SetCursorPosX(alinhamentoDireita);
            TextColored(black, calc->temMemoria() ? "M" : ""); // Indicador de memória
            PopFont();

            PushFont(fonteDisplay);
            TextColored(black, "%s", calc->exibicao()); // Número exibido
            PopFont();

            PushFont(fonteIndicador);
            SetCursorPosX(alinhamentoDireita);
            TextColored(black, calc->isError() ? "E" : ""); // Indicador de erro
            PopFont();

            PopStyleColor();
            EndChild();
        }
        
        Spacing();

        /* Botões */
        PushFont(fonteBotao);
        if (BeginTable("Botões", 5))
        {
            Spacing();
            TableNextRow();
            PushStyleColor(ImGuiCol_Button, azul);
            PushStyleColor(ImGuiCol_ButtonHovered, azul);
            PushStyleColor(ImGuiCol_ButtonActive, azul_click);

            // MR
            TableSetColumnIndex(0);
            if (Button("MR", ImVec2(-1, alturaBotao))) calc->resultMemoria();

            // MC
            TableSetColumnIndex(1);
            if (Button("MC", ImVec2(-1, alturaBotao))) calc->zeraMemoria();

            // M+
            TableSetColumnIndex(2);
            if (Button("M+", ImVec2(-1, alturaBotao))) calc->colocaMemoria(true);

            // M-
            TableSetColumnIndex(3);
            if (Button("M-", ImVec2(-1, alturaBotao))) calc->colocaMemoria(false);
            PopStyleColor();

            // ON/C
            TableSetColumnIndex(4);
            PushStyleColor(ImGuiCol_Button, vermelho);
            PushStyleColor(ImGuiCol_ButtonHovered, vermelho);
            PushStyleColor(ImGuiCol_ButtonActive, vermelho_click);
            if (Button("ON/C", ImVec2(-1, alturaBotao))) calc->limpa();
            PopStyleColor();

            Spacing();
            TableNextRow();

            PushStyleColor(ImGuiCol_Button, preto);
            PushStyleColor(ImGuiCol_ButtonHovered, preto);
            PushStyleColor(ImGuiCol_ButtonActive, preto_click);

            // 7
            TableSetColumnIndex(0);
            if (Button("7", ImVec2(-1, alturaBotao))) calc->digita(7);

            // 8
            TableSetColumnIndex(1);
            if (Button("8", ImVec2(-1, alturaBotao))) calc->digita(8);

            // 9
            TableSetColumnIndex(2);
            if (Button("9", ImVec2(-1, alturaBotao))) calc->digita(9);

            PopStyleColor();

            // Divisão
            PushStyleColor(ImGuiCol_Button, cinza);
            PushStyleColor(ImGuiCol_ButtonHovered, cinza);
            PushStyleColor(ImGuiCol_ButtonActive, cinza_click);
            TableSetColumnIndex(3);
            if (Button("÷", ImVec2(-1, alturaBotao))) calc->seleciona('/');
            PopStyleColor();

            // CE
            PushStyleColor(ImGuiCol_Button, magenta);
            PushStyleColor(ImGuiCol_ButtonHovered, magenta);
            PushStyleColor(ImGuiCol_ButtonActive, magenta_click);
            TableSetColumnIndex(4);
            if (Button("CE", ImVec2(-1, alturaBotao))) calc->limpaEntrada();
            PopStyleColor();

            Spacing();
            TableNextRow();

            PushStyleColor(ImGuiCol_Button, preto);
            PushStyleColor(ImGuiCol_ButtonHovered, preto);
            PushStyleColor(ImGuiCol_ButtonActive, preto_click);

            // 4
            TableSetColumnIndex(0);
            if (Button("4", ImVec2(-1, alturaBotao))) calc->digita(4);

            // 5
            TableSetColumnIndex(1);
            if (Button("5", ImVec2(-1, alturaBotao))) calc->digita(5);

            // 6
            TableSetColumnIndex(2);
            if (Button("6", ImVec2(-1, alturaBotao))) calc->digita(6);

            PopStyleColor();
            PushStyleColor(ImGuiCol_Button, cinza);
            PushStyleColor(ImGuiCol_ButtonHovered, cinza);
            PushStyleColor(ImGuiCol_ButtonActive, cinza_click);

            // Multiplicação
            TableSetColumnIndex(3);
            if (Button("×", ImVec2(-1, alturaBotao))) calc->seleciona('*');

            // Raiz quadrada (o caractere '√' não é exibido devidamente)
            TableSetColumnIndex(4);
            if (Button("sqrt", ImVec2(-1, alturaBotao))) calc->raiz();
            
            PopStyleColor();
            Spacing();
            TableNextRow();

            PushStyleColor(ImGuiCol_Button, preto);
            PushStyleColor(ImGuiCol_ButtonHovered, preto);
            PushStyleColor(ImGuiCol_ButtonActive, preto_click);

            // 1
            TableSetColumnIndex(0);
            if (Button("1", ImVec2(-1, alturaBotao))) calc->digita(1);

            // 2
            TableSetColumnIndex(1);
            if (Button("2", ImVec2(-1, alturaBotao))) calc->digita(2);

            // 3
            TableSetColumnIndex(2);
            if (Button("3", ImVec2(-1, alturaBotao))) calc->digita(3);

            PopStyleColor();
            PushStyleColor(ImGuiCol_Button, cinza);
            PushStyleColor(ImGuiCol_ButtonHovered, cinza);
            PushStyleColor(ImGuiCol_ButtonActive, cinza_click);

            // Subtração
            TableSetColumnIndex(3);
            if (Button("-", ImVec2(-1, alturaBotao))) calc->seleciona('-');

            // Porcentagem
            TableSetColumnIndex(4);
            if (Button("%", ImVec2(-1, alturaBotao))) calc->porcentagem();

            Spacing();
            TableNextRow();

            // +/-
            TableSetColumnIndex(0);
            if (Button("+/-", ImVec2(-1, alturaBotao))) calc->inverteSinal();
            PopStyleColor();

            PushStyleColor(ImGuiCol_Button, preto);
            PushStyleColor(ImGuiCol_ButtonHovered, preto);
            PushStyleColor(ImGuiCol_ButtonActive, preto_click);

            // 0
            TableSetColumnIndex(1);
            if (Button("0", ImVec2(-1, alturaBotao))) calc->digita(0);

            // .
            TableSetColumnIndex(2);
            if (Button(".", ImVec2(-1, alturaBotao))) calc->ponto();

            PopStyleColor();

            // Soma
            PushStyleColor(ImGuiCol_Button, cinza);
            PushStyleColor(ImGuiCol_ButtonHovered, cinza);
            PushStyleColor(ImGuiCol_ButtonActive, cinza_click);           
            TableSetColumnIndex(3);
            if (Button("+", ImVec2(-1, alturaBotao))) calc->seleciona('+');
            PopStyleColor();

            // Botão =
            TableSetColumnIndex(4);
            PushStyleColor(ImGuiCol_Button, verde);
            PushStyleColor(ImGuiCol_ButtonHovered, verde);
            PushStyleColor(ImGuiCol_ButtonActive, verde_click);
            if (Button("=", ImVec2(-1, alturaBotao))) calc->calcula();
            PopStyleColor();

            EndTable();
        }
        PopFont();

        // Encerramento
        Spacing();
        End();
    }
}
