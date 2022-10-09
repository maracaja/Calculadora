#ifndef CALCULADORA_HPP
#define CALCULADORA_HPP

#define TAM_DISPLAY 16 // Display da calculadora poderá exibir até 16 dígitos

class Calculadora
{
    public:
        Calculadora(); // Construtor
        double bufferParaNumero(); // Traduz o array de dígitos em um número de fato
        void calcula(); // Realiza operação selecionada
        void colocaMemoria(bool soma); // Soma ou subtrai um valor na memória
        void desliga(); // Desliga a calculadora
        void digita(int n); // Permite digitar o número escolhido na tela
        char* exibicao(); // Converte os dígitos armazenados no buffer para uma string a ser apresentada no display
        void inverteSinal(); // Executa o botão +/-
        bool isError(); // Verifica se a calculadora está em situação de erro ou não
        void limpa(); // Limpa os dados da calculadora, exceto a memória
        void limpaEntrada(); // Limpa o valor digitado na tela, sem cancelar a operação em andamento
        void ponto(); // Permite digitar o ponto decimal
        void porcentagem(); // Realiza operações envolvendo porcentagem
        void raiz(); // Calcula raiz quadrada do número na tela
        void resultMemoria(); // Apresenta o valor armazenado na memória
        void seleciona(char c); // Seleciona operação a ser realizada
        bool temMemoria(); // Verifica se há informação na memória
        void zeraMemoria(); // Limpa o valor armazenado na memória

    private:
        /* Estados possíveis para as situações da calculadora */
        enum class EstadoGeral {OFF, ON};
        enum class EstadoLigado {NORMAL, ERRO};
        enum class Operacao {NADA, SOMA, SUB, MULT, DIV};
        enum class Display {RESULT, DIGIT};
        EstadoGeral lig; // Ligado ou desligado
        EstadoLigado est; // Situação normal ou erro (precisa pressionar ON/C para voltar ao normal)
        Operacao op; // Estado da operação selecionada
        Display disp; // Operação do display (exibe um resultado ou o que está sendo digitado)

        /* Dados para controlar o display */ 
        int bufferDisplay[TAM_DISPLAY]; // Dígitos em posição invertida
        int cont, dec; // Número de dígitos exibidos e Posição do ponto decimal
        bool sinal, pt; // Sinal (se positivo, false); Se ponto já foi digitado ou não

        /* Dados numéricos internos */
        double memoria, acc; // Memória e acumulador

        /* Funções auxiliares */
        bool habilitado(); // Informa se a calculadora está apta a receber comandos
        void resetEntrada(); // Redefine todos os dados referentes à entrada
        void zeraBuffer(); // Realiza a limpeza dos dados do buffer de entrada
};

#endif