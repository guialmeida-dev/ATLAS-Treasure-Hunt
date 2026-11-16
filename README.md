# ATLAS — A Maldição do Tesouro Perdido

Jogo de caça ao tesouro em **C11**, executado no terminal.

**FACAPE — Ciência da Computação — Programação Avançada 2026.2**

Professor: Thierry Araujo. 
Tema: **J4 — Caça ao Tesouro**.

## 1. Objetivo

Encontrar todos os tesouros escondidos escolhendo coordenadas, antes de esgotar as tentativas. O jogador não se movimenta pelo mapa; não há inventário ou interface gráfica.

O tema está na página 10 do PDF `Projeto_Programacao_Avancada_2026_2_v3.pdf`; os requisitos comuns estão nas páginas 3–6. O exemplo visual da página 11 é ilustrativo. O projeto utiliza TAD, memória dinâmica, coleção de tesouros separada do mapa, persistência e validação de entradas.

## 2. Compilar e executar

É necessário ter GCC com suporte a C11. Na pasta raiz, execute no PowerShell:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic src/*.c -o atlas.exe
.\atlas.exe
```

Para Linux, a compilação equivalente é:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic src/*.c -o atlas
./atlas
```

A execução foi validada em Windows. Linux ainda não foi testado neste ambiente. Os arquivos de dados ficam no diretório em que o programa é executado.

## 3. Equipe de desenvolvimento

O ATLAS é desenvolvido colaborativamente pelos quatro integrantes abaixo, com responsabilidade compartilhada pela qualidade do projeto.

| Integrante | Participação |
|---|---|
| Guilherme de Almeida | Equipe de desenvolvimento |
| Guilherme Arthur | Equipe de desenvolvimento; implementação dos módulos `manual.c` e `manual.h` |
| Marcio | Equipe de desenvolvimento; implementação dos módulos `treasure.c` e `treasure.h` |
| Enzo | Equipe de desenvolvimento — nome completo pendente |

As contribuições efetivamente realizadas são registradas por etapa no histórico abaixo e nos commits, sem estabelecer hierarquia entre os integrantes.

### 3.1. Histórico de contribuições

| Data | Etapa | Atividades registradas | Responsável pela etapa |
|---|---|---|---|
| 21/10/2026 | Estrutura inicial (primeiro commit) | Organização do repositório e dos módulos, menu principal inicial, definições de dificuldade e README | Guilherme de Almeida |
| 22/10/2026 | Integração das dificuldades | Integração do menu à seleção, validação de entradas, implementação de `getGameConf()` e testes das configurações EASY, NORMAL e HARD | Guilherme de Almeida |
| Até 26/10/2026 | Núcleo de caça ao tesouro | Desenvolvimento de `treasure.c`/`treasure.h`: distribuição e investigação de tesouros, controle de tentativas, salvamento/carregamento e primeira versão do ranking | Marcio |
| 26/10/2026 | Cadastro do explorador | Ajustes e depuração de `player.c`/`player.h`, validação de nomes e integração do cadastro após a seleção da dificuldade no menu | Guilherme de Almeida |
| Até 27/10/2026 | Manual do Explorador | Desenvolvimento de `manual.c`/`manual.h`: submenu, criação de arquivo de texto, abertura externa e tratamento condicional para Windows/Linux; chamada adicionada à opção 3 do menu na versão recebida | Guilherme Arthur |
| 28/10/2026 | Interface Terminal do Explorador | Criação de `board_view.c`/`board_view.h`, tabuleiro textual em três tamanhos, painel do jogador, depuração e testes locais | Guilherme de Almeida |

Novas contribuições serão incluídas conforme as entregas de cada integrante. A responsabilidade pelas implementações e revisões é compartilhada por toda a equipe.

### 3.2. Colaboração e autoria

O histórico de commits e as revisões no GitHub serão utilizados para identificar alterações, correções e novas funcionalidades. Cada integrante utilizará sua própria identidade Git e deverá compreender o código com o qual contribuir.

---

## 4. Como jogar

| Opção do menu | Ação |
|---|---|
| 1 — Nova Expedição | Cadastrar explorador, escolher dificuldade e jogar |
| 2 — Continuar Expedição | Retomar o arquivo `partida.sav` |
| 3 — Manual do Explorador | Ler as instruções ou atualizar `manual.txt` |
| 4 — Ranking dos Exploradores | Consultar os melhores resultados por dificuldade |
| 0 — Sair | Encerrar o programa |

O nome aceita espaços e até 50 bytes. Nomes vazios, longos ou com caracteres de controle são rejeitados. Espaços nas extremidades são retirados. Em UTF-8, uma letra com acento pode ocupar mais de um byte.

A dificuldade aceita número ou nome: `1/easy`, `2/normal`, `3/hard` e `0/back` para voltar. Maiúsculas e minúsculas são aceitas.

| Dificuldade | Mapa | Tesouros | Tentativas | Bússolas | Casas distintas de radar |
|---|---:|---:|---:|---:|---:|
| Easy | 6×6 | 3 | 30 | 3 | 3 |
| Normal | 8×8 | 5 | 45 | 2 | 3 |
| Hard | 10×10 | 7 | 60 | 1 | 3 |

### Mapa e investigação

As colunas usam letras e as linhas usam números. `B3` significa coluna B, linha 3.

- `?`: casa oculta.
- `X`: casa investigada sem tesouro.
- `$`: tesouro conquistado.
- `!`: enigma aguardando resposta no Hard.

Uma casa nova válida consome uma tentativa, com ou sem tesouro. Coordenadas inválidas ou repetidas não gastam tentativas. Encontrar o último tesouro na última tentativa conta como vitória. Se as tentativas acabarem com tesouros pendentes, ocorre derrota.

O painel mostra explorador, dificuldade, descobertas, tentativas, pontos e recursos disponíveis. Depois do encerramento, novas investigações são bloqueadas.

### Comandos durante a partida

```text
B3           investigar uma posição
radar B3     consultar a vizinhança de uma casa vazia já investigada
bussola B3   indicar uma direção a partir de uma posição válida
salvar       gravar a partida
sair         voltar ao menu, com opção de salvar
ajuda        mostrar os comandos
```

O jogo rejeita entradas incompletas ou malformadas, como `B3xyz`, `B0` e números grandes demais. EOF encerra a entrada sem salvar automaticamente; use `salvar` antes de fechar.

## 5. Funcionalidades implementadas

### Radar

O comando conta tesouros nas oito casas vizinhas, incluindo os já encontrados, conforme o planejamento original. São permitidas três casas distintas por partida. Repetir a consulta na mesma casa não gasta outra carga e considera as posições atuais dos tesouros.

O radar só funciona em uma casa vazia já investigada. Não é automático e não desconta tentativas ou pontos.

### Bússola

Indica a direção do tesouro ainda oculto mais próximo da posição escolhida. A distância é a soma das diferenças de linha e coluna. Em empate, escolhe a menor linha e depois a menor coluna.

Pode indicar Norte, Sul, Leste, Oeste, uma combinação ou “Aqui”. Cada consulta válida consome uma bússola. Os limites são 3, 2 e 1, conforme a dificuldade. Não há desconto de tentativas ou pontos.

### Enigmas no Hard

Encontrar um tesouro apresenta uma pergunta aritmética. Resposta correta conquista o tesouro. Resposta incorreta ou malformada marca a casa vazia e realoca o tesouro para uma posição oculta e livre.

A tentativa é gasta apenas uma vez. O enigma é resolvido antes de decidir o resultado da partida. EOF na resposta conta como erro e encerra a entrada sem salvar automaticamente.

### Pontuação e ranking

São 100 pontos por tesouro. Na vitória, somam-se 10 pontos por tentativa restante.

O ranking reutiliza o nome cadastrado e mostra até dez melhores exploradores por dificuldade. Considera o melhor resultado de cada nome exato e dificuldade; maiúsculas diferentes no nome identificam exploradores diferentes. A ordem é pontuação decrescente, vitória, menos tentativas e nome.

Cada partida tem um identificador salvo. O histórico mantém os identificadores dos resultados, inclusive dos inferiores, para impedir registros repetidos. O limite é de 10.000 resultados. Arquivo ausente significa ranking vazio; arquivo inválido é preservado e o erro é informado.

## 6. Salvamento e arquivos

Há um slot, `partida.sav`. Substituir o save de outra partida exige confirmação. Ao terminar, somente o save da própria partida pode ser removido, depois de registrar seu resultado.

O formato `ATLAS_SAVE 2` guarda ID, nome, dificuldade, dimensões, tentativas, tesouros, descobertas, mapa conhecido, recursos e pontuação. O carregamento verifica limites e a consistência entre esses dados. O arquivo termina com `END`.

A linha numérica do save contém: dificuldade, linhas, colunas, máximo de tentativas, tentativas restantes, total de tesouros, encontrados, bússolas restantes, casas de radar usadas e pontos. Em seguida vêm os tesouros (`linha coluna encontrado`), as linhas do mapa (`.`, `o`, `$`), as linhas de radar (`0/1`) e `END`. As coordenadas internas começam em zero.

O formato `ATLAS_RANKING 2` guarda a quantidade de resultados e, para cada um, dificuldade, vitória, encontrados, tentativas usadas, pontos, nome e ID. Também termina com `END`.

As gravações usam `.tmp` e preservam o arquivo anterior em `.bak` durante a troca. Erros de escrita e fechamento são verificados. Auxiliares deixados por interrupção não são sobrescritos. Se o principal estiver ausente, a leitura tenta `.bak`.

Para recuperar uma gravação interrompida, feche o jogo e copie os arquivos antes de alterar qualquer um deles. Se o principal estiver ausente, restaure o `.bak` para seu nome original. Se o principal estiver válido, arquive os auxiliares antes de gravar novamente. Não substitua o principal por um `.tmp` incompleto.

Os formatos antigos são rejeitados e preservados, sem migração automática. A substituição de um save antigo também exige confirmação.

## 7. Organização do projeto

```text
include/   cabeçalhos dos módulos
src/       código do jogo
  main.c         inicia o programa
  menu.c         organiza a navegação
  difficulty.c   configura as dificuldades
  player.c       cadastra o explorador
  treasure.c     mantém a partida, as regras, o save e o ranking
  board_view.c   mostra o mapa e o painel
  manual.c       apresenta e exporta o manual
  screen.c       limpa a tela com cls no Windows e clear no Linux
  input.c        lê e valida entradas
  storage.c      protege a gravação dos arquivos
test/      testes escritos em C
README.md
manual.txt
```

`input` evita repetir a leitura e a validação nos menus. `storage` reúne as verificações usadas para salvar partidas, ranking e manual. Esses módulos foram mantidos porque ajudam a organizar o código.

## 8. Conceitos utilizados

- Funções, `if/else`, `switch`, `for` e `while`.
- `struct`, `enum`, ponteiros, vetores e strings.
- `malloc`, `calloc`, `realloc` e `free`.
- Leitura e escrita de arquivos.
- Modularização com `.c` e `.h`.
- TAD: `TreasureGame` tem seus dados internos definidos somente em `treasure.c`.

O mapa é uma matriz dinâmica contígua: o índice de uma casa é `linha * colunas + coluna`. A coleção de tesouros é um vetor dinâmico separado. A memória é liberada ao sair da partida.

Algumas proteções foram mantidas: `strtol` detecta números grandes demais; um ponteiro temporário protege o `realloc`; `.tmp/.bak` preservam o arquivo anterior; o trecho `fail` fecha o arquivo e libera uma leitura incompleta. O `qsort` utiliza uma função de comparação para ordenar o ranking.

## 9. Testes em C

O jogo e os testes são escritos em C. Não é necessário Python ou outra linguagem para compilar, executar ou testar.

Na raiz, execute no PowerShell:

```powershell
New-Item -ItemType Directory -Force build | Out-Null
gcc -std=c11 -Wall -Wextra -Wpedantic test/test_integration.c -o build/test_integration.exe
.\build\test_integration.exe
```

O executável de integração chama o GCC para compilar o jogo e os testes auxiliares. Depois executa os 45 grupos em pastas próprias dentro de `build/`, sem usar os saves e rankings da pasta principal.

- `test_core.c`: regras, pistas, vitória, derrota e persistência com sementes controladas.
- `test_board.c`: mapas das três dificuldades.
- `test_player.c`: cadastro isolado.
- `test_alloc.c` e `alloc_hooks.h`: simulam falta de memória somente nos testes.
- `test_integration.c`: compila e exercita o executável real pelo menu.

As quatro substituições de alocação em `alloc_hooks.h` existem apenas para testar falhas. Elas não fazem parte do executável normal do jogo.

Resultado da revisão: **45/45 grupos PASSOU**, com compilação completa sem avisos e sem `-Iinclude`. O relatório atual está em `build/c-test-results.txt`; entradas e saídas ficam na pasta `build/c-tests-*` indicada nele. Relatórios JSON antigos foram preservados apenas como histórico.

A cobertura inclui menu, nomes, dificuldades, mapa, investigações, pistas, enigmas, vitória e derrota na última tentativa, reinício e retomada, ranking, arquivos inválidos, EOF, linhas longas, falhas de gravação e de alocação.

## 10. Limitações conhecidas

- Um slot de save por diretório de execução.
- Formatos antigos sem migração automática.
- Nomes limitados por bytes; a codificação depende do terminal.
- Sem suporte a duas instâncias escrevendo os mesmos arquivos.
- Auxiliares após interrupção podem exigir recuperação manual.
- Uma partida nova nunca salva pode ser perdida se o armazenamento falhar ao finalizar.
- Histórico de ranking limitado a 10.000 resultados.
- Linux, disco fisicamente cheio e perda real de energia ainda não foram testados.

## 11. Uso de inteligência artificial

Durante o desenvolvimento, foram utilizadas ferramentas de inteligência artificial como apoio em consultas técnicas, planejamento, revisão, sugestões, documentação, implementação, correção e testes do código.

A equipe é responsável por revisar a versão entregue, compreender seu funcionamento e explicar as decisões durante a apresentação. Os créditos dos módulos originais foram preservados. Não foram feitos commits ou pushes automáticos nesta revisão.
### Limpeza do terminal

A função clearScreen() chama cls no Windows e clear nos demais sistemas, usando comandos fixos, sem entrada do jogador. No Linux, o comando clear deve estar disponível e TERM deve estar configurado. Os testes usam entrada e saída redirecionadas; a aparência da limpeza deve ser conferida também em um terminal interativo.
