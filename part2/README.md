# PARTE 2 – Transmissão para nuvem e visualização (Fog/Cloud Computing)

Esta parte implementa um sistema completo de monitoramento de dados vitais (temperatura e batimentos cardíacos) usando ESP32, com envio via MQTT seguro (TLS) para HiveMQ Cloud e armazenamento local quando offline.

## Estrutura do Projeto

```
part2/
├── src/
│   ├── sketch.ino             # Código principal do ESP32 com MQTT e SPIFFS
│   ├── diagram.json           # Configuração do circuito no Wokwi (ESP32 DevKit C v4)
│   ├── libraries.txt          # Bibliotecas: PubSubClient, ArduinoJson
│   └── wokwi-project.txt      # Configuração do projeto Wokwi
├── node_red/
│   └── flows.json             # Fluxo básico do Node-RED (debug)
├── assets/
│   ├── report.md              # Relatório detalhado da Parte 2
│   ├── broker.jpg             # Screenshot do broker MQTT
│   ├── cmd.jpg                # Screenshot do terminal
│   ├── dash.jpg               # Screenshot do dashboard
│   ├── erro.jpg               # Screenshot de erro de conexão
│   ├── esp32.jpg              # Screenshot do ESP32
│   ├── fluxo.jpg              # Screenshot do fluxo Node-RED
│   ├── high.jpg               # Screenshot de alerta alto
│   └── hive.jpg               # Screenshot do HiveMQ Cloud
└── README.md                  # Este arquivo
```

## Link do Projeto no Wokwi

Simulação completa do sistema de monitoramento com ESP32, MQTT e armazenamento local.

  * **Link:** [https://wokwi.com/projects/445631947081068545](https://wokwi.com/projects/445631947081068545)

**Componentes do Circuito:**

- **ESP32 DevKit C v4**: Microcontrolador principal com WiFi e TLS
- **Monitor Serial**: Para visualização dos dados e logs do sistema
- **Sensores simulados**: Temperatura (36.5°C ± 1°C) e BPM (60-130 bpm)

**Funcionalidades Implementadas:**

- ✅ **Geração de dados simulados** com variação realística
- ✅ **Conexão WiFi** com timeout de 10 segundos
- ✅ **MQTT com TLS** para HiveMQ Cloud (modo inseguro no Wokwi)
- ✅ **Armazenamento local SPIFFS** quando offline
- ✅ **Reconexão automática** e sincronização de dados
- ✅ **JSON estruturado** com device, sequência, timestamp, temperatura e BPM

**Instruções no Wokwi:**

1. Pressione "Play" na simulação
2. Observe o monitor serial para logs detalhados
3. Acompanhe o envio dos dados JSON ao HiveMQ Cloud
4. Teste a funcionalidade offline/online

## Como Testar o Sistema

### Teste de Conexão e Envio MQTT
1. **Aguarde a conexão WiFi**: `[WIFI] Conectado: [IP]`
2. **Observe a conexão MQTT**: `[MQTT] Conectando ao broker... conectado.`
3. **Verifique o envio de dados**: `[MQTT] Enviado com sucesso.`
4. **Analise o JSON enviado**: `{"device":"device1","seq":X,"ts":Y,"temperature":Z,"bpm":W}`

### Teste de Funcionalidade Offline
1. **Simule perda de conexão**: `[OFFLINE] Sem conexão. Salvando dados...`
2. **Observe o armazenamento local**: `[SALVO] Dados armazenados localmente.`
3. **Reconecte e veja a sincronização**: `[REENVIO] Mensagem reenviada.`
4. **Confirme a limpeza**: `[REENVIO] Todos os dados enviados. Arquivo limpo.`

### Teste de Logs Detalhados
1. **Monitore o boot**: `[BOOT] Iniciando dispositivo...`
2. **Verifique SPIFFS**: `[OK] SPIFFS montado.`
3. **Acompanhe os dados**: `[DATA] #X | Temp=Y | BPM=Z | WiFi=1 | MQTT=1`
4. **Observe reconexões**: `[MQTT] Conectando ao broker...`

### Configuração MQTT
- **Broker**: HiveMQ Cloud (TLS 8883)
- **Tópico**: `device1/data`
- **Intervalo**: 3 segundos
- **Autenticação**: Usuário e senha configurados

### Funcionalidades Implementadas

- ✅ **Geração de dados simulados** com variação realística
- ✅ **Conexão WiFi** com timeout e reconexão
- ✅ **MQTT com TLS** para HiveMQ Cloud
- ✅ **Armazenamento local SPIFFS** quando offline
- ✅ **Reconexão automática** e sincronização
- ✅ **JSON estruturado** com metadados completos
- ✅ **Logs detalhados** para debugging
- ✅ **Tratamento de erros** robusto

## Node-RED Integration

O fluxo Node-RED implementado inclui:

- **MQTT Input**: Recebe dados do tópico `device1/data`
- **Data Parser**: Processa JSON e extrai temperatura e BPM
- **Temperature Gauge**: Medidor visual da temperatura (35-40°C)
- **BPM Gauge**: Medidor visual dos batimentos (50-150 BPM)
- **Status Display**: Informações do dispositivo e timestamp
- **Dashboard**: Interface web para visualização em tempo real

**Funcionalidades implementadas:**
- ✅ Recepção MQTT com TLS
- ✅ Parsing automático de dados JSON
- ✅ Gauges coloridos com alertas visuais
- ✅ Dashboard responsivo
- ✅ Debug completo dos dados recebidos

## Relatório

Para mais detalhes, consulte o relatório: [report.md](./assets/report.md)
