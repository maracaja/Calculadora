# Atividade 1 - Calculadora
#### Autor: Sérgio Maracajá Junior - RA 11000315

* * *

## Sobre a aplicação

A presente aplicação simula o funcionamento de uma calculadora comum, com um limite de 16 dígitos e todas as suas funcionalidades básicas. As operações suportadas por esta calculadora são:

-   As quatro operações básicas (soma, subtração, multiplicação e divisão), permitindo novas operações diretamente sobre o resultado calculado (por exemplo, somar vários números um após o outro);
-   Raiz quadrada (calcula imediatamente sobre o número exibido na tela);
-   Cálculos com porcentagem (com as quatro operações básicas, tal como numa calculadora convencional);
-   Inversão de sinal do valor em tela;
-   Operações com memória (incluindo exibição do resultado e apagamento de seu valor);
-   Distinção entre C (que limpa tudo, exceto a memória) e CE (que limpa apenas o dado que está sendo inserido);
-   Menu superior que permite mudar a cor de plano de fundo (para a aplicação desktop) e simular o desligamento da calculadora.

Também estão inclusos indicadores no canto direito, um informando se há dados em memória e outro indicando um erro aritmético ou de estouro do valor máximo suportado.

* * *

## Implementação

Segue abaixo a descrição do código-fonte, contido na pasta do projeto como mais um exemplo na pasta `examples`, sob o título `atividade1`. Além dos arquivos C++ que serão descritos mais adiante, foram realizadas as seguintes adições ao projeto:

-   Em `examples/CMakeLists.txt`, foi adicionada a instrução `add_subdirectory(atividade1)`, sendo comentada a instrução original;
-   Em `public`, além dos arquivos gerados pela compilação do WASM, a página `helloworld.html` foi modificada para que o canvas possa conter toda a janela da aplicação, configurada para 800 x 725 (linhas 48-49). Também foi modificada a linha 258 para que a página execute o arquivo `atividade1.js`;
-   A pasta `docs` foi criada com os arquivos compilados pelo `build-wasm` e o `helloworld.html`, visando a criação do site no GitHub Pages;
-   Na pasta `atividade1`:
    -   Em `CMakeLists.txt`, foi definido o nome do projeto na linha 1, enquanto na linha 2, foi adicionado o arquivo `calculadora.cpp`;
    -   Em `assets`, foram adicionados os arquivos de fonte para Arial Black e formato de 7 segmentos (`Seven_Segment.ttf`).


### main.cpp

Este arquivo acabou por ser, na prática, uma repetição do código apresentado no Jogo da Velha, com o método `main` instanciando a aplicação (linha 8), gerando a janela onde a aplicação reside (linhas 11-12), e chamando seu método `run` (linha 15), tudo isso dentro de uma estrutura `try-catch`.


### window.hpp

Assim como no Jogo da Velha e outros exemplos de aula, este arquivo tem a definição da classe `Window`, com as inserções pertinentes a esta aplicação:

-   Nas linhas 8 e 9, foram incluídas as diretivas `using namespace` para ImGui e `std`, visando facilitar a escrita e a visualização do código;
-   Entre as linhas 14 e 16, são declarados os protótipos dos métodos a serem sobrescritos `onCreate()`, `onPaint()` e `onPaintUI()`;
-   Entre as linhas 19 e 21, são declaradas (já como variáveis globais privadas):
    -   O array `fundo` armazena a cor de fundo, definifa inicialmente com um cinza médio (linha 19);
    -   Na linha 20, os objetos `fonteDisplay`, `fonteBotao` e `fonteIndicador` armazenam as definições de fonte para o display numérico, para as teclas da calculadora e para os indicadores de erro e memória, respectivamente;
    -   Na linha 21, é instanciado o objeto `calc`, que encapsula a lógica da calculadora.
-   Entre as linhas 24 e 27 são definidas constantes que definem dimensões importantes (altura e largura da calculadora, altura dos respectivos botões e posição dos indicadores de memória e erro);
-   Entre as linhas 30 e 43, são definidas as constantes que definem as cores utilizadas para colorir diferentes elementos da aplicação.


