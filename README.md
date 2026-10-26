# ATLAS — A Maldição do Tesouro Perdido

**Um jogo de caça ao tesouro, estratégia e investigação desenvolvido em linguagem C.**

---

## 1. Informações do projeto

| Informação | Descrição |
|---|---|
| Projeto | ATLAS — A Maldição do Tesouro Perdido |
| Instituição | FACAPE |
| Curso | Ciência da Computação |
| Disciplina | Programação Avançada |
| Período | 2026.2 |
| Professor | Thierry Araujo |
| Tema | J4 — Caça ao Tesouro |
| Linguagem | C |
| Interface | Terminal (CLI) |
| Versão | 0.3 — Cadastro de explorador e base do sistema de tesouros |
| Status | Em desenvolvimento |

O ATLAS é um projeto acadêmico desenvolvido por quatro estudantes de Ciência da Computação da FACAPE.

A proposta é criar um jogo que combine investigação, raciocínio lógico e descoberta de tesouros escondidos, utilizando os principais conceitos da linguagem C e da disciplina de Programação Avançada.

Além do objetivo acadêmico, pretendemos construir uma base organizada para que o projeto possa ser expandido futuramente e utilizado no portfólio dos integrantes.

---

## 2. Sobre o jogo

Em **ATLAS — A Maldição do Tesouro Perdido**, o jogador assume o papel de um explorador que precisa localizar tesouros escondidos em diferentes regiões de uma ilha misteriosa.

Cada expedição possui um tabuleiro com posições ocultas, uma quantidade definida de tesouros e um limite de tentativas.

O jogador deverá investigar coordenadas, interpretar informações obtidas durante a partida e localizar todos os tesouros antes de esgotar suas oportunidades.

A proposta é que a experiência não dependa exclusivamente da sorte, mas também da capacidade de analisar pistas e decidir estrategicamente onde investigar.

O projeto busca inspiração na atmosfera dos jogos clássicos de terminal, valorizando interfaces textuais, mapas ASCII e uma ambientação própria.

### Objetivos de jogabilidade

- Localizar todos os tesouros escondidos.
- Investigar posições por coordenadas.
- Utilizar pistas para orientar novas investigações.
- Administrar uma quantidade limitada de tentativas.
- Concluir expedições em diferentes dificuldades.
- Registrar resultados em um ranking.
- Salvar partidas e continuar posteriormente.

O jogo será executado no terminal, sem movimentação de personagem pelo mapa.

---

## 3. Equipe de desenvolvimento

O ATLAS é desenvolvido colaborativamente pelos quatro integrantes abaixo, com responsabilidade compartilhada pela qualidade do projeto.

| Integrante | Participação |
|---|---|
| Guilherme de Almeida | Equipe de desenvolvimento |
| Guilherme Arthur | Equipe de desenvolvimento |
| Marcio | Equipe de desenvolvimento — nome completo pendente |
| Enzo | Equipe de desenvolvimento — nome completo pendente |

As contribuições efetivamente realizadas são registradas por etapa no histórico abaixo e nos commits, sem estabelecer hierarquia entre os integrantes.

### 3.1. Histórico de contribuições

| Data | Etapa | Atividades registradas | Responsável pela etapa |
|---|---|---|---|
| 21/10/2026 | Estrutura inicial (primeiro commit) | Organização do repositório e dos módulos, menu principal inicial, definições de dificuldade e README | Guilherme de Almeida |
| 22/10/2026 | Integração das dificuldades | Integração do menu à seleção, validação de entradas, implementação de `getGameConf()` e testes das configurações EASY, NORMAL e HARD | Guilherme de Almeida |
| Até 26/10/2026 | Núcleo de caça ao tesouro | Desenvolvimento de `treasure.c`/`treasure.h`: distribuição e investigação de tesouros, controle de tentativas, salvamento/carregamento e primeira versão do ranking | Marcio |
| 26/10/2026 | Cadastro do explorador | Ajustes e depuração de `player.c`/`player.h`, validação de nomes e integração do cadastro após a seleção da dificuldade no menu | Guilherme de Almeida |

