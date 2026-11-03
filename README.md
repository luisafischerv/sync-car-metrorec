# sync-car-metrorec

Implementação 5 – Sincronização do Metrô (Infraestrutura de Software, CESAR School).

## Descrição

Sistema de embarque da METROREC em que cada vagão e cada passageiro é uma thread.
A estação é implementada como um **monitor** (Silberschatz, Cap. 6, Seção 6.7, montado com Pthreads como na Seção 7.3.3):
um único `pthread_mutex_t` protege três contadores, e duas variáveis de condição fazem passageiros e vagão
dormirem, sem busy-wait, até o evento que esperam acontecer.

- `station_init` – inicializa o mutex, as condições e zera os contadores.
- `station_wait_for_car` – o passageiro espera (em `car_arrived`) até haver vagão com assento livre e reserva um assento.
- `station_on_board` – registra que um passageiro sentou; quando o último senta, acorda o vagão (`signal` em `all_boarded`).
- `station_load_car` – o vagão abre as portas, acorda os passageiros (`broadcast`) e espera até todos sentarem e o vagão estar cheio ou sem ninguém esperando.

## Arquivos

| Arquivo | Descrição |
| --- | --- |
| `sync_car.h` | Declaração da `struct station` e protótipos das 4 funções. A struct fica no header porque o runner declara `struct station` na pilha e precisa do seu tamanho. |
| `sync_car.c` | Implementação das funções `station_init`, `station_wait_for_car`, `station_on_board` e `station_load_car`. |
| `sync_car-runner.c` | Programa de teste fornecido pelo professor (não alterado). |
| `test_busywait.c` | Teste auxiliar: 10 passageiros esperando sem vagão por 3 s; mede o tempo de CPU do processo para provar que não há busy-wait. |
| `test_limites.c` | Teste auxiliar: caso inválido (`count` negativo) e caso limite (mais assentos que passageiros). |
| `Makefile` | Compila o executável `sync_car` (`make`) e remove os compilados (`make clean`). |
| `evidencias.log` | Registro das sessões de terminal (`script -a`), usado como evidência no relatório. |

## Como compilar

```bash
make clean
make
```

Gera o executável `sync_car` (runner do professor + `sync_car.c`).

## Como executar e testar

Teste principal (runner do professor):

```bash
./sync_car
```

Saída esperada no final: `Looks good!`

Teste de repetição (bugs de concorrência são intermitentes):

```bash
for i in $(seq 1 50); do ./sync_car > /dev/null || echo "FALHOU na execucao $i"; done
```

Nenhuma linha `FALHOU` deve aparecer.

Testes auxiliares:

```bash
gcc -pthread test_busywait.c sync_car.c -o test_busywait && ./test_busywait
gcc -pthread test_limites.c sync_car.c -o test_limites && ./test_limites
```

Saídas esperadas: `OK: sem busy-wait` e `OK: casos limite e invalido passaram`.

## Sistema operacional

Desenvolvido e testado no Windows com WSL2:

- Distribuição: `<saída do lsb_release -d>`
- Compilador: `<saída do gcc --version>`
- Kernel: Linux 6.18.33.2-microsoft-standard-WSL2 x86_64