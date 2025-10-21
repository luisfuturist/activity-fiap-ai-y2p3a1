# PARTE 1 – Armazenamento e processamento local (Edge Computing)

Esta parte foca na resiliência do dispositivo (Edge), garantindo que ele colete dados mesmo quando estiver offline e os envie quando a conexão for restabelecida.

## Estrutura do Projeto

```
part1/
├── assets/
│   └── report.md              # Relatório da Parte 1
├── data/                      # Diretório para dados armazenados (SPIFFS)
├── src/
│   └── sketch.ino             # Código principal do ESP32
├── diagram.json               # Configuração do circuito no Wokwi
├── platformio.ini             # Configuração do PlatformIO
├── wokwi.toml                 # Configuração do Wokwi
├── TODO.md                    # Lista de tarefas e progresso
└── README.md                  # Este arquivo
```

## Preparação do Ambiente

1.  Instale o PlatformIO:
    ```bash
    python -m venv .venv
    source .venv/bin/activate
    pip install platformio
    ```

2. Rode o projeto:
    ```bash
    pio run
    ```

## Link do Projeto no Wokwi

Aqui está o link para o projeto Wokwi simulando o ESP32 com o sensor DHT22 para leitura de temperatura e umidade.

  * **Link:** `https://wokwi.com/projects/445383801865359361`

**Componentes do Circuito:**

- **ESP32 DevKit C v4**: Microcontrolador principal
- **DHT22**: Sensor de temperatura e umidade (conectado ao pino 15)
- **Push Button**: Simulador de batimentos cardíacos (conectado ao pino 32)

**Instruções no Wokwi:**

1.  Pressione "Play" na simulação.
2.  Observe o monitor serial para ver as leituras de temperatura, umidade do DHT22 e BPM calculado.
3.  Pressione o botão verde para simular batimentos cardíacos.
4.  Simule a mudança da temperatura e umidade do DHT22 alterando o valor dos sensores no painel de controle.

**Funcionalidades Implementadas:**

- ✅ Leitura periódica do sensor DHT22 (temperatura e umidade)
- ✅ Simulação de batimentos cardíacos com botão
- ✅ Cálculo de BPM (batimentos por minuto)
- ✅ Exibição dos dados no monitor serial
- ✅ Tratamento de erros de leitura do sensor
- ✅ Debounce do botão para evitar leituras falsas
- ✅ Simulação de conectividade WiFi
- ✅ Armazenamento local com SPIFFS (Edge Computing)
- ✅ Resiliência offline com buffer circular
- ✅ Sincronização automática de dados
- ✅ Estratégia de armazenamento limitado (100 amostras)

## Relatório

Para mais detalhes, consulte o relatório: [report.md](./assets/report.md)