Novas contribuições serão incluídas conforme as entregas de cada integrante. A responsabilidade pelas implementações e revisões é compartilhada por toda a equipe.

### 3.2. Colaboração e autoria

O histórico de commits e as revisões no GitHub serão utilizados para identificar alterações, correções e novas funcionalidades. Cada integrante utilizará sua própria identidade Git e deverá compreender o código com o qual contribuir.

---

## 4. Funcionalidades desenvolvidas

### 4.1. Menu principal

Foi criada a estrutura inicial do menu principal, responsável por apresentar as funcionalidades disponíveis e interpretar a opção selecionada pelo jogador.

A interface atual segue este modelo:

```text

\===========================================

                 A T L A S

       A MALDICAO DO TESOURO PERDIDO

\===========================================

         1 - Nova Expedicao

         2 - Continuar Expedicao

         3 - Manual do Explorador

         4 - Ranking dos Exploradores

         0 - Sair

\===========================================

Escolha uma opcao:

```

O módulo utiliza:

- `printf()` para exibição das opções;
- `scanf()` para leitura inicial;
- `switch` para identificar a escolha;
- `do while` para repetir o menu;
- `getchar()` para descartar caracteres após determinadas falhas de leitura.

As opções ainda não integradas aos respectivos módulos apresentam mensagens temporárias de desenvolvimento.

A validação completa de entradas, incluindo sequências como `1abc`, ainda está pendente.

### 4.2. Seleção de dificuldades e configuração da partida

A opção **Nova Expedição** do menu principal já está integrada à seleção de dificuldade. O jogador pode escolher uma opção pelo número ou pelo nome, sem diferenciação entre letras maiúsculas e minúsculas.

| Dificuldade | Entradas aceitas |
|---|---|
| Fácil | `1`, `easy` |
| Normal | `2`, `normal` |
| Difícil | `3`, `hard` |
| Voltar | `0`, `back` |

O módulo utiliza `fgets()` para leitura, `strcspn()` para remover a quebra de linha, `tolower()` para normalizar caracteres e `strcmp()` com operadores ternários para reconhecer a opção. As dificuldades são representadas pelo `enum difficulty`.

| Função | Responsabilidade atual |
|---|---|
| `showDifficulty()` | Mostrar as opções e capturar a escolha do jogador |
| `difficultyLevel()` | Normalizar e interpretar a entrada, retornando `EASY`, `NORMAL`, `HARD`, `BACK` ou `INVALID` |
| `getGameConf()` | Retornar a estrutura `gameConfig` com as configurações da dificuldade escolhida |

O `menu.c` utiliza temporariamente uma etapa de limpeza dos caracteres restantes após o `scanf()` para que a leitura com `fgets()` funcione. Ainda é necessário aprimorar a validação de entradas completas e a repetição automática diante de dificuldades inválidas.

**Estado atual (26/10/2026):** seleção da dificuldade e cadastro do explorador integrados e verificados no terminal. A opção Nova Expedição ainda não chama o laço jogável implementado em `treasure.c`.


### 4.3. Cadastro do explorador

Os módulos `player.c` e `player.h` definem a estrutura `Player`, com nome, pontuação inicial, dificuldade, ponteiro para uma expedição futura e ponteiro `next` para uma possível lista encadeada. A função `infoPlayer()` solicita e valida o nome usando `fgets()`, rejeita nomes vazios ou compostos somente por espaços e inicializa os campos básicos.

Em 26/10/2026 foram realizados testes isolados de cadastro e validação de nomes. O fluxo **Menu → Dificuldade → Cadastro** também foi testado com sucesso. A associação do `Player` ao módulo de tesouros e ao ranking ainda está pendente.

### 4.4. Núcleo de caça ao tesouro (implementação de Marcio)