### calculadora.hpp

Este arquivo de cabeçalho define as funções e variáveis que regem o funcionamento da calculadora, e que mediam a interação entre seu comportamento interno e os elementos gráficos, por meio do objeto criado no início da aplicação, conforme instanciado em `window.hpp`.

-   Já na linha 4, é definido `TAM_DISPLAY`, que informa o máximo de caracteres numéricos suportados pelo display (assim como ocorreria em uma calculadora física qualquer). Na linha 6, é iniciada a declaração da classe `Calculadora`;
-   Entre as linhas 9 e 26, são declarados todos os métodos públicos (conforme destaque abaixo), inclusive o construtor, acompanhados dos comentários que resumem seu funcionamento. O funcionamento de cada um deles será descrito de forma pormenorizada no tópico referente a `calculadora.cpp`:
```C++
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
```
-   A partir da linha 28, são declarados os demais elementos, todos privados;
-   Entre as linhas 30 e 33, são definidos os `enum` que definem diferentes estados que ajudam a gerenciar o comportamento interno da calculadora; as variáveis que armazenam estes estados estão declaradas entre as linhas 34 e 37:
    -   `EstadoGeral` (linhas 30 e 34) informa se a calculadora está ligada (`ON`) ou desligada (`OFF`). Quando ela está desligada, não responde a nenhum comando, exceto o de ligar;
    -   `EstadoLigado` (linhas 31 e 35) informa se a calculadora está operando normalmente (`NORMAL`) ou se houve um erro (`ERRO`). Em caso de erro, a aplicação somente responde aos comandos de limpar tudo (via tecla **ON/C**) ou de desligar;
    -   `Operacao` (linhas 32 e 36) define qual operação foi selecionada (para ser calculada ao pressionar **=** ou em caso de operações subsequentes, bem como **%** ou inserção na memória). Como porcentagem e raiz quadrada são calculadas imediatamente ao serem pressionados seus botões, são armazenadas como estados as quatro operações fundamentais (representadas por `SOMA`, `SUB`, `MULT` e `DIV`). O estado `NADA` informa que nenhuma operação foi previamente escolhida pelo usuário;
    -   `Display` (linhas 33 e 37) informa a diversos métodos se o que está sendo exibido naquele momento é um valor resultante de uma operação anterior (modo `RESULT`) ou se é um valor que está sendo digitado pelo usuário (modo `DIGIT`).
-   Entre as linhas 40 e 42, são declaradas variáveis que controlam o comportamento do display quando está ocorrendo a inserção de um valor:
    -   `bufferDisplay` (linha 40) é um vetor que armazena os dígitos inseridos pelo usuário como números inteiros individuais, com o seu tamanho máximo definido por `TAM_DISPLAY`. O dígito menos significativo está na posição **0**;
    -   Na linha 41, são declarados os inteiros `cont`, que armazena o número de dígitos já inseridos pelo usuário, e `dec`, que informa em qual posição está o dígito das unidades (e após o qual aparecerá o separador decimal);
    -   Na linha 42, são declarados os booleanos `sinal`, que armazena se é um valor negativo — caso seja `true` e `pt`, que armazena se o ponto foi ou não digitado (permitindo a inserção e exibição corretas dos números decimais).
-   Na linha 45, são declaradas duas variáveis de tipo `double`: `memoria` armazena a memória acessível ao usuário, ao passo que `acc` é o acumulador utilizado para realizar os cálculos;
-   Entre as linhas 48 e 50, são declarados métodos que são usados internamente. Também acompanham comentários que resumem seu funcionamento e serão melhor detalhados no tópico referente ao arquivo `calculadora.cpp`.


### window.cpp

Neste arquivo, são implementados os métodos públicos descritos em `window.hpp`.

#### onCreate() — linhas 6 a 17

-   Nas linhas 9 e 10, são carregados os arquivos de fonte, seguindo o exempĺo do Jogo da Velha;
-   Entre as linhas 11 e 13, são definidos os três formatos de fontes utilizados — para o display, Seven Segments, tamanho 72; para os indicadores no canto do display, Arial Black, 12; para os botões, Arial Black, 72;
-   Caso um dos três ponteiros acima resulte em referência nula, é lançada uma exceção (linhas 14 e 15);
-   Se nada der problema, a instância de Calculadora `calc` é criada (linha 16).

