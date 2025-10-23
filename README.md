# activity-fiap2-ai-p3a1 <!-- omit in toc -->

> Este projeto faz parte do curso de **Inteligência Artificial** da [FIAP](https://github.com/fiap) - Online 2024. Este repositório é a atividade "**Ano 2 - Fase 3** - Cap 1 - CardioIA Conectada: IoT e Visualização de Dados para a Saúde Digital".

---

## Sobre o Projeto CardioIA

O CardioIA é um projedo simples didático de um sistema de monitoramento cardíaco inteligente que integra conceitos de IoT, computação em nuvem, Edge/Fog Computing e visualização de dados para aplicações em saúde digital.

## Esta Fase 3 - Monitoramento Contínuo

Este repositório contém a implementação da **Fase 3** do projeto CardioIA, focada no desenvolvimento de um sistema de monitoramento contínuo que:

- Captura sinais vitais simulados (batimentos cardíacos, temperatura, umidade e movimento)
- Armazena e processa informações localmente (Edge Computing)
- Transmite dados para a nuvem via MQTT
- Exibe resultados em dashboards interativos com alertas automáticos

---

## Estrutura do Projeto

### Parte 1 — Armazenamento e processamento local (Edge Computing)

Desenvolvimento no Wokwi com ESP32, incluindo:
- 2 sensores (DHT22 obrigatório + sensor de livre escolha)
- Armazenamento local no SPIFFS
- Simulação de conectividade Wi-Fi
- Resiliência offline

Acesse a pasta [part1](part1) para mais informações.

### Parte 2 — Transmissão para nuvem e visualização (Fog/Cloud Computing)

Sistema completo de monitoramento com:
- Transmissão via MQTT para nuvem
- Dashboard no Node-RED com visualização em tempo real
- Alertas automáticos baseados em limites definidos
- Integração opcional com Grafana Cloud

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