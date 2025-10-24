# PARTE 2 – Envio MQTT e Dashboard Node-RED

Esta parte complementa o sistema de monitoramento cardíaco da Parte 1, integrando o **ESP32** com **MQTT seguro (TLS)** e visualização em tempo real no **Node-RED Dashboard**.

---

## 🎯 Objetivo

* Enviar leituras simuladas de **temperatura corporal** e **batimentos cardíacos (BPM)** via **MQTT (HiveMQ Cloud)**.
* Visualizar e monitorar os dados em **gráficos, medidores e indicadores de alerta**.
* Implementar **resiliência offline**, armazenando dados localmente no ESP32 quando o MQTT estiver indisponível.
* Ativar **alertas visuais (LEDs virtuais)** na dashboard quando valores críticos forem atingidos.

---

## ⚙️ Estrutura do Projeto

```
part2/
├── src/
│   └── main.ino              # Código principal do ESP32
├── node_red/
│   └── flow.json             # Fluxo do Node-RED com dashboard
├── assets/
│   └── readme.txt            # Este arquivo
└── README.md
```

---

## 🌐 Link do Projeto no Wokwi

Simulação do ESP32 enviando dados simulados de temperatura e batimentos cardíacos via MQTT:
[Simulação no Wokwi](https://wokwi.com/projects/445631947081068545)

**Componentes do Circuito:**

* ESP32 DevKit C v4 – Microcontrolador principal
* Sensores simulados: Temperatura e BPM (gerados por código)

### Instruções no Wokwi

* Pressione **“Play”** para iniciar a simulação.
* Acompanhe o **Monitor Serial** para ver temperatura e BPM.
* Observe o envio dos dados JSON ao **HiveMQ Cloud**.
* Confirme que os dados aparecem no **Node-RED via MQTT**.

---

## ⚙️ Funcionamento Geral

* O ESP32 gera e envia dados JSON com temperatura e batimentos cardíacos via MQTT.
* Caso perca conexão, os dados são armazenados localmente no **SPIFFS** e reenviados ao reconectar.
* O Node-RED recebe os dados, processa e exibe na dashboard:

| Elemento        | Função                                       |
| --------------- | -------------------------------------------- |
| Gráfico (Chart) | Temperatura corporal (°C)                    |
| Gauge (Medidor) | Batimentos cardíacos (BPM)                   |
| LEDs Virtuais   | Condição normal (verde) ou alerta (vermelho) |

---

## 🧩 Node-RED Dashboard

**Elementos configurados:**

* **Gráfico:** temperatura corporal (°C)
* **Gauge:** batimentos cardíacos (BPM)
* **LED Verde:** funcionamento normal
* **LED Vermelho:** alerta se:

  * BPM > 120
  * Temperatura > 38°C
* **Switch:** controle manual da simulação

---

## 🧠 Tecnologias Utilizadas

* ESP32 DevKit C v4
* Wokwi (simulação online)
* Node-RED + Dashboard
* MQTT (HiveMQ Cloud)
* WiFiClientSecure / PubSubClient / SPIFFS / ArduinoJson

---

## ✅ Funcionalidades Implementadas e Testadas

* Envio de dados JSON via MQTT com TLS
* Reconexão e reenvio automático (SPIFFS)
* Processamento e visualização em Node-RED
* Gráfico e medidor dinâmicos
* LED de alerta automático
* Integração total entre ESP32 → MQTT → Node-RED

---

## 📘 Relatório

Para mais detalhes, consulte o relatório completo:
[📄 report.md](assets/report.md)
---

## 👥 Integrantes do Grupo

* Gustavo Castro – RM560831
* Luis Emidio – RM559976
* Ricardo Oliveira – RM561182

**FIAP – IoT & Edge Computing**
Outubro / 2025