#### onPaint() — linhas 20 a 24

Neste método, foi apenas repetido o procedimento de aulas anteriores, preenchendo o plano de fundo.

#### onPaintUI() — linhas 26 a 269

-   Este método tem, na linha 28, a chamada para o método `onPaintUI()` da biblioteca `abcg`;
-   Na linha 32, é criada a variável `wflags`, que é alterada, na linha seguinte, para configurar a janela que conterá o layout da calculadora: foi habilitada a barra de menus, e removidas a possibilidade de redimensionamento, a barra de título e a barra de rolagem;
-   Na linha 36, são configuradas as dimensões da janela, com os valores definidos em `window.hpp`;
-   Na linha 37, a posição da calculadora é calculada de forma que ocupe o centro da janela tanto na vertical quanto na horizontal;
-   Na linha 42, é iniciado o código da janela que contém a calculadora, carregando as configurações definidas para `wflags`;
-   Entre as linhas 44 e 58, há a definição da barra de menus:
    -   Na linha 46, é criado o menu "Configurações", que tem em seu interior o submenu "Personalizar cor de fundo" (criado na linha 48). Este submenu abre um editor de cor definido na linha 50 e é encerrado na linha 51;
    -   Na linha 56, é criado um segundo item no menu com a opção desligar. Este, ao ser clicado, invoca o método `desliga()` do objeto `calc`.
    -   Na linha 57, é chamado o método que finaliza a construção do menu
-   É colocado um espaçamento entre o menu e o próximo elemento (linha 60);
-   Um elemento sem conteúdo e com largura de 50 pixels é colocado de forma a centralizar o próximo elemento gráfico, que contém o display (que tem 100 pixels de largura a menos que a janela);
-   Na linha 64, é definida a cor de fundo do elemento do tipo `Child`, que conterá o display;
-   Entre as linhas 65 e 83, é definido um elemento `Child`, com 100 px a menos de largura que a janela e 136 px de altura. Foi também configurada a ausência de barra de rolagem lateral, por meio de `ImGuiWindowFlags_NoScrollbar` (linha 65).
    -   Entre as linhas 67 e 70, há a definição do elemento de texto que conterá uma indicação de há memória ou não:
        -   Na linha 67, é configurada a fonte Arial Black, 12 (segundo a linha 13);
        -   Na linha 68, é definido o alinhamento à direita do texto (segundo `window.hpp`);
        -   Na linha 69, é criado o elemento de texto propriamente dito, com o texto preto, e exibindo **M** se há dados na memória (ou seja, `temMemoria()` retorna verdadeiro) ou uma string vazia, caso contrário;
        -   Na linha 70, encerra-se o uso da fonte iniciado na linha 67.
    -   Entre as linhas 72 e 74, define-se o elemento de texto que conterá o número exibido em tela para o usuário:
        -   Na linha 72, é configurada a fonte Seven Segments, 72 (segundo a linha 11);
        -   Na linha 73, é criado o elemento de texto propriamente dito, com o texto preto, e exibindo o valor numérico fornecido por `exibicao()` do objeto calculadora;
        -   Na linha 74, encerra-se o uso da fonte iniciado na linha 72.
    -   Entre as linhas 76 e 79, há quase uma repetição do feito entre as linhas 67 e 70. A exceção é a penúltima linha desse conjunto, onde o elemento exibe **E** se `isError()` retorna `true`, ou uma string vazia, caso contrário
    -   Na linha 81, é finalizado o estilo de cor definido na linha 64. Na linha 82, o elemento `Child` do display é encerrado.
