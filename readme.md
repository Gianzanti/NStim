# Organização do repositório

## IHM

Folder com o projeto da Unidade de Gerenciamento de IHM.
Realizado em C/C++, usando como microcontralador um Arduino Nano

## GCEstimulo

Folder com o projeto da Unidade de Gerenciamento e Controle de Estímulos
Realizado em C/C++, usando como microcontralador um Arduino Nano

## UI

Folder com o projeto visual da UI
Realizado usando o software UnicView AD, da Proculus, usando como LCD um Proculus Family J - Class C - 320 x 240 px

### Controle dos VP (Variable Points) da UI

| **Descrição do VP**     | **VP** | **Range** | **Descrição do Valor**       |
| ----------------------- | :----: | :-------: | :--------------------------- |
| Erro                    | **1**  |   0 - 4   | 0 - Não exibe erros          |
|                         |        |           | 1 - Bateria                  |
|                         |        |           | 2 - Cabo não reconhecido     |
|                         |        |           | 3 - Inserir cabo             |
|                         |        |           | 4 - Estimulação abortada     |
| Nível da bateria        | **3**  |   0 - 3   | 0 - 0% - 32%                 |
|                         |        |           | 1 - 33% - 65%                |
|                         |        |           | 2 - 66% - 99%                |
|                         |        |           | 3 - 100%                     |
| Volume                  | **5**  |  0 - 100  | 0 a 20 - 0% - 32%            |
|                         |        |           | 21 a 50 - 33% - 65%          |
|                         |        |           | 51 a 80 - 66% - 99%          |
|                         |        |           | 81 a 100 - 100%              |
| Modo de Estímulo        | **7**  |   0 - 1   | 0 - Modo Cortical            |
|                         |        |           | 1 - Modo Localização         |
| Forma de Onda           | **9**  |   0 - 3   | 0 - Trem de Pulso            |
|                         |        |           | 1 - Pulso                    |
|                         |        |           | 2 - Bifásica Positiva        |
|                         |        |           | 3 - Bifásica Negativa        |
| Estado do Estímulo      | **11** |   0 - 1   | 0 - Estímulo em Pausa        |
|                         |        |           | 1 - Estímulo em Progresso    |
| Definição de Corrente   | **13** |  0 - 999  | Valor com ponto fixo         |
|                         |        |           | intervalo 0.00 e 99.9 mA     |
| Definição de Período    | **15** |  0 - 999  | Valor com ponto fixo         |
|                         |        |           | intervalo 0.00 e 99.9 ms     |
| Definição de Frequência | **17** | 0 - 9999  | em Hz (Hertz)                |
|                         |        |           | intervalo 0.00 e 99.9 ms     |
| Carga mensurada         | **19** | 0 - 9999  | em nC (nanoCoulomb)          |
| Impedância mensurada    | **21** | 0 - 9999  | em Ω (ohms)                  |
| Idioma da Interface     | **23** |   0 - 1   | 0 - Português (pt-br)        |
|                         |        |           | 1 - Inglês (en-us)           |
| Backlight               | **25** |  0 - 100  | Percentual de luminosidade   |
| Pulsos                  | **27** |  0 - 10   | Quantidade de pulsos, quando |
|                         |        |           | no modo Trem de Pulso        |
| Intervalo               | **29** |  0 - 999  | Distância entre cada         |
|                         |        |           | trem de pulsos               |