Os módulos `treasure.c` e `treasure.h` já incluem geração aleatória de tesouros sem posições duplicadas, coleção dinâmica de tesouros com `realloc()`, investigação de coordenadas, marcação de casas conhecidas, contagem de tentativas, verificação de vitória/derrota e exibição do tabuleiro no terminal.

Também há rotinas para salvar/carregar o estado da partida em `partida.sav` e registrar/exibir pontuações em `ranking.txt`. O laço de partida é exposto por `runTreasureHunt()` e `resumeTreasureHunt()`, mas ainda não está ligado ao menu principal. A implementação deve ser considerada **base em revisão**, e não funcionalidade final validada.

---

## 5. Níveis de dificuldade

O projeto possuirá três dificuldades, cada uma com configurações próprias.

| Configuração | Easy | Normal | Hard |
|---|---:|---:|---:|
| Dimensões do mapa | 6 × 6 | 8 × 8 | 10 × 10 |
| Quantidade de casas | 36 | 64 | 100 |
| Tesouros | 3 | 5 | 7 |
| Tentativas | 30 | 45 | 60 |
| Usos da bússola | 3 | 2 | 1 |

Essas configurações foram adotadas como parâmetros iniciais do projeto e poderão ser ajustadas após testes de jogabilidade.

### 5.1. Representação com enum

As dificuldades são representadas internamente pelo tipo:

```c

typedef enum {

    INVALID = -1,

    BACK = 0,

    EASY = 1,

    NORMAL = 2,

    HARD = 3

} difficulty;

```

Essa estrutura permite identificar cada dificuldade através de constantes nomeadas.

A entrada textual fornecida pelo jogador é interpretada e convertida para uma dessas constantes.

### 5.2. Configuração com struct

As características de cada dificuldade serão armazenadas em:

```c

typedef struct {

    int rows;

    int columns;

    int treasures;

    int attempts;

    int compasses;

} gameConfig;

```

Cada campo representa uma característica da partida.

Por exemplo, a dificuldade Fácil terá seis linhas, seis colunas, três tesouros, 30 tentativas e três usos da bússola.

A função `getGameConf()` retorna essas informações para as dificuldades válidas. A aplicação efetiva desses parâmetros ao tabuleiro dependerá da futura `runGame()`.

---

## 6. Regras obrigatórias do Caça ao Tesouro

O ATLAS seguirá as regras estabelecidas no tema **J4 — Caça ao Tesouro** do projeto da disciplina.

### Regras principais

1\. O jogador não se movimenta pelo tabuleiro.

2\. Cada jogada consiste na escolha de uma coordenada.

3\. Os tesouros permanecem ocultos até serem encontrados.

4\. As posições investigadas permanecem identificadas.

5\. Selecionar uma posição já explorada não consome outra tentativa.

6\. Cada partida possui uma quantidade limitada de tentativas.

7\. A vitória ocorre quando todos os tesouros são encontrados antes do fim das tentativas.

8\. A derrota ocorre quando as tentativas terminam sem que todos os tesouros tenham sido encontrados.

9\. Os tesouros devem ser armazenados em uma coleção dinâmica separada da representação do tabuleiro.

### Funcionalidades obrigatórias

O programa deverá permitir:

- Iniciar uma partida.
- Gerar ou carregar um tabuleiro.
- Exibir o estado conhecido pelo jogador.
- Investigar coordenadas.
- Revelar o resultado de cada investigação.
- Contabilizar tesouros encontrados.
- Contabilizar tentativas restantes.
- Salvar uma partida em andamento.
- Retomar uma partida salva.
- Manter um ranking de resultados.

### Limitações do tema

Conforme o enunciado, o jogo não possuirá:

- Movimentação de personagem;
- Inventário;
- Interface gráfica;
- Regras semelhantes às do tema J1 — Jogo de Exploração em Mapa.

O mapa será utilizado como representação de locais investigáveis, não como um ambiente de movimentação de personagem.

---

## 7. Sistema de mapas — Implementação inicial e evolução prevista