-   É criado mais um espaçamento entre o último elemento e o próximo na linha 85;
-   Na linha 88, é configurada a fonte Arial Black, 72 (conforme linha 12) para todos os elementos que seguirão — o método `PopFont()` só é chamado na linha 264.
A partir daqui, usou-se o padrão de criação de botões usado no exemplo do Jogo da Velha. Porém, devido às diferenças entre textos, cores e funções, que mudam a cada botão, cada botão é implementado individualmente. Algumas características, porém, são comuns:
-   Os botões estão dispostos em uma tabela (a partir do método `BeginTable` chamado na linha 89), que possui 5 colunas, e também terá 5 linhas. A construção desta tabela, com todos os seus elementos gráficos, é finalizada na linha 262 (`EndTable()`);
-   Para cada botão, é definida a devida coluna por meio do método `TableSetColumnIndex`, cujo parâmetro assumiria um valor de 0 a 4, dependendo da sua posição no layout;
-   Os estilos de cores definidos por `PushStyleColor` (e finalizados ao fim de cada conjunto de botões por `PopStyleColor()`) definem sempre que a cor original do botão sempre será alterada para um tom mais claro se clicado (segundo a flag `ImGuiCol_ButtonActive`), mas não se altera caso o mouse seja passado por cima do botão (os estilos com as flags `ImGuiCol_Button` e `ImGuiCol_ButtonHovered` possuem a mesma cor em todos os casos);
-   Todos os botões são dimensionados com o mesmo parâmetro de dimensões `ImVec2(-1, alturaBotao)`, sinalizando que todos os botões dividirão igualmente a largura disponível e têm a mesma altura de 100 px (conforme definido em `window.hpp`);
-   Antes de cada nova linha (método `TableNextRow()` chamado nas linhas 92, 123, 160, 193 e 225), é colocado um espaçamento (método `Spacing()` aplicado às linhas 91, 122, 159, 192 e 224).
Além das considerações elencadas acima, os botões da calculadora são implementados como segue:
-   Entre as linhas 93 e 95, aplica-se a cor azul para os próximos 4 botões (`PopStyleColor()` é aplicado na linha 112):
    -   Nas linhas 98 e 99, é criado o botão **MR** que, ao ser clicado, aciona o método `resultMemoria()`;
    -   Nas linhas 102 e 103, é criado o botão **MC** que, ao ser clicado, aciona o método `zeraMemoria()`;
    -   Nas linhas 106 e 107, é criado o botão **M+** que, ao ser clicado, aciona o método `colocaMemoria`, passando `true` como parâmetro, somando o valor em tela à memória;
    -   Nas linhas 110 e 111, é criado o botão **M-** que, ao ser clicado, aciona o método `colocaMemoria`, passando `false` como parâmetro, sdubtraindo o valor em tela à memória;
-   Entre as linhas 115 e 120, é criado o botão **ON/C**, que chama a função `limpa()` da calculadora, responsável pela redefinição dos dados ou do acionamento do dispositivo simulado. Somente este botão foi configurado com a cor vermelha;
-   Entre as linhas 125 e 127, aplica-se a cor preta para os próximos 3 botões (`PopStyleColor()` é aplicado na linha 141):
    -   Nas linhas 130 e 131, é criado o botão **7**, que aciona o método `digita` passando este número como parâmetro. O mesmo se repete nas linhas 134 e 135 para **8** e nas linhas 138 e 139 para o valor **9**;
-   Entre as linhas 144 e 149, é criado o botão **÷**, que aplica o método `seleciona` passando o caractere '/' como parâmetro, para executar a operação de divisão. Este botão tem a cor cinza;
-   Entre as linhas 152 e 157, temos a definição do botão **CE**, que aplica o método `limpaEntrada()`. Somente este botão foi configurado com uma cor próxima a magenta;
-   Entre as linhas 162 e 164, aplica-se a cor preta para os próximos 3 botões (`PopStyleColor()` é aplicado na linha 178):
    -   Nas linhas 167 e 168, é criado o botão **4**, que aciona o método `digita` passando este número como parâmetro. O mesmo se repete nas linhas 171 e 172 para **5** e nas linhas 175 e 176 para o valor **6**;
