# TODO

## PARTE 1 – Armazenamento e Processamento Local (Edge Computing)

### Desenvolvimento (Wokwi/ESP32)

- [x] Configurar projeto no Wokwi com ESP32.
- [x] Adicionar sensor obrigatório: DHT22 (temperatura e umidade).
- [x] Adicionar segundo sensor (livre escolha, ex: botão para simular BPM, LDR, etc.).
- [x] Implementar leitura periódica dos sensores.
- [x] Implementar armazenamento local (SPIFFS) para salvar cada leitura.
- [x] Simular conectividade Wi-Fi (usar variável booleana `isConnected`).
- [x] Implementar lógica de resiliência offline:
    - [x] Se `isConnected == false`, continuar coletando e salvando dados no SPIFFS.
    - [x] Definir estratégia de armazenamento limitado (quantas amostras) antes de sobrescrever dados antigos.
- [x] Implementar lógica de sincronização:
    - [x] Se `isConnected == true`, ler dados do SPIFFS.
    - [x] Enviar dados armazenados via `Serial.println` (simulando envio para nuvem).
    - [x] Apagar o arquivo local (SPIFFS) após o envio bem-sucedido.

### Entregáveis (Parte 1)

- [x] Link do projeto funcional no Wokwi.
- [x] Código C++ (ESP32) limpo e comentado.
- [x] Relatório (Parte 1 - mínimo 1 página):
    - [x] Descrever o fluxo de funcionamento geral.
    - [x] Detalhar a lógica de resiliência offline e sincronização.
    - [x] Explicar a estratégia de armazenamento limitado escolhida.
