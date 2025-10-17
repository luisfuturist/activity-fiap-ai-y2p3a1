# TODO – FASE 3: Monitoramento Contínuo – IoT na Saúde

## PARTE 1 – Armazenamento e Processamento Local (Edge Computing)

### Desenvolvimento (Wokwi/ESP32)

- [ ] Configurar projeto no Wokwi com ESP32.
- [ ] Adicionar sensor obrigatório: DHT22 (temperatura e umidade).
- [ ] Adicionar segundo sensor (livre escolha, ex: botão para simular BPM, LDR, etc.).
- [ ] Implementar leitura periódica dos sensores.
- [ ] Implementar armazenamento local (SPIFFS) para salvar cada leitura.
- [ ] Simular conectividade Wi-Fi (usar variável booleana `isConnected`).
- [ ] Implementar lógica de resiliência offline:
    - [ ] Se `isConnected == false`, continuar coletando e salvando dados no SPIFFS.
    - [ ] Definir estratégia de armazenamento limitado (quantas amostras) antes de sobrescrever dados antigos.
- [ ] Implementar lógica de sincronização:
    - [ ] Se `isConnected == true`, ler dados do SPIFFS.
    - [ ] Enviar dados armazenados via `Serial.println` (simulando envio para nuvem).
    - [ ] Apagar o arquivo local (SPIFFS) após o envio bem-sucedido.

### Entregáveis (Parte 1)

- [ ] Link do projeto funcional no Wokwi.
- [ ] Código C++ (ESP32) limpo e comentado.
- [ ] Relatório (Parte 1 - mínimo 1 página):
    - [ ] Descrever o fluxo de funcionamento geral.
    - [ ] Detalhar a lógica de resiliência offline e sincronização.
    - [ ] Explicar a estratégia de armazenamento limitado escolhida.

## PARTE 2 – Transmissão para Nuvem e Visualização (Fog/Cloud)

### Desenvolvimento (MQTT & Node-RED)

- [ ] Configurar um broker MQTT (ex: HiveMQ Cloud).
- [ ] Modificar código do ESP32 (Wokwi) para enviar dados (dos 2 sensores) via MQTT para o broker.
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

## IR ALÉM 1 – Comunicação com REST e E-mail (Opcional)

- [ ] Desenvolver script em Python.
- [ ] Implementar cliente REST para enviar dados (simulando o ESP32) para uma API (pode ser mockada ou simples).
- [ ] Implementar cliente REST para consumir dados da API.
- [ ] Implementar lógica de verificação de risco no script (ex: taquicardia, febre, ausência de movimento).
- [ ] Implementar automação de disparo de e-mail (ex: usando `smtplib`) quando um risco for detectado.
- [ ] Escrever Relatório (Ir Além 1 - 1 a 2 páginas) descrevendo o fluxo implementado.
- [ ] Organizar o código Python e o relatório.

## IR ALÉM 2 – IA em Séries Temporais (Opcional)

- [ ] Criar um Notebook Python (Jupyter ou Google Colab).
- [ ] Obter ou gerar um dataset de séries temporais de sinais vitais.
- [ ] Implementar um classificador tradicional (ex: Regressão Logística, SVM) para análise.
- [ ] Implementar uma rede neuromórfica simples (ex: modelo LIF ou FHN).
- [ ] Treinar e comparar o desempenho dos dois modelos.
- [ ] Criar repositório GitHub público para o projeto.
- [ ] Adicionar o Notebook (comentado) ao repositório.
- [ ] Escrever um `README.md` claro no repositório.
- [ ] Escrever Relatório Comparativo (Ir Além 2 - 2 páginas) sobre o desempenho dos modelos, vantagens e limitações.
- [ ] Gravar vídeo de apresentação (máx. 4 minutos) explicando os resultados e a análise.
- [ ] Fazer upload do vídeo no YouTube (Não Listado).
- [ ] Adicionar o link do vídeo ao `README.md`.
