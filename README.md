# ThermoControl - Monitoramento de Temperatura

## Descrição
ThermoControl é um sistema de monitoramento de temperatura desenvolvido como um projeto final para o Embarcatech. Ele utiliza a plataforma BitDogLab no qual tem no coração de seu hardware o Raspberry Pi Pico W. Ele mede a temperatura ambiente, exibe os dados em um display OLED SSD1306 e envia os valores periodicamente para o ThingSpeak. Além disso, permite ajustar o limite de temperatura via joystick analógico e aciona alertas visuais e sonoros quando a temperatura ultrapassa o limite estabelecido.

## Recursos
- 📡 **Conexão Wi-Fi:** Envio de dados para o ThingSpeak
- 📟 **Display OLED:** Exibição de temperatura e limite
- 🎮 **Joystick Analógico:** Ajuste dinâmico do limite de temperatura
- 🚨 **Alertas:** LEDs e buzzer acionados quando a temperatura ultrapassa o limite
- 🌡️ **Monitoramento em tempo real**

## Tecnologias Utilizadas
- **Linguagem:** C
- **Microcontrolador:** Raspberry Pi Pico W
- **Sensores e periféricos:**
  - Sensor de temperatura interno do Pico W
  - Display OLED SSD1306 via I2C
  - Buzzer para alertas sonoros
  - LEDs para indicação de status
  - Joystick analógico para ajuste do limite de temperatura
  - Conexão Wi-Fi para envio de dados ao ThingSpeak

## Requisitos
- Raspberry Pi Pico W
- Display OLED SSD1306 (I2C)
- Joystick analógico
- LEDs e buzzer
- Conexão Wi-Fi configurada
- Conta no [ThingSpeak](https://thingspeak.com/) para recebimento dos dados

## Instalação e Configuração
1. **Clone este repositório:**
   ```bash
   git clone https://github.com/seuusuario/ThermoControl.git
   cd ThermoControl
   ```

2. **Configure o Wi-Fi e o ThingSpeak:**
   Edite o arquivo `ThermoControl.c` e substitua:
   ```c
   #define WIFI_SSID "SEU_WIFI"
   #define WIFI_PASS "SUA_SENHA"
   #define THINGSPEAK_CHANNEL_API_KEY "SUA_API_KEY"
   ```

3. **Compile e faça o upload para o Raspberry Pi Pico W:**
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
   Em seguida, envie o binário gerado para o Raspberry Pi Pico.

## Funcionamento
- O display OLED exibe a temperatura e o limite configurado.
- O joystick permite aumentar ou diminuir o limite de temperatura.
- 🌐 Os dados são enviados para o ThingSpeak a cada 15 segundos
- Se a temperatura ultrapassar o limite:
  - 🚨 O LED vermelho acende
  - 🔊 O buzzer emite um alerta sonoro

## Possíveis Melhorias
- 📈 Utilizar um sensor externo para medições mais precisas
- 📊 Criar um dashboard web para monitoramento remoto
- 📡 Melhorar a comunicação com APIs externas

## Licença
Este projeto está sob a Licença MIT. Sinta-se à vontade para usar e modificar!