-   Entre as linhas 179 e 181, aplica-se a cor cinza para os próximos 2 botões (`PopStyleColor()` é aplicado na linha 191):
    -   Nas linhas 184 e 185, é criado o botão **×**, que aplica o método `seleciona` passando o caractere '*' como parâmetro, para executar a operação de multiplicação;
    -   Nas linhas 188 e 189, é criado o botão **sqrt**, que aplica o método `raiz()` para o cálculo da raiz quadrada do valor em tela. Foi necessário representar o botão com esse texto pois o caractere '√' não é suportado pelo ImGui, que coloca um "?" em seu lugar;
-   Entre as linhas 195 e 197, aplica-se a cor preta para os próximos 3 botões (`PopStyleColor()` é aplicado na linha 211):
    -   Nas linhas 200 e 201, é criado o botão **1**, que aciona o método `digita` passando este número como parâmetro. O mesmo se repete nas linhas 204 e 205 para **2** e nas linhas 208 e 209 para o valor **3**;
-   Entre as linhas 212 e 214, aplica-se a cor cinza para os próximos 3 botões (`PopStyleColor()` é aplicado na linha 230):
    -   Nas linhas 217 e 218, é criado o botão **-**, que aplica o método `seleciona` passando o caractere '-' como parâmetro, para executar a operação de subtração;
    -   Nas linhas 221 e 222, é criado o botão **%**, que aplica o método `porcentagem()`;
    -   Nas linhas 228 e 229, é criado o botão **+/-**, que aplica o método `inverteSinal()`;
-   Entre as linhas 232 e 234, aplica-se a cor preta para os próximos 2 botões (`PopStyleColor()` é aplicado na linha 244):
    -   Nas linhas 237 e 238, é criado o botão **0**, que aciona o método `digita` passando este número como parâmetro;
    -   Nas linhas 241 e 242, é criado o botão **.**, que aciona o método `ponto()`, que habilita a digitação do ponto decimal;
-   Entre as linhas 247 e 252, é definido o botão **+**, que aplica o método `seleciona` passando o caractere '+' como parâmetro, para executar a operação de soma. Este botão tem a cor cinza;
-   Entre as linhas 255 e 260, é criado o botão **=**, que aplica o método `calcula()`. Somente este botão tem a cor verde;
- O método se encerra com mais um espaçamento (linha 267) e uma chamada ao método `End()`, finalizando a construção da janela.


### calculadora.cpp

Este arquivo possui a implementação dos métodos listados no item `calculadora.hpp`, que será detalhada nesta seção. Para a simplicidade do código, a diretiva `using namespace std` é incluída na linha 7.

#### Métodos privados

Apesar de estarem no fim do arquivo, estes trechos de código serão descritos antes, pois são utilizados em muitos dos métodos públicos.

##### habilitado() — linhas 240 a 243

Este método retorna um booleano que informa se a calculadora está ligada e não está em estado de erro (linha 242). Nos métodos onde é chamado, a simulação não responderá aos comandos dos botões caso uma dessas situações ocorra.

##### resetEntrada() — linhas 245 a 253

Este método redefine a entrada do usuário para os valores iniciais:
-   `cont` com o valor 0 indica que nenhum algarismo foi inserido (linha 247);
-   `dec` com o valor 0 indica que a casa das unidades está no último algarismo apresentado na tela (linha 248);
-   `pt` com o valor falso indica que o ponto decimal ainda não foi digitado (linha 249);
-   `sinal` com o valor falso (linha 250) indica que o sinal não foi digitado e o valor inicial é não-negativo (esperado para um valor inicial igual a zero);
-   `zeraBuffer()` chamado na linha 251 atribui o valor de entrada inicial igual a zero, conforme o próximo subtítulo;
-   `disp` com o estado `Display::DIGIT` habilita a entrada para a inserção de um número pelo usuário (linha 252).

##### zeraBuffer() — linhas 255 a 258

Este método atua zerando o vetor que atua como buffer dos dígitos de entrada. Trata-se apenas de um laço `for` que percorre todo o vetor `bufferDisplay` igualando todos os seus valores a 0 (linha 257).

#### Calculadora() — linhas 9 a 13

Este é o construtor da classe. Na linha 11, o método define que o objeto é criado no estado desligado e, na linha seguinte, é atribuído o valor inicial 0 à memória.

