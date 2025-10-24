# activity-fiap2-ai-p3a1 <!-- omit in toc -->

> Este projeto faz parte do curso de **Inteligência Artificial** da [FIAP](https://github.com/fiap) - Online 2024. Este repositório é a atividade "**Ano 2 - Fase 3** - Cap 1 - CardioIA Conectada: IoT e Visualização de Dados para a Saúde Digital".

---

## Sobre o Projeto CardioIA

O CardioIA é um projeto simples didático de um sistema de monitoramento cardíaco inteligente que integra conceitos de IoT, computação em nuvem, Edge/Fog Computing e visualização de dados para aplicações em saúde digital.

## Esta Fase 3 - Monitoramento Contínuo

Este repositório contém a implementação da **Fase 3** do projeto CardioIA, focada no desenvolvimento de um sistema de monitoramento contínuo que:

- ✅ **Captura sinais vitais simulados** (batimentos cardíacos, temperatura, umidade)
- ✅ **Armazena e processa informações localmente** (Edge Computing com SD Card)
- ✅ **Resiliência offline** com sincronização automática quando conecta
- ✅ **Transmite dados para a nuvem** via MQTT (Parte 2)
- ✅ **Exibe resultados em dashboards** interativos com alertas automáticos (Parte 2)

---

## Estrutura do Projeto

### Parte 1 — Armazenamento e processamento local (Edge Computing)

Desenvolvimento no Wokwi com ESP32, incluindo:
- ✅ **2 sensores**: DHT22 (temperatura/umidade) + Botão (batimentos cardíacos)
- ✅ **Armazenamento local no SPIFFS** com buffer circular (100 amostras)
- ✅ **Simulação de conectividade Wi-Fi** (alterna a cada 30s)
- ✅ **Resiliência offline** com sincronização automática
- ✅ **Sincronização em lote** eficiente quando conecta

**Link Wokwi**: [Simulação Funcionando](https://wokwi.com/projects/445383801865359361)

Acesse a pasta [part1](part1) para mais informações.

### Parte 2 — Transmissão para nuvem e visualização (Fog/Cloud Computing)

Sistema completo de monitoramento com:
- ✅ **Transmissão via MQTT** para HiveMQ Cloud (TLS)
- ✅ **Dashboard Node-RED** com visualização em tempo real
- ✅ **Armazenamento offline** com sincronização automática
- ✅ **Alertas visuais** baseados em limites definidos

**Link Wokwi**: [Simulação Funcionando](https://wokwi.com/projects/445631947081068545)

Acesse a pasta [part2](part2) para mais informações.

---

## Equipe

### Membros

- Gustavo Castro (RM560831)
- Luis Emidio (RM559976)
- Ricardo Oliveira (RM561182)

### Professores

- **Tutor**: [Leonardo Ruiz Orabona](https://www.linkedin.com/in/leonardoorabona/)  
- **Coordenador**: [André Godoi](https://www.linkedin.com/in/profandregodoi/)  

---

[LICENSE](LICENSE)