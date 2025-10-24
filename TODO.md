# TODO – FASE 3: Monitoramento Contínuo – IoT na Saúde

## PARTE 1 – Armazenamento e Processamento Local (Edge Computing)

- [PARTE 1 - TODO.md](./part1/TODO.md) - **Em revisão**

## PARTE 2 – Transmissão para Nuvem e Visualização (Fog/Cloud)

### Desenvolvimento (MQTT & Node-RED)

- [ ] Configurar um broker MQTT (ex: HiveMQ Cloud).
- [ ] Escrever código do ESP32 (Wokwi) que envia dados (dos 2 sensores) via MQTT para o broker.
- [ ] Configurar o Node-RED.
- [ ] Criar fluxo no Node-RED para subscrever aos tópicos MQTT.
- [ ] Montar dashboard no Node-RED:
    - [ ] Adicionar gráfico (ex: linha ou barra) para o sinal vital 1 (ex: batimentos).
    - [ ] Adicionar medidor (gauge) para o sinal vital 2 (ex: temperatura).
    - [ ] Adicionar indicador visual de alerta (ex: LED virtual, mudança de cor, ou texto).
- [ ] Implementar lógica de alerta no Node-RED (ex: temperatura > 38°C ou BPM > 120).
- [ ] (Opcional) Integrar Node-RED com Grafana Cloud para visualização avançada.

### Entregáveis (Parte 2)

- [ ] Código do ESP32 (Wokwi/Arduino IDE) com integração MQTT.
- [ ] Prints do dashboard no Node-RED funcionando (recebendo dados e mostrando alertas).
- [ ] (Opcional) Export do *flow* do Node-RED (arquivo JSON).
- [ ] (Opcional) Prints do dashboard no Grafana.
- [ ] Relatório (Parte 2 - mínimo 2 páginas):
    - [ ] Descrever o fluxo de comunicação MQTT (broker, tópicos, publicador, subscritor).
    - [ ] Detalhar a configuração do dashboard no Node-RED.
    - [ ] Explicar como os alertas foram configurados.