#### bufferParaNumero() — linhas 15 a 21

Este método traduz o vetor de dígitos inseridos pelo usuário em um número no formato `double` que pode ser computado internamente.
Na linha 17, a variável `num` é criada com o valor inicial 0. O laço das linhas 18 e 19 incrementa a `num` o valor dos dígitos do mais ao menos significativo, usando `cont` e `dec` para determinar a potência de 10 pela qual este dígito será multiplicado. Na linha 20, a variável sinal indicará se o valor que será retornado é negativo ou não.

#### calcula() — linhas 23 a 54

Este método basicamente é a alma do negócio: realiza todas as contas, com exceção das envolvendo porcentagem ou a raiz quadrada. Como já é de se esperar, este método só atua se a instância de Calculadora está caracterizada como ligada e em funcionamento normal.
O booleano `pos` indica se o número em `acc` é positivo (linha 27), já `n` representa um possível operando (linha 28), com a entrada futura redefinida na linha 29.
A partir da operação selecionada (`switch` da linha 30), temos:
-   Na linha 32, temos a soma do acumulador com o operando `n`;
-   Na linha 33, temos o valor do acumulador subtraído do operando `n`;
-   Entre as linhas 34 e 38, é realizada a multiplicação entre o acumulador e `n`; nas linhas 36 e 37 é detectado um possível _overflow_ na aritmética do processador (sinalizado por fatores de mesmo sinal e resultado negativo) — neste caso, o modo de erro é acionado;
-   Entre as linhas 39 e 47, é realizada a divisão: na linha 40, um erro é atribuído quando o divisor é zero; de outra forma a divisão é calculada (linha 43) e, como no caso anterior, testa-se o _overflow_ e o correspondente feedback (linhas 44-45).
-   Em caso de não haver nenhuma operação, `n` é armazenado em `acc`, sem nenhuma ação aparente na visualização.
Se o valor da operação matemática exceder um valor que possa ser exibido em 16 dígitos (ou seja, maior que 10¹⁶), então o estado de funcionamento também é configurado como `EstadoLigado::ERRO` (linha 50). Se tudo correr bem, então o modo de display muda para a apresentação do resultado (linha 51) e o modo de operação é resetado (linha 52).

#### colocaMemoria(bool soma) — linhas 56 a 69

Este método implementa a adição de números digitados ou calculados à memória do dispositivo. O booleano `soma` passado como parâmetro define se esta adição será do valor positivo ou negativo, fazendo com que este trecho de código seja utilizado tanto pelo botão **M+** como no **M-**.
-   O `if` da linha 58 faz com que esta funcionalidade apenas atue quando a calculadora está ligada e sem erro;
-   Na linha 60, é tratado o caso em que o valor a ser adicionado à memória foi digitado pelo usuário;
-   Na linha 62, é realizada a operação matemática previamente selecionada, se houver; senão (linha 63) o valor no buffer de entrada deve ser convertido para `double` e passado ao acumulador `acc`;
-   Na linha 64, é definido que será exibido o resultado do acumulador;
-   Na linha 66, é definido se o valor em `acc` é somado à memória (se `soma` vale `true`) ou então subtraído (se `false`).

#### desliga() — linhas 70 a 73

Este método apenas altera o estado geral do objeto para desligado (linha 73).

#### digita(int n) — linhas 75 a 94

Este método permite a recepção da entrada de um valor numérico pelo usuário e exibe na tela sua situação em dado instante de tempo. A princípio é avaliado se o valor de `habilitado()` é `true`, caso em que o dispositivo responderá às entradas digitadas (linha 77).
Se estiver previamente sendo exibido um resultado calculado (modo `Display::RESULT`), o modo é alterado para `Display::DIGIT` e as variáveis associadas à entrada de informações são redefinidas para os valores iniciais (linhas 79-83).
Se inserido um dígito '0', este é contado para fins de exibição na tela (linha 84). Um possível bug devido à contagem de um dígito extra é corrigido eliminando possível zero à esquerda na linha 90.
Se o limite de dígitos não foi atingido ou não estão sendo inseridos zeros adicionais em um valor que já vale zero (condição da linha 85), os dígitos do buffer são deslocados de forma que o novo dígito ocupe o lugar do menos significativo (linhas 87-89).
Por fim, se o ponto decimal já foi digitado em algum momento, este também é deslocado (linha 91).

