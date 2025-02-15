#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include <string.h>
#include "hardware/adc.h"
#include "inc/ssd1306.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define WIFI_SSID ""  // Nome da rede Wi-Fi
#define WIFI_PASS ""     // Senha da rede Wi-Fi
#define THINGSPEAK_CHANNEL_API_KEY "" // API Key do ThingSpeak

#define I2C_PORT i2c1
#define PINO_SCL 14
#define PINO_SDA 15
#define BUZZER_PIN 21
#define GREEN_LED_PIN 11
#define RED_LED_PIN 13
#define VRX 26
ssd1306_t disp; // Display OLED
static float temperatura = 0.0; // Temperatura inicial
static float temp_limit = 30.0f; // Limite inicial de temperatura

// Funcao para enviar dados para o ThingSpeak
static void enviar_para_thingspeak() {
    char post_data[256];
    // Formatar os dados para o POST
    snprintf(post_data, sizeof(post_data),
             "api_key=%s&field1=%.2f",
             THINGSPEAK_CHANNEL_API_KEY, temperatura);

    // Criar conexão TCP
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB TCP\n");
        return;
    }

    // Endereço IP do ThingSpeak
    ip_addr_t ip;
    IP4_ADDR(&ip, 184, 106, 153, 149);

    // Conectar ao servidor na porta 80
    err_t err = tcp_connect(pcb, &ip, 80, NULL);
    if (err != ERR_OK) {
        printf("Erro ao conectar ao ThingSpeak\n"); 
        tcp_close(pcb);
        return;
    }

    // Montar a requisição HTTP
    char request[512];
    snprintf(request, sizeof(request),
             "POST /update.json HTTP/1.1\r\n"
             "Host: api.thingspeak.com\r\n"
             "Connection: close\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "Content-Length: %d\r\n\r\n"
             "%s",
             (int)strlen(post_data), post_data);

    // Enviar a requisição
    err_t send_err = tcp_write(pcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    if (send_err != ERR_OK) { // Verificar se os dados foram enviados
        printf("Erro ao enviar os dados para o ThingSpeak\n");
        tcp_close(pcb);
        return;
    } else{
        printf("Dados enviados para o ThingSpeak com sucesso!\n");        
    }

    // Garantir que os dados sejam enviados
    tcp_output(pcb);
}

// Inicializa o sistema
void inicializar() {
    stdio_init_all();
    adc_init();

    adc_set_temp_sensor_enabled(true); // Prepara o sistema para leitura da temperatura de um sensor interno

    // Inicialização do display OLED
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SCL);
    gpio_pull_up(PINO_SDA);
    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT);
   
    // Configuração do buzzer
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    
    // Configuração dos LEDs RGB
    gpio_init(GREEN_LED_PIN);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
    gpio_put(GREEN_LED_PIN, 0); // LED desligado inicialmente
    gpio_init(RED_LED_PIN);
    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_put(RED_LED_PIN, 0); // LED desligado inicialmente

    adc_gpio_init(VRX); // Configura o pino do eixo X do joystick para entrada ADC
}

