#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define led_pin_g 11
#define led_pin_b 12
#define led_pin_r 13
#define button_a 5
#define button_b 6
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 5; // Intensidade do vermelho
uint8_t led_g = 0; // Intensidade do verde
uint8_t led_b = 0; // Intensidade do azul

bool numero_0[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1};

bool numero_1[NUM_PIXELS] = {
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0};

bool numero_2[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1};

bool numero_3[NUM_PIXELS] = {
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 1,
    1, 1, 1, 1, 1};

bool numero_4[NUM_PIXELS] = {
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    1,
};

bool numero_5[NUM_PIXELS] = {
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
};

bool numero_6[NUM_PIXELS] = {
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
};

bool numero_7[NUM_PIXELS] = {
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
};

bool numero_8[NUM_PIXELS] = {
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
};

bool numero_9[NUM_PIXELS] = {
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
};

void gpio_irq_handler(uint gpio, uint32_t events);
static inline void put_pixel(uint32_t pixel_grb);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void set_one_led(uint8_t r, uint8_t g, uint8_t b, bool led_buffer[]);
void decisao_switch(char c);

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

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

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
                decisao_switch(c);
            }
        }
        sleep_ms(40);
    }

    return 0;
}

void decisao_switch(char c)
{
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
    case '0':
        set_one_led(led_r, led_g, led_b, numero_0);
        break;
    case '1':
        set_one_led(led_r, led_g, led_b, numero_1);
        break;
    case '2':
        set_one_led(led_r, led_g, led_b, numero_2);
        break;
    case '3':
        set_one_led(led_r, led_g, led_b, numero_3);
        break;
    case '4':
        set_one_led(led_r, led_g, led_b, numero_4);
        break;
    case '5':
        set_one_led(led_r, led_g, led_b, numero_5);
        break;
    case '6':
        set_one_led(led_r, led_g, led_b, numero_6);
        break;
    case '7':
        set_one_led(led_r, led_g, led_b, numero_7);
        break;
    case '8':
        set_one_led(led_r, led_g, led_b, numero_8);
        break;
    case '9':
        set_one_led(led_r, led_g, led_b, numero_9);
        break;
    default:
        printf("Comando inválido: '%c'\n", c);
    }

    return;
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

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Converte valores RGB para o formato de 32 bits utilizado pelos LEDs WS2812.
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Atualiza os LEDs da matriz de acordo com o número a ser exibido.
void set_one_led(uint8_t r, uint8_t g, uint8_t b, bool led_buffer[])
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_buffer[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0); // Desliga os LEDs com zero no buffer
        }
    }
}