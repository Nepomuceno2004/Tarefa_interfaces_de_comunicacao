#include <stdio.h>
#include "pico/stdlib.h"

#define led_pin_g 11
#define led_pin_b 12
#define led_pin_r 13

int main()
{
    stdio_init_all();

    // Configura os pinos dos LEDs como saída
    gpio_init(led_pin_r);
    gpio_set_dir(led_pin_r, GPIO_OUT);
    gpio_put(led_pin_r, 0); // Inicialmente desligado

    gpio_init(led_pin_g);
    gpio_set_dir(led_pin_g, GPIO_OUT);
    gpio_put(led_pin_g, 0); // Inicialmente desligado

    gpio_init(led_pin_b);
    gpio_set_dir(led_pin_b, GPIO_OUT);
    gpio_put(led_pin_b, 0); // Inicialmente desligado

    printf("RP2040 inicializado. Envie 'r', 'g' ou 'b' para alternar os LEDs.\n");

    while (true)
    {
        if (stdio_usb_connected())
        { // Certifica-se de que o USB está conectado
            char c;
            if (scanf("%c", &c) == 1)
            { // Lê caractere da entrada padrão
                printf("Recebido: '%c'\n", c);

                switch (c)
                {
                case 'r':
                    gpio_put(led_pin_r, !gpio_get(led_pin_r));
                    printf("LED vermelho alternado!\n");
                    break;
                case 'g':
                    gpio_put(led_pin_g, !gpio_get(led_pin_g));
                    printf("LED verde alternado!\n");
                    break;
                case 'b':
                    gpio_put(led_pin_b, !gpio_get(led_pin_b));
                    printf("LED azul alternado!\n");
                    break;
                default:
                    printf("Comando inválido: '%c'\n", c);
                }
            }
        }
        sleep_ms(40);
    }

    return 0;
}
