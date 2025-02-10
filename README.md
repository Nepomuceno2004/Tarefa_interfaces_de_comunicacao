# Tarefa sobre interfaces de comunicações.
Este projeto tem como objetivo explorar e consolidar os conceitos de comunicação serial utilizando o microcontrolador RP2040 na placa de desenvolvimento BitDogLab.

## Funcionalidades

1. Modificação da Biblioteca font.h
   - Adição de caracteres minúsculos personalizados.

2. Entrada de Caracteres via PC
   - Entrada de caracteres pelo Serial Monitor do VS Code.
   - Exibição do caractere no display SSD1306.
   - Quando um número (0 a 9) for digitado, um símbolo correspondente é exibido na matriz de LEDs WS2812.

3. Interação com o Botão A
   - Alterna o estado do LED RGB Verde (ligado/desligado).
   - Exibe mensagem no display SSD1306 e no Serial Monitor.

4. Interação com o Botão B
   - Alterna o estado do LED RGB Azul (ligado/desligado).
   - Exibe mensagem no display SSD1306 e no Serial Monitor.

## Como usar

1. Entrada de Caracteres pelo Serial Monitor
   - Abra o Serial Monitor no VS Code (115200 baud).
   - Digite um caractere e pressione Enter.
   - O caractere será exibido no display SSD1306.
   - Se for um número (0-9), um símbolo correspondente aparecerá na matriz WS2812.

2. Uso dos Botões
   - Botão A (GPIO 5) → Alterna o LED Verde do RGB.
   - Mostra uma mensagem no display SSD1306 e no Serial Monitor.
   - Botão B (GPIO 6) → Alterna o LED Azul do RGB.
   - Mostra uma mensagem no display SSD1306 e no Serial Monitor.

## Vídeo de funcionamento
[DRIVE](https://drive.google.com/file/d/1rXAHaI3Q7h9mYURxemshJwHarujbNvVD/view?usp=drive_link)

## Autor
### Matheus Nepomuceno Souza