#### exibicao() — linhas 96 a 113

Este método caracteriza a sequência de caracteres a ser apresentada na parte do display reservada ao número para os diferentes contextos possíveis.
Nas linhas 98 e 99, são declarados a `string` s, usada para o processamento interno, e o `char*` arr, que será a referência retornada na linha 112.
Caso a calculadora esteja desligada ou apontando erro (linha 100), arr será uma string contendo apenas o caractere de fim de string e nenhum valor será exibido no display (linhas 102 e 103).
Se estiver em funcionamento normal, `s` recebe o valor convertido do buffer de entrada (caso esteja em modo de digitação) ou então o valor de `acc`, convertido para o formato decimal em no máximo 16 algarismos (linha 107). No caso específico de ser um número inteiro e portanto `s` não possuir um caractere '.', este é adicionado ao final da string, visando imitar a apresentação de uma calculadora convencional, onde o ponto aparece no final do número, se inteiro (linha 108).
Ao fim, a string é convertido para array de caracteres (linhas 109-110), que será retornado ao fim da execução.

#### inverteSinal() — linhas 115 a 122

Este método inverte o sinal do número apresentado no display. Primeiro é testado se a simulação aceita comandos (linha 117). Em caso positivo, verifica-se se a tela está apresentando dados digitados e se esse valor é diferente de zero (para impedir a apresentação de um valor "-0"). Se sim, `sinal` é negado, gerando a inversão (linha 119).
Porém, se estiver sendo apresentado o valor de `acc` em tela, este valor é diretamente negativado, tendo portanto seu sinal invertido ou caso seja zero, não se altera (linha 120).

#### isError() — linhas 124 a 128

Este método indica se a calculadora está em situação de erro ou não. Para tal, o `if-else` observa primeiro se a calculadora está desligada, retornando `false` em caso afirmativo (linha 126). Em caso contrário, verifica se a variável `est` tem o valor de `ERRO`, sendo o resultado desta comparação o valor de retorno (linha 127).

#### limpa() — linhas 130 a 137

Este método acumula duas funções. A primeira delas está na linha 132, que torna o estado armazenado em `lig` para `EstadoGeral::ON` caso esta esteja definida como desligada, "ligando" o objeto `calc`. Uma vez garantido que o dispositivo está ligado, os valores armazenados são redefinidos para seus valores iniciais, com exceção da memória.
Assim, `est` é definida como `EstadoLigado::NORMAL`, que torna este método o único que reverte um estado de erro (linha 133); `op` é atribuído como `Operacao::NADA`, reiniciando as operações matemáticas (linha 134); `acc` recebe o valor 0 (linha 135) e a entrada do usuário é limpa na chamada do método `resetEntrada()`.

#### limpaEntrada() — linhas 139 a 142

Este método limpa a entrada do usuário, mas só executa o método privado `resetEntrada()` caso a calculadora esteja habilitada a este comando (usando o método também privado `habilitado()`), conforme a cláusula `if` da linha 141.

#### ponto() — linhas 144 a 156

Este método garante a devida inserção do separador decimal. Seu funcionamento só ocorre se a calculadora está ligada e sem erro, e se `pt` for `false`, o que implica que o ponto não foi digitado antes neste mesmo número (linha 146).
Se um resultado de acumulador estiver sendo exibido (conforme apontado pelo estado `Display::RESULT` na linha 148), ao digitar um ponto, o modo de tela é alterado para mostrar a digitação (linha 150), bem como a entrada é redefinida (linha 151).
Se a entrada foi redefinida (linha 153), `cont` é atualizada para fazer constar um 0 na casa das unidades (digitar a sequência ".13", por exemplo, implica na entrada de valor entendida como "0.13").
Por fim, `pt` recebe o valor `true`, garantindo a presença de um único ponto decimal, no máximo.