O módulo `TreasureGame`, declarado em `treasure.h`, mantém o tabuleiro conhecido pelo jogador, com dimensões definidas pela dificuldade. As posições não investigadas são representadas por `.`, as casas vazias por `o` e os tesouros encontrados por `$`.

**Limitação técnica:** o tabuleiro atual usa a matriz fixa `char board[TREASURE_MAX_SIZE][TREASURE_MAX_SIZE]`, com limite de 10×10; ainda precisa ser convertido para **matriz dinâmica**, conforme o requisito do tema J4. A apresentação de terrenos e o aprimoramento visual permanecem apenas como ideias futuras. O jogador investiga coordenadas, sem movimentação de personagem.

---

## 8. Sistema de tesouros — Implementação inicial

Marcio implementou, em `treasure.c` e `treasure.h`, uma coleção dinâmica de estruturas `Treasure` separada do tabuleiro. A coleção cresce por meio de `realloc()`, mantém linha, coluna e estado de descoberta de cada tesouro e é liberada por `freeTreasureGame()`.

As funções `startTreasureGame()`, `investigateCoordinate()`, `getTreasureState()` e `showKnownBoard()` compõem o núcleo de geração, investigação, verificação de vitória/derrota e apresentação do estado conhecido. Coordenadas repetidas não consomem novas tentativas.

**Pendências:** integrar a partida ao menu e ao `Player`, converter o tabuleiro para matriz dinâmica, aprimorar a validação de entrada e impedir investigações realizadas diretamente pela API depois do encerramento da partida. Radar e bússola ainda não foram implementados.

---

## 9. Sistema de pistas — Funcionalidade adicional planejada

O ATLAS terá um sistema de pistas para reduzir a dependência exclusiva da sorte e tornar a investigação mais estratégica.

Essa será a principal proposta de funcionalidade adicional da equipe.

### 9.1. Radar de proximidade

Ao investigar uma coordenada vazia, o jogador poderá receber uma informação sobre quantos tesouros existem nas oito posições vizinhas.

Por exemplo:

```text

Coordenada investigada: C4

Nenhum tesouro encontrado.

RADAR:

Existem 2 tesouros nas proximidades.

```

A informação deverá ser calculada com base nas posições reais dos tesouros.

Os tesouros já encontrados continuarão sendo considerados na contagem, mantendo os resultados consistentes durante a partida.

### 9.2. Bússola do Explorador

A bússola será um recurso de dica com quantidade limitada de utilizações.

Sua função será fornecer informações adicionais sobre a localização aproximada de tesouros ainda ocultos.

O número de utilizações dependerá da dificuldade selecionada.

A bússola será representada por um contador de dicas, sem criar um sistema de inventário.

**Estado atual:** funcionalidades planejadas, ainda não implementadas.

---

## 10. Manual do Explorador — Planejamento

O jogo contará com um manual externo, armazenado em arquivo de texto.

**Arquivo previsto:** `data/manual.txt`

O manual apresentará as principais informações sobre o universo e o funcionamento do ATLAS.

### Conteúdo previsto

1\. Introdução e história.

2\. Objetivo do jogo.

3\. Regras de investigação.

4\. Controles e coordenadas.

5\. Dificuldades.

6\. Tipos de terreno.

7\. Tesouros possíveis.

8\. Funcionamento das pistas.

9\. Condições de vitória e derrota.

10\. Salvamento, carregamento e ranking.

11\. Dicas gerais para iniciantes.

12\. Créditos.

A proposta é que o manual também contribua para a ambientação do jogo, com uma apresentação inspirada nos manuais de jogos clássicos de computador.

O acesso será realizado pelo menu principal.

**Estado atual:** planejado, ainda não implementado.

---

## 11. Persistência e ranking — Primeira versão implementada

### 11.1. Salvamento e carregamento

