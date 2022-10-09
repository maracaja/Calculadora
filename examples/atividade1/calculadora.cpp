#include "calculadora.hpp"
#include <core.h>
#include <math.h>
#include <string>
#include <string.h>

using namespace std;

Calculadora::Calculadora()
{
    desliga();
    memoria = 0.0;
}

double Calculadora::bufferParaNumero()
{
    double num {0.0};
    for (int i = cont - 1; i >= 0; i--)
        num += bufferDisplay[i] * pow(10, i - dec);
    return sinal ? -num : num;
}

void Calculadora::calcula()
{
    if (habilitado())
    {
        bool pos = acc > 0;
        double n = disp == Display::DIGIT ? bufferParaNumero() : acc;
        resetEntrada();
        switch (op)
        {
            case Operacao::SOMA: acc += n; break;
            case Operacao::SUB: acc -= n; break;
            case Operacao::MULT:
                acc *= n;
                if ((pos && n > 0 && acc < 0) || (!pos && n < 0 && acc > 0))
                    est = EstadoLigado::ERRO;
                break;
            case Operacao::DIV:
                if (n == 0) est = EstadoLigado::ERRO; // Divisão por zero
                else
                {
                    acc /= n;
                    if ((pos && n > 0 && acc < 0) || (!pos && n < 0 && acc > 0))
                        est = EstadoLigado::ERRO;
                }
                break;
            default: acc = n;
        }
        if (abs(acc) >= pow(10, TAM_DISPLAY)) est = EstadoLigado::ERRO;
        else disp = Display::RESULT;
        op = Operacao::NADA;
    }
}

void Calculadora::colocaMemoria(bool soma)
{
    if (habilitado())
    {
        if (disp == Display::DIGIT)
        {
            if (op != Operacao::NADA) calcula();
            else acc = bufferParaNumero();     
            disp = Display::RESULT;

        }
        memoria += soma ? acc : -acc;
    }
}

void Calculadora::desliga()
{
    lig = EstadoGeral::OFF;
}

void Calculadora::digita(int n)
{
    if (habilitado())
    {
        if (disp == Display::RESULT) 
        {
            disp = Display::DIGIT;
            resetEntrada();
        }
        if (n == 0 && cont == 0) cont++; // Caso em que entra um valor zero pela primeira vez
        else if (cont < TAM_DISPLAY && (((n == 0 && pt) || bufferParaNumero() != 0) || n != 0))
        { // Demais casos, até o limite de dígitos
            for (int i = cont; i > 0; i--)
                bufferDisplay[i] = bufferDisplay[i - 1];
            bufferDisplay[0] = n;
            cont++;
            if (pt) dec++;
        }
    }
}

char* Calculadora::exibicao()
{
    std::string s;
    char *arr;
    if (!habilitado())
    {
        arr = new char[1];
        arr[0] = '\0';
    }
    else
    {
        string s = fmt::format("{:.16}", disp == Display::RESULT ? acc : bufferParaNumero());
        s.append(s.find(".") == string::npos ? "." : "");
        arr = new char[s.length() + 1];
        strcpy(arr, s.c_str());
    }
    return arr;
}

void Calculadora::inverteSinal()
{
    if (habilitado())
    {
        if (disp == Display::DIGIT && bufferParaNumero() != 0) sinal = !sinal;
        else acc = -acc;
    }
}

bool Calculadora::isError()
{
    if (lig == EstadoGeral::OFF) return false;
    else return est == EstadoLigado::ERRO;
}

void Calculadora::limpa()
{
    if (lig == EstadoGeral::OFF) lig = EstadoGeral::ON; // Se estiver desligada, liga a calculadora
    est = EstadoLigado::NORMAL;
    op = Operacao::NADA;
    acc = 0;
    resetEntrada();
}

void Calculadora::limpaEntrada()
{
    if (habilitado()) resetEntrada();
}

void Calculadora::ponto()
{
    if (habilitado() && !pt)
    {
        if (disp == Display::RESULT)
        {
            disp = Display::DIGIT;
            resetEntrada();
        }
        if (cont == 0) cont++;
        pt = true;
    }
}

void Calculadora::porcentagem()
{
    if (habilitado())
    {
        double n = disp == Display::DIGIT ? bufferParaNumero() : acc;
        switch (op)
        {
            case Operacao::SOMA: acc += 0.01 * n * acc; break;
            case Operacao::SUB: acc -= 0.01 * n * acc; break;
            case Operacao::MULT: acc *= n * 0.01; break;
            case Operacao::DIV: acc /= n * 0.01;
            default: break;
        }
        if (op != Operacao::NADA)
        {
            op = Operacao::NADA;
            disp = Display::RESULT;
        }
    }
    
}

void Calculadora::raiz()
{
    if (habilitado())
    {
        if ((disp == Display::DIGIT && sinal) || (disp == Display::RESULT && acc < 0)) 
            est = EstadoLigado::ERRO; // Raiz quadrada de número negativo
        else 
        {
            if (disp == Display::DIGIT) 
            {
                acc = sqrt(bufferParaNumero());
                disp = Display::RESULT;
            }
            else acc = sqrt(acc);
        }
    }    
}

void Calculadora::resultMemoria()
{
    if (habilitado())
    {
        acc = memoria;
        disp = Display::RESULT;
    }   
}

void Calculadora::seleciona(char c)
{
    if (habilitado())
    {
        if (op != Operacao::NADA && disp == Display::DIGIT) calcula();
        switch (c)
        {
            case '+': op = Operacao::SOMA; break;
            case '-': op = Operacao::SUB; break;
            case '*': op = Operacao::MULT; break;
            case '/': op = Operacao::DIV; break;
            default: op = Operacao::NADA;
        }
        if (disp == Display::DIGIT) 
        {
            acc = bufferParaNumero();
            resetEntrada();
            disp = Display::RESULT;
        }
    }    
}

bool Calculadora::temMemoria()
{
    if (lig == EstadoGeral::OFF) return false;
    else return memoria != 0;
}

void Calculadora::zeraMemoria()
{
    if (lig == EstadoGeral::ON) memoria = 0.0;
}

bool Calculadora::habilitado()
{
    return lig == EstadoGeral::ON && !isError();
}

void Calculadora::resetEntrada()
{
    cont = 0;
    dec = 0;
    pt = false;
    sinal = false;
    zeraBuffer();
    disp = Display::DIGIT;
}

void Calculadora::zeraBuffer()
{
    for (int i = 0; i < TAM_DISPLAY; i++) bufferDisplay[i] = 0;
}
