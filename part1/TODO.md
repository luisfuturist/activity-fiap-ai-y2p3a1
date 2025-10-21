# TODO

## PARTE 1 – Armazenamento e Processamento Local (Edge Computing)

### Desenvolvimento (Wokwi/ESP32)

- [x] Configurar projeto no Wokwi com ESP32.
- [x] Adicionar sensor obrigatório: DHT22 (temperatura e umidade).
- [x] Adicionar segundo sensor (livre escolha, ex: botão para simular BPM, LDR, etc.).
- [x] Implementar leitura periódica dos sensores.
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
