# TODO

## PARTE 2 – Transmissão para nuvem e visualização (Fog/Cloud Computing)

### Desenvolvimento (MQTT & Node-RED)

- [x] Configurar um broker MQTT (ex: HiveMQ Cloud).
- [x] Escrever código do ESP32 (Wokwi) que envia dados (dos 2 sensores) via MQTT para o broker.
- [x] Configurar o Node-RED.
- [x] Criar fluxo no Node-RED para subscrever aos tópicos MQTT.
- [x] Montar dashboard no Node-RED:
    - [x] Adicionar gráfico (ex: linha ou barra) para o sinal vital 1 (ex: batimentos).
    - [x] Adicionar medidor (gauge) para o sinal vital 2 (ex: temperatura).
    - [x] Adicionar indicador visual de alerta (ex: LED virtual, mudança de cor, ou texto).
- [x] Implementar lógica de alerta no Node-RED (ex: temperatura > 38°C ou BPM > 120).

### Entregáveis (Parte 2)

- [x] Código do ESP32 (Wokwi/Arduino IDE) com integração MQTT.
- [x] Prints do dashboard no Node-RED funcionando (recebendo dados e mostrando alertas).
- [x] (Opcional) Export do *flow* do Node-RED (arquivo JSON).
- [x] Relatório (Parte 2 - mínimo 2 páginas):
    - [x] Descrever o fluxo de comunicação MQTT (broker, tópicos, publicador, subscritor).
    - [x] Detalhar a configuração do dashboard no Node-RED.
    - [x] Explicar como os alertas foram configurados.
