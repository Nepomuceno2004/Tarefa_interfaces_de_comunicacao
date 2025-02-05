#include <stdio.h>
#include "pico/stdlib.h"

#define led_pin_g 11
#define led_pin_b 12
#define led_pin_r 13
#define button_a 5
#define button_b 6

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

void gpio_irq_handler(uint gpio, uint32_t events);


int main()
{
    stdio_init_all();

    gpio_init(led_pin_r);
    gpio_set_dir(led_pin_r, GPIO_OUT);
    gpio_put(led_pin_r, 0); // Inicialmente desligado

    gpio_init(led_pin_g);
    gpio_set_dir(led_pin_g, GPIO_OUT);
    gpio_put(led_pin_g, 0); // Inicialmente desligado

    gpio_init(led_pin_b);
    gpio_set_dir(led_pin_b, GPIO_OUT);
    gpio_put(led_pin_b, 0); // Inicialmente desligado

    gpio_init(button_a);
    gpio_set_dir(button_a, GPIO_IN);
    gpio_pull_up(button_a);

    gpio_init(button_b);
    gpio_set_dir(button_b, GPIO_IN);
    gpio_pull_up(button_b);

    printf("RP2040 inicializado. Envie 'r', 'g' ou 'b' para alternar os LEDs ou aperte os botões 'A' ou 'B' .\n");

    gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true)
    {
        if (stdio_usb_connected())
        { // Certifica-se de que o USB está conectado
            char c;
            if (scanf("%c", &c) == 1)
            {
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

void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000) // 200ms de debounce
    {
        last_time = current_time;

        if (gpio == button_a)
        {
            gpio_put(led_pin_g, !gpio_get(led_pin_g));
            printf("LED verde alternado!\n");
        }
        else if (gpio == button_b)
        {
            gpio_put(led_pin_b, !gpio_get(led_pin_b));
            printf("LED azul alternado!\n");
        }
    }
}