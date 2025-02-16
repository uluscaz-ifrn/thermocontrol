# ThermoControl - Monitoramento Inteligente de Temperatura

## 📌 Visão Geral
**ThermoControl** é um sistema de monitoramento de temperatura utilizando a placa **BitDogLab** baseada na **Raspberry Pi Pico**. O projeto permite a leitura da temperatura em tempo real, exibição dos dados em um **display OLED**, alertas via **LEDs e buzzer**, além do envio de informações para a plataforma **ThingSpeak** para monitoramento remoto.

## 🛠 Tecnologias e Componentes Utilizados

### 🔌 Hardware
- **BitDogLab (Raspberry Pi Pico)**
- **Display OLED (SSD1306 - I2C)**
- **LEDs RGB para indicação de status**
- **Buzzer para alertas sonoros**
- **Sensor de temperatura interno da Raspberry Pi Pico**
- **Conectividade Wi-Fi (CYW43)**

### 💻 Software
- **Linguagem C (SDK do Raspberry Pi Pico)**
- **Plataforma ThingSpeak para armazenamento e análise de dados**
- **Protocolo HTTP para comunicação com ThingSpeak**

## ⚙️ Funcionalidades
- 🔥 *Leitura de temperatura* utilizando o sensor interno da Raspberry Pi Pico
- 📟 *Exibição no display OLED* das informações de temperatura e status
- 🚦 *Alertas visuais e sonoros* quando a temperatura ultrapassa o limite ajustado
- 📡 *Envio de dados para ThingSpeak* via Wi-Fi
- 🎛 *Ajuste do limite de temperatura* utilizando um joystick analógico

## 📜 Estrutura do Código
O código-fonte principal está no arquivo `ThermoControl.c`, e segue a seguinte estrutura:

1. **Inicialização do sistema** (GPIOs, I2C, Wi-Fi, Display OLED)
2. **Leitura da temperatura e exibição no display**
3. **Verificação e acionamento de alertas** (LEDs/Buzzer)
4. **Envio de dados para o ThingSpeak**
5. **Ajuste dinâmico do limite de temperatura**
6. **Loop principal para atualização periódica**

## 🚀 Como Configurar e Executar

### 1️⃣ Pré-requisitos
- Placa **BitDogLab**
- **Raspberry Pi Pico SDK** instalado no seu ambiente de desenvolvimento
- Conexão com a Internet para envio de dados ao **ThingSpeak**

### 2️⃣ Clonar o repositório
```bash
  git clone https://github.com/seu-usuario/ThermoControl.git
  cd ThermoControl
```

### 3️⃣ Configurar Wi-Fi e API Key do ThingSpeak
No arquivo `ThermoControl.c`, edite as seguintes linhas com suas credenciais:
```c
#define WIFI_SSID "SEU_WIFI"
#define WIFI_PASS "SUA_SENHA"
#define THINGSPEAK_CHANNEL_API_KEY "SUA_API_KEY"
```

### 4️⃣ Compilar e carregar o código na Raspberry Pi Pico
```bash
  mkdir build && cd build
  cmake ..
  make
  picotool load -f ThermoControl.uf2
```

## 📊 Monitoramento no ThingSpeak
1. Acesse [ThingSpeak](https://thingspeak.com/)
2. Crie um canal e copie sua **API Key**
3. Acompanhe os dados recebidos e visualize os gráficos gerados

## 📌 Melhorias Futuras
- 🔧 **Suporte para sensores de temperatura externos**
- 📡 **Implementação do protocolo MQTT para comunicação mais robusta**
- 📱 **Criação de um dashboard web para visualização remota dos dados**

## 📄 Licença
Este projeto é de código aberto e está sob a licença **MIT**.

## 🙌 Contribuições
Contribuições são bem-vindas! Sinta-se à vontade para abrir **issues** e enviar **pull requests**.

