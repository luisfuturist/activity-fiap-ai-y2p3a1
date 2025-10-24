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

## Preparação do Ambiente no VSCode

Se você preferir testar o projeto localmente, você pode usar o VSCode com o PlatformIO e rodar o projeto no Wokwi com as mesmas instruções acima, mas antes de rodar o projeto, você precisa instalar as extensões [PlatformIO](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) e [Wokwi](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode) no VSCode.

1.  Instale o PlatformIO CLI (caso não tenha instalado):
    ```bash
    python -m venv .venv
    source .venv/bin/activate
    pip install platformio
    ```

2. Compile o projeto:
    ```bash
    pio run
    ```

## Como Testar o Sistema

### Teste de Funcionamento Offline
1. **Aguarde o modo offline** (WiFi: DISCONNECTED)
2. **Observe o acúmulo de dados**: `💾 Stored locally (X/100)`
3. **Pressione o botão** para simular batimentos e ver `❤️ Beat: X`
4. **Aguarde o cálculo de BPM**: `💓 BPM: X` a cada 10 segundos

### Teste de Sincronização
1. **Aguarde a conexão WiFi** (a cada 30s): `📡 WiFi: CONNECTED`
2. **Observe a sincronização**: `🔄 Syncing X samples...`
3. **Verifique o envio em lote**: `📤 Batch syncing X samples...`
4. **Confirme a limpeza**: `✅ Synced X samples - local storage cleared`

### Teste de Resiliência
1. **Desconecte o WiFi** (aguarde 30s): `📡 WiFi: DISCONNECTED`
2. **Continue coletando dados** offline
3. **Reconecte** e veja a sincronização automática

**Funcionalidades Implementadas e Testadas:**

- ✅ **Leitura periódica do sensor DHT22** (temperatura e umidade a cada 2s)
- ✅ **Simulação de batimentos cardíacos** com botão (cálculo de BPM a cada 10s)
- ✅ **Cálculo de BPM** (batimentos por minuto) baseado em pulsos do botão
- ✅ **Exibição dos dados** no monitor serial com emojis e formatação clara
- ✅ **Tratamento de erros** de leitura do sensor DHT22
- ✅ **Debounce do botão** para evitar leituras falsas (50ms)
- ✅ **Simulação de conectividade WiFi** (alterna a cada 30s)
- ✅ **Armazenamento local no SD Card** (Edge Computing)
- ✅ **Resiliência offline** com buffer circular (100 amostras)
- ✅ **Sincronização automática** de dados quando WiFi conecta
- ✅ **Estratégia FIFO** com buffer circular para gerenciar limite de amostras
- ✅ **Sincronização em lote** eficiente (envia todos os dados de uma vez)
- ✅ **Limpeza automática** do armazenamento local após sincronização

## Relatório

Para mais detalhes, consulte o relatório: [report.md](./assets/report.md)