O código de Marcio inclui `saveTreasureGame()` e `loadTreasureGame()`, com escrita e leitura do arquivo `partida.sav` no diretório de execução. O formato atual guarda dimensões, tentativas, coleção de tesouros, estado de descoberta e casas reveladas. As funções `runTreasureHunt()` e `resumeTreasureHunt()` permitem iniciar ou retomar o laço de jogo em nível de módulo.

Ainda é necessário integrar a opção **Continuar Expedição** ao menu e persistir os dados de identificação do jogador, dificuldade e futuras cargas da bússola. A validação de arquivos corrompidos e a segurança de gravações também precisam ser fortalecidas.

### 11.2. Ranking

`treasure.c` já apresenta `registerRanking()` e `showRanking()`, além de leitura do `ranking.txt`, ordenação com `qsort()` e retenção de até dez resultados gerais. A pontuação implementada considera 100 pontos por tesouro e, em caso de vitória, dez pontos adicionais por tentativa restante.

**Regra acordada ainda não implementada:** manter a melhor pontuação por **jogador e dificuldade**, com consulta separada por dificuldade. O código atual ainda solicita o nome ao final da partida, de forma independente do cadastro em `player.c`.

**Estado:** persistência iniciada, ainda sem teste completo de integração no fluxo do menu.

---

## 12. Arquitetura do projeto

### 12.1. Estrutura atual dos módulos recebidos

```text
ATLAS-Treasure-Hunt/
|-- include/
|   |-- menu.h
|   |-- difficulty.h
|   |-- player.h
|   `-- treasure.h
|-- src/
|   |-- main.c
|   |-- menu.c
|   |-- difficulty.c
|   |-- player.c
|   `-- treasure.c
|-- test/
|   `-- test_player.c
`-- README.md
```

A pasta `test/` foi utilizada na depuração local do cadastro; confirmar seu conteúdo com `git status` antes de adicioná-la ao commit. Outros arquivos do projeto, como os do manual, podem existir na cópia local e não foram incluídos nesta listagem dos módulos analisados.

### 12.2. Responsabilidades

| Módulo | Responsabilidade |
|---|---|
| `main.c` | Chamar o menu principal |
| `menu.c/.h` | Exibir menu, escolher dificuldade e iniciar cadastro |
| `difficulty.c/.h` | Ler dificuldade e produzir `gameConfig` |
| `player.c/.h` | Validar e inicializar os dados do explorador |
| `treasure.c/.h` | Gerar e investigar tesouros, exibir tabuleiro conhecido, salvar/carregar e registrar ranking |

### 12.3. Organização futura

Separar melhor orquestração da partida, lógica do tabuleiro, exibição e persistência conforme a evolução e a necessidade. O código atual ainda reúne várias dessas responsabilidades no módulo `treasure.c`.

### 12.4. TAD e memória dinâmica

A coleção dinâmica de tesouros já emprega ponteiros e `realloc()`/`free()`. Ainda falta migrar o tabuleiro para matriz dinâmica e avaliar os requisitos de encapsulamento e TAD do projeto.

---

## 13. Tecnologias e conceitos utilizados

### Linguagem e ferramentas

- Linguagem C;
- GCC;
- Visual Studio Code;
- Git;
- GitHub;
- Interface de terminal.

### Conceitos aplicados ou previstos

- Funções;
- Estruturas condicionais;
- Estruturas de repetição;
- Vetores de caracteres;
- Manipulação de strings;
- `enum`;
- `struct`;
- `typedef`;
- Ponteiros;
- Memória dinâmica;
- TAD;
- Encapsulamento;
- Manipulação de arquivos;
- Tratamento de erros;
- Modularização.

O desenvolvimento buscará compatibilidade entre Windows e Linux, utilizando principalmente a biblioteca padrão de C.

---

## 14. Limitações atuais

Em 26/10/2026, o menu principal, a seleção de dificuldade e o cadastro do explorador já funcionam juntos. O núcleo jogável, desenvolvido em módulo separado por Marcio, ainda não foi integrado ao menu.