// Função para tocar o buzzer
void tocar_buzzer() {
    gpio_put(BUZZER_PIN, 1);
    sleep_ms(100);
    gpio_put(BUZZER_PIN, 0);
    sleep_ms(100);
}
// Função para alterar o status do LED em relação a temperatura limite
void set_led_status(bool alert) {
    if (alert) { // LED vermelho se a temperatura ultrapassar o limite
        gpio_put(RED_LED_PIN, 1);
        gpio_put(GREEN_LED_PIN, 0);
    } else { // LED verde se a temperatura estiver dentro do limite
        gpio_put(RED_LED_PIN, 0);
        gpio_put(GREEN_LED_PIN, 1);
    }
}
// Função para ajustar o limite de temperatura
float ajustar_limite_temperatura(float limite_atual) {
    adc_select_input(0); // Seleciona o canal do eixo X do joystick
    uint16_t vrx_value = adc_read();
    const float ajuste_passo = 1.0f; // Passo para ajuste do limite
    bool ajustado = false;
    // Ajusta o limite de temperatura com base no eixo X do joystick
    if (vrx_value > 3000) {
        limite_atual += ajuste_passo; // Aumenta o limite
        ajustado = true;
    } else if (vrx_value < 1000) {
        limite_atual -= ajuste_passo; // Diminui o limite
        ajustado = true;
    }
    if (limite_atual < 10.0f) limite_atual = 10.0f;  // Limite mínimo
    if (limite_atual > 50.0f) limite_atual = 50.0f;  // Limite máximo

    // Exibe mensagem de ajuste no display
    if (ajustado) {
        char buffer[32];
        ssd1306_clear(&disp);
        snprintf(buffer, sizeof(buffer), "Novo Limite: %.2f C", limite_atual);
        ssd1306_draw_string(&disp, 10, 28, 1, buffer);
        ssd1306_draw_string(&disp, 10, 44, 1, "Ajustando...");
        ssd1306_show(&disp);
        sleep_ms(1800); // Breve pausa para feedback visual
    }
    return limite_atual;
}
// Função para exibir a temperatura e o limite no display
void mostrar_temperatura(float temperature, float limit) {
    char buffer[32];
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 2, 2, 1, "Sis. Temp");
    snprintf(buffer, sizeof(buffer), "Temp: %.2f C", temperature);
    ssd1306_draw_string(&disp, 2, 18, 1, buffer);
    snprintf(buffer, sizeof(buffer), "Limite: %.2f C", limit);
    ssd1306_draw_string(&disp, 2, 34, 1, buffer);
        // Exibe alerta se a temperatura ultrapassar o limite
        if (temperature > limit) {
        ssd1306_draw_string(&disp, 2, 50, 1, "Status: ALERTA!");
        set_led_status(true);
    } else {
        // Exibe status o normal se a temperatura estiver dentro do limite
        ssd1306_draw_string(&disp, 2, 50, 1, "Status: Normal");
        set_led_status(false);
    }
    ssd1306_show(&disp);
}

int main() {
    inicializar();
    sleep_ms(1000);  // Breve pausa para inicialização
    uint32_t ultimo_tempo_solicitacao = 0; // Último tempo de envio de dados

    // Inicializa o Wi-Fi
    if (cyw43_arch_init()) {
        printf("Erro ao inicializar o Wi-Fi\n");
        return 1;
    }
    // Habilita o modo estação
    cyw43_arch_enable_sta_mode();
    printf("Conectando a rede Wi-Fi...\n");
     // Conecta ao Wi-Fi
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Falha ao conectar a rede Wi-Fi\n");
        return 1;
    } else {
        printf("Conexao com Wi-Fi bem sucedida!\n");
    }
    // Loop principal
    while (true) {
        uint32_t tempo_atual = to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual
        temperatura = 31.5 + (rand() % 100) / 50.0; // Simula a temperatura
        temp_limit = ajustar_limite_temperatura(temp_limit); // Ajusta o limite de temperatura
        mostrar_temperatura(temperatura, temp_limit); // Exibe a temperatura e o limite
        
        if (temperatura > temp_limit) { // Toca o buzzer se a temperatura ultrapassar o limite
            tocar_buzzer();}
        sleep_ms(2000); // Atualiza a cada 2 segundos
        
        // Envia a temperatura para o ThingSpeak a cada 15 segundos por utilizar a versao gratuita
        if (tempo_atual - ultimo_tempo_solicitacao >= 15000) {
            printf("Enviando temperatura ao ThingSpeak: %.2f C\n", temperatura);
            enviar_para_thingspeak();
            ultimo_tempo_solicitacao = tempo_atual;
        }
    }
    cyw43_arch_deinit();  // Desliga o Wi-Fi
    return 0;
}
