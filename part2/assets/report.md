# **Relatório – Integração Parte 1 + Parte 2: Monitoramento Contínuo de Sinais Vitais**

## 1. Resumo Executivo

Este relatório descreve a integração do sistema de monitoramento cardíaco baseado em ESP32, combinando **Edge Computing (Parte 1)** e **Transmissão via MQTT + Dashboard (Parte 2)**. O sistema coleta dados simulados de **temperatura corporal** e **batimentos cardíacos (BPM)**, armazena localmente quando offline e envia os dados para a nuvem via **HiveMQ Cloud**, exibindo-os em tempo real no **Node-RED Dashboard**.

---

## 2. Arquitetura do Sistema

### 2.1 Componentes Principais

* **ESP32 DevKit C v4**: microcontrolador principal
* **Sensores simulados**: temperatura e BPM
* **SPIFFS**: armazenamento local das mensagens offline
* **HiveMQ Cloud**: broker MQTT seguro (TLS)
* **Node-RED + Dashboard**: visualização e alertas

### 2.2 Fluxo de Dados

1. O ESP32 lê dados simulados de temperatura e BPM a cada 2–3 segundos.
2. Cria JSON com:

```json
{
  "device": "device1",
  "seq": 1,
  "ts": 1734567890,
  "temperature": 36.5,
  "bpm": 75
}
```

3. Se MQTT conectado → envia JSON para HiveMQ.
4. Se offline → salva localmente no SPIFFS (buffer circular de 100 mensagens).
5. Reconexão → reenvio automático das mensagens armazenadas.
6. Node-RED recebe as mensagens e atualiza dashboard: gráfico, gauge e LED de alerta.

---

## 3. Integração Parte 1 + Parte 2

### 3.1 Parte 1 – Edge Computing

* Coleta de dados simulados (BPM e temperatura)
* Armazenamento local no SPIFFS
* Buffer circular garante que os últimos 100 dados sejam preservados
* Sistema continua coletando mesmo sem conectividade

### 3.2 Parte 2 – MQTT e Node-RED

* Configuração MQTT segura (TLS) para HiveMQ
* Tópico utilizado: `device1/data`
* Publicação de mensagens JSON
* Node-RED Dashboard:

  * **Chart**: BPM em tempo real
  * **Gauge**: temperatura em tempo real
  * **LED virtual**: vermelho se BPM > 120 ou Temp > 38°C; verde caso contrário
* Reenvio automático do SPIFFS garante que nenhum dado seja perdido

---

## 4. Estrutura do Código

```
part2/
├── src/
│   └── main.ino        # Código integrado (Parte 1 + Parte 2)
├── node_red/
│   └── flow.json       # Fluxo Node-RED
├── assets/
│   └── readme.txt
└── README.md
```

* O **main.ino** inclui:

  * Conexão WiFi
  * Conexão MQTT com TLS
  * Função `storeOffline()` → buffer circular SPIFFS
  * Função `resendStored()` → reenvio automático
  * Loop de envio a cada 2–3 segundos

---

## 5. Testes e Funcionamento

### 5.1 Cenário Online

* ESP32 conectado à WiFi
* Mensagens publicadas no broker HiveMQ
* Dashboard Node-RED atualiza gráficos e alertas em tempo real

### 5.2 Cenário Offline

* WiFi desconectado ou MQTT offline
* Mensagens armazenadas localmente (SPIFFS)
* Reconexão automática envia dados armazenados
* Dashboard atualiza os dados históricos assim que reconectar

---

## 6. Benefícios da Integração

* **Resiliência**: funciona mesmo sem internet
* **Eficiência**: minimiza perda de dados e tráfego de rede
* **Visualização**: Node-RED mostra dados em tempo real e alertas
* **Escalabilidade**: pronto para adicionar novos sensores ou funcionalidades

---




## 7. Conclusão

A integração da **Parte 1 (Edge Computing)** com a **Parte 2 (MQTT + Node-RED)** foi concluída com sucesso. O ESP32 coleta dados de sinais vitais, preserva mensagens offline, publica via MQTT e exibe informações em tempo real. Essa abordagem garante **resiliência, confiabilidade e monitoramento contínuo**, podendo ser expandida para aplicações reais de saúde ou IoT.

---

## 8. Evidências do Sistema

### 8.1 Conexão com o Broker MQTT
![Conexão com Broker MQTT](assets/broker_ok.jpg)

### 8.2 Terminal CMD - Envio de Dados
Mostrando ESP32 conectado ao HiveMQ e envio de mensagens JSON.
![Terminal CMD](assets/cmd.jpg)

### 8.3 Dashboard Node-RED - Valor Normal
Valor baixo de BPM/Temperatura, LED verde indicando funcionamento normal.
![Dashboard Normal](assets/dash.jpg)

### 8.4 Cenário Offline / Erro de Conexão
Exemplo de desconexão, erro no MQTT e salvamento de dados localmente.
![Erro de Conexão](assets/erro.jpg)

### 8.5 ESP32 - Visão Geral do Wokwi
Mostrando dispositivo, serial monitor e fluxo de execução.
![ESP32 Geral](assets/esp32.jpg)

### 8.6 Fluxo Node-RED
Exibição do fluxo configurado, incluindo tópicos MQTT e lógica do dashboard.
![Fluxo Node-RED](assets/fluxo.jpg)

### 8.7 Cenário Crítico - Valor Alto
Exemplo de batimento/temperatura alto, LED vermelho indicando alerta.
![Alerta Alto](assets/high.jpg)

### 8.8 HiveMQ Cloud - Visualização do Broker
Servidor MQTT com tópicos e dados recebidos.
![HiveMQ Cloud](assets/hive.jpg)



**Desenvolvido por:** Gustavo Castro, Luis Emidio, Ricardo Oliveira
**Data:** Outubro 2025
**Tecnologias:** ESP32, SPIFFS, MQTT (HiveMQ Cloud), Node-RED Dashboard, ArduinoJson