Pendências identificadas:

- Integrar `runTreasureHunt()` e `resumeTreasureHunt()` ao fluxo do menu e aproveitar o `Player` já cadastrado.
- Converter a matriz fixa do tabuleiro para matriz dinâmica.
- Vincular nome e dificuldade do jogador ao salvamento e ao ranking.
- Corrigir a regra do ranking para melhor pontuação por jogador e dificuldade.
- Reforçar validação do arquivo salvo e o tratamento de falhas de gravação.
- Impedir ações pela API de investigação após vitória ou derrota.
- Melhorar entradas inválidas, EOF, comandos e nomes acima do limite.
- Concluir ou integrar o Manual do Explorador, cuja implementação foi delegada.
- Implementar radar e bússola no estágio apropriado.
- Executar testes integrados da partida completa e registrar resultados.

A documentação distingue código existente de funcionalidades efetivamente integradas e testadas.

---

## 15. Uso de Inteligência Artificial

Ferramentas de Inteligência Artificial generativa estão sendo utilizadas como apoio ao desenvolvimento do ATLAS.

O uso inclui:

- Planejamento e organização do projeto;
- Discussão de arquitetura e divisão de módulos;
- Explicações sobre conceitos da linguagem C;
- Orientação sobre `typedef`, `enum` e `struct`;
- Revisão de código e identificação de erros;
- Sugestões de funcionalidades;
- Planejamento do sistema de dificuldades;
- Apoio à elaboração da documentação.

A IA é utilizada como ferramenta de consulta, orientação e revisão.

A responsabilidade pela compreensão, implementação, revisão e teste do código entregue pertence à equipe.

Todos os integrantes deverão ser capazes de explicar as decisões técnicas e o funcionamento dos módulos desenvolvidos.

---

## 16. Contribuições e controle de versão

O desenvolvimento será registrado por meio de commits no GitHub.

As mensagens deverão identificar claramente as alterações realizadas.

Exemplos:

```text

chore: initialize ATLAS project structure

feat: implement main menu

feat: add difficulty selection

fix: validate invalid menu input

feat: implement dynamic board

feat: add treasure generation

docs: update game manual

```

Cada integrante deverá configurar sua própria identidade Git.

O projeto utilizará inicialmente a branch `main`, conforme a organização definida pela equipe.

O histórico de commits servirá como registro técnico da evolução do ATLAS e das contribuições de seus desenvolvedores.

---

## 17. Próximas tarefas — Checkpoint 2 (27/10/2026)

O Checkpoint 2 exige: **principais regras do tema funcionando; memória dinâmica integrada; maior parte das funcionalidades mínimas implementada; persistência iniciada ou funcional; interface utilizável**.

Prioridades imediatas:

1. Integrar menu, dificuldade, cadastro e núcleo de caça ao tesouro.
2. Implementar matriz dinâmica do tabuleiro, preservando a coleção dinâmica de tesouros.
3. Demonstrar investigação por coordenadas, repetição sem custo, vitória e derrota.
4. Conectar o carregamento/salvamento e o ranking às opções do menu.
5. Revisar entradas, memória, falhas e executar uma demonstração de ponta a ponta.

A refatoração do ranking para melhor pontuação por jogador e dificuldade permanece uma próxima tarefa, **não uma entrega já concluída**. Recursos extras não devem substituir os requisitos centrais do checkpoint.

---

## 18. Créditos

**ATLAS — A Maldição do Tesouro Perdido**

Projeto desenvolvido em equipe por:

- Guilherme de Almeida
- Guilherme Arthur
- Marcio
- Enzo

**FACAPE — Ciência da Computação**  
**Programação Avançada — 2026.2**  
**Professor: Thierry Araujo**

As atividades realizadas por integrante são registradas no histórico de contribuições e no GitHub.

---
**ATLAS — A Maldição do Tesouro Perdido**

**Nem todo tesouro deve ser encontrado. Mas todos precisam ser procurados.**