#### porcentagem() — linhas 158 a 177

Este método responde pelos cálculos envolvendo porcentagem. Estes cálculos dependem de uma operação fundamental previamente selecionada. A condição `if` da linha 160 faz com que esta operação somente seja executada caso a calculadora esteja apta para receber entradas.
A priori, a entrada referente à porcentagem é passada ao acumulador, se já não estiver armazenada, e carregada na variável `n` (linha 162). Na linha 163, o `switch` verifica a operação prévia:
-   Na linha 65, tendo sido selecionada a soma, o acumulador tem seu valor acrescido de `n`% de seu valor;
-   Na linha 66, tendo sido selecionada a subtração, o acumulador tem seu valor decrescido de `n`% de seu valor;
-   Na linha 67, tendo sido selecionada a multiplicação, o acumulador tem seu valor multiplicado em `n`% (ou `n`/100);
-   Na linha 68, tendo sido selecionada a divisão, o acumulador tem seu valor dividido por `n`/100;
-   Na linha 69, a operação não é realizada caso nenhuma operação prévia tenha sido selecionada.
Tendo sido selecionada alguma operação (linha 171), esta é definida internamente, após o cálculo, como `NADA`, e o modo de tela é alterado para que o resultado final seja apŕesentado.

#### raiz() — linhas 179 a 195

Este método calcula a raiz quadrada do número que está sendo exibido no display. Como na maioria dos casos anteriores, só atua quando a calculadora está devidamente habilitada para receber entradas do usuário (linha 181).
Na condição da linha 183, é avaliado se a entrada é um número negativo, no modo de `Display` vigente. Em caso afirmativo, a simulação vai ao estado de erro (linha 184), pois é uma operação inválida no conjunto dos números reais.
Caso seja uma entrada válida (`else`, linha 185), verifica-se se a entrada estava sendo digitada (linha 187), sendo, neste caso, convertido o buffer de entrada em um número, cuja raiz quadrada é calculada e armazenada no acumulador, mudando o modo de exibição (linhas 189 e 190).
Caso esta entrada já seja um valor previamente presente em `acc`, este valor é atualizado com o cálculo de sua própria raiz quadrada (linha 192).

#### resultMemoria() — linhas 197 a 204

Caso a calculadora esteja habilitada para receber comandos (`if` da linha 199), o acumulador recebe o valor da memória e o display é habilitado para exibir o valor do acumulador (o estado `disp` é modificado para `Display::RESULT`), conforme linhas 201 e 202.

#### seleciona(char c) — linhas 206 a 226

Este método permite definir a operação que será realizada por meio do reconhecimento do botão de uma operação básica. Como descrito em `window.cpp`, cada um destes quatro botões fornece um valor distinto para o parâmetro `c`. Mais uma vez, este método só irá operar se a calculadora estiver em seu funcionamento normal (linha 208).
Se houver uma operação anterior pendente, esta será calculada e atribuída ao acumulador (linha 210). Em seguida, o valor de `c` entra em um seletor `switch` (linha 211). Daí, dependendo do caractere fornecido, uma das quatro operações fundamentais tem seu estado salvo visando a próxima conta (linhas 213-216). Em caso de um valor diverso (previsto apenas como proteção) o indicador de ausência de operação `NADA` é selecionado (linha 217).
Em caso de não estar se operando sobre um valor calculado anteriormente (linha 219), este valor digitado é atribuído a `acc` (linha 221), redefinindo-se a entrada e mostrando o número inserido no display (linhas 222 e 223).

#### temMemoria() — linhas 228 a 232

Este método retorna um booleano que informa se há um dado diferente de zero armazanado na memória do dispositivo simulado. Caso a calculadora esteja desligada, retornará falso (linha 230), pois este valor é usado para gerar um texto indicativo segundo descrito em `window.cpp`. Caso a calculadora esteja ligada, informa sobre haver um valor não-nulo armazenado (linha 231).

#### zeraMemoria() — linhas 234 a 237

Este método apenas redefine a memória para 0 caso o objeto tenha o estado ligado (linha 236).
