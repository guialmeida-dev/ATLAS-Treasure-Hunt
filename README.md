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
| Versão | 0.1 — Desenvolvimento inicial |
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

O projeto será desenvolvido colaborativamente por quatro integrantes.

| Nº | Integrante | Identificação |
|---|---|---|
| 1 | **Guilherme de Almeida** | Responsável pela estrutura e implementação inicial |
| 2 | Guilherme Arthur | Integrante |
| 3 | Marcio | Nome completo pendente |
| 4 | Enzo | Nome completo pendente |

As responsabilidades futuras serão definidas conforme a distribuição de tarefas entre os integrantes.

### 3.1. Desenvolvimento inicial

**Versão:** 0.1  
**Data de referência:** 21/10/2026  
**Responsável:** Guilherme de Almeida  
**Marco:** Primeiro commit do projeto

A estrutura inicial do ATLAS foi desenvolvida por Guilherme de Almeida, incluindo:

- Organização inicial dos diretórios e arquivos.
- Criação do arquivo `main.c`.
- Criação dos módulos `menu.c` e `menu.h`.
- Desenvolvimento inicial do menu principal.
- Utilização de `switch` para tratar opções.
- Utilização de `do while` para controlar a repetição do menu.
- Implementação inicial de validação com `scanf()` e `getchar()`.
- Criação dos arquivos `difficulty.c` e `difficulty.h`.
- Definição dos tipos `difficulty` e `gameConfig`.
- Desenvolvimento da interface de seleção de dificuldade.
- Implementação inicial da leitura de entradas com `fgets()`.
- Tratamento de quebras de linha utilizando `strcspn()`.
- Padronização de caracteres utilizando `tolower()`.
- Comparação de entradas utilizando `strcmp()`.
- Desenvolvimento inicial da lógica de identificação das dificuldades.
- Planejamento das configurações de tabuleiro, tesouros, tentativas e dicas.
- Organização inicial da documentação do projeto.

**Observação:** a seleção de dificuldades ainda precisa de integração e testes. A versão 0.1 representa a estrutura inicial em desenvolvimento, não uma versão jogável.

### 3.2. Desenvolvimento colaborativo

Os próximos módulos serão desenvolvidos com a participação da equipe.

O histórico de commits do GitHub será utilizado para registrar a evolução do projeto e identificar as contribuições realizadas por cada integrante.

Cada colaborador deverá utilizar sua própria identidade Git, realizar commits descritivos e compreender as implementações pelas quais é responsável.

A divisão das tarefas não elimina a necessidade de todos os integrantes compreenderem o funcionamento geral da aplicação.

---

## 4. Funcionalidades desenvolvidas

### 4.1. Menu principal

Foi criada a estrutura inicial do menu principal, responsável por apresentar as funcionalidades disponíveis e interpretar a opção selecionada pelo jogador.

A interface atual segue este modelo:

```text
===========================================
                 A T L A S
       A MALDICAO DO TESOURO PERDIDO
===========================================

         1 - Nova Expedicao
         2 - Continuar Expedicao
         3 - Manual do Explorador
         4 - Ranking dos Exploradores
         0 - Sair

===========================================
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

### 4.2. Sistema inicial de dificuldades

Foi iniciada a implementação de um sistema que permite selecionar dificuldades utilizando números ou palavras.

O sistema foi planejado para aceitar:

| Dificuldade | Entradas |
|---|---|
| Fácil | `1`, `easy` |
| Normal | `2`, `normal` |
| Difícil | `3`, `hard` |
| Voltar | `0`, `back` |

As palavras poderão ser informadas com letras maiúsculas ou minúsculas.

Por exemplo:

```text
EASY
Easy
easy
eAsY
```

Todas serão interpretadas como a dificuldade `EASY`, após a padronização da entrada.

A implementação utiliza:

- `fgets()` para receber o texto;
- `strcspn()` para localizar a quebra de linha;
- `tolower()` para converter letras maiúsculas;
- `strcmp()` para comparar os comandos;
- `enum` para representar o resultado da seleção.

O módulo está sendo organizado em três funções:

| Função | Responsabilidade |
|---|---|
| `showDifficulty()` | Exibir o menu de dificuldades |
| `selectDiff()` | Ler, validar e interpretar a escolha |
| `getGameConf()` | Retornar as configurações da dificuldade selecionada |

**Estado atual:** a lógica inicial de interpretação foi elaborada, mas a integração completa e os testes permanecem pendentes.

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

As configurações foram definidas inicialmente pela equipe e poderão ser ajustadas após testes de jogabilidade.

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

A entrada textual fornecida pelo usuário será interpretada e convertida para uma dessas constantes.

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

A função `getGameConf()` será responsável por retornar essas informações.

---

## 6. Regras obrigatórias do Caça ao Tesouro

O ATLAS seguirá as regras estabelecidas no tema **J4 — Caça ao Tesouro** do projeto da disciplina.

### Regras principais

1. O jogador não se movimenta pelo tabuleiro.
2. Cada jogada consiste na escolha de uma coordenada.
3. Os tesouros permanecem ocultos até serem encontrados.
4. As posições investigadas permanecem identificadas.
5. Selecionar uma posição já explorada não consome outra tentativa.
6. Cada partida possui uma quantidade limitada de tentativas.
7. A vitória ocorre quando todos os tesouros são encontrados antes do fim das tentativas.
8. A derrota ocorre quando as tentativas terminam sem que todos os tesouros tenham sido encontrados.
9. Os tesouros devem ser armazenados em uma coleção dinâmica separada da representação do tabuleiro.

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

## 7. Sistema de mapas — Planejamento

O sistema de mapas utilizará uma matriz dinâmica para representar o tabuleiro.

A arquitetura pretende separar a representação lógica da apresentação visual.

### 7.1. Mapa lógico

Será responsável pelas informações internas da partida, como:

- Dimensões;
- Tipos de terreno;
- Casas investigadas;
- Estado das posições;
- Informações necessárias à investigação.

Os tesouros serão mantidos também em uma coleção dinâmica independente, conforme o requisito específico do J4.

### 7.2. Mapa visual

Será responsável pela representação textual do mapa no terminal.

A proposta visual é inspirada em jogos clássicos baseados em caracteres ASCII.

Os mapas poderão apresentar diferentes tipos de terreno:

| Símbolo proposto | Terreno |
|---|---|
| `~` | Mar |
| `.` | Praia ou terra |
| `F` | Floresta |
| `^` | Montanha |
| `R` | Ruínas |
| `C` | Caverna |

Esses símbolos são provisórios.

A presença de diferentes terrenos não implicará movimentação, obstáculos de percurso ou coleta de itens.

Todas as coordenadas poderão inicialmente ser investigadas, incluindo posições marítimas, que poderão representar naufrágios.

### 7.3. Representação das descobertas

Durante a partida, o mapa deverá identificar visualmente as posições exploradas.

A interface poderá utilizar:

- `?` para uma posição oculta;
- `X` para uma posição investigada sem tesouro;
- `T` para um tesouro encontrado.

A representação final deverá conciliar o terreno visual com as informações descobertas pelo jogador.

**Estado atual:** sistema planejado, ainda não implementado.

---

## 8. Sistema de tesouros — Planejamento

Os tesouros serão distribuídos pelo tabuleiro conforme as configurações da dificuldade.

A geração deverá garantir que uma posição não receba dois tesouros por engano.

Os tesouros serão armazenados em uma coleção dinâmica separada da matriz, permitindo trabalhar com informações como:

- Identificação;
- Coordenadas;
- Tipo ou raridade;
- Estado de descoberta;
- Pontuação, caso seja implementada.

O programa deverá verificar se uma coordenada investigada corresponde à posição de algum tesouro.

A descoberta de todos os tesouros será a condição obrigatória de vitória.

**Estado atual:** sistema planejado, ainda não implementado.

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

1. Introdução e história.
2. Objetivo do jogo.
3. Regras de investigação.
4. Controles e coordenadas.
5. Dificuldades.
6. Tipos de terreno.
7. Tesouros possíveis.
8. Funcionamento das pistas.
9. Condições de vitória e derrota.
10. Salvamento, carregamento e ranking.
11. Dicas gerais para iniciantes.
12. Créditos.

A proposta é que o manual também contribua para a ambientação do jogo, com uma apresentação inspirada nos manuais de jogos clássicos de computador.

O acesso será realizado pelo menu principal.

**Estado atual:** planejado, ainda não implementado.

---

## 11. Persistência e ranking — Planejamento

### 11.1. Salvamento

O sistema permitirá salvar uma expedição e retomá-la posteriormente.

Os dados persistidos deverão ser suficientes para restaurar corretamente a partida, incluindo:

- Dificuldade;
- Dimensões do tabuleiro;
- Localização dos tesouros;
- Casas já investigadas;
- Tesouros encontrados;
- Tentativas restantes;
- Dicas disponíveis;
- Demais informações essenciais do estado do jogo.

### 11.2. Carregamento

O programa deverá verificar a existência de um arquivo de salvamento e tratar possíveis erros de abertura, leitura e consistência dos dados.

A ausência de um arquivo na primeira execução não deverá causar falhas.

### 11.3. Ranking

O ranking armazenará resultados dos jogadores.

Os critérios de pontuação ainda serão definidos pela equipe.

Poderão ser considerados elementos como tesouros encontrados, dificuldade e tentativas restantes, respeitando as condições obrigatórias de vitória e derrota.

O formato de persistência será escolhido durante a implementação e documentado posteriormente.

**Estado atual:** ainda não implementado.

---

## 12. Arquitetura do projeto

O ATLAS será organizado em módulos para separar as responsabilidades do programa.

### 12.1. Estrutura inicial

```text
ATLAS/
|
|-- include/
|   |-- menu.h
|   |-- difficulty.h
|
|-- src/
|   |-- main.c
|   |-- menu.c
|   |-- difficulty.c
|
|-- README.md
```

### 12.2. Responsabilidades atuais

| Arquivo | Responsabilidade |
|---|---|
| `main.c` | Inicialização do programa |
| `menu.c` | Interface e controle do menu principal |
| `menu.h` | Declarações públicas do menu |
| `difficulty.c` | Leitura e processamento da seleção de dificuldade |
| `difficulty.h` | Definição dos tipos e funções de dificuldade |

### 12.3. Módulos previstos

Conforme o desenvolvimento avançar, poderão ser adicionados módulos responsáveis por:

- Tabuleiro;
- Tesouros;
- Investigação;
- Pistas;
- Salvamento e carregamento;
- Ranking;
- Manual;
- Interface textual.

A divisão poderá ser ajustada conforme as necessidades do projeto, evitando modularização excessiva.

### 12.4. TAD e encapsulamento

O projeto utilizará pelo menos um Tipo Abstrato de Dados efetivo.

O tabuleiro é o principal candidato a TAD, pois possuirá dados próprios e operações para criação, investigação, consulta e liberação de memória.

O encapsulamento buscará manter detalhes internos no arquivo `.c`, expondo no `.h` apenas as funções e tipos necessários.

A simples separação entre arquivos `.h` e `.c` não será considerada suficiente para caracterizar um TAD.

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

## 14. Compilação e execução - Em Desenvolvimento

O projeto utilizará GCC como compilador.

A compilação deverá ser realizada a partir da pasta principal do repositório.

### Windows

Com os módulos integrados, o comando previsto será:

```bash
gcc -std=c11 -Wall -Wextra -Iinclude src/main.c src/menu.c src/difficulty.c -o atlas.exe
```

Execução no PowerShell:

```powershell
.\atlas.exe
```

### Linux

```bash
gcc -std=c11 -Wall -Wextra -Iinclude src/main.c src/menu.c src/difficulty.c -o atlas
```

Execução:

```bash
./atlas
```

**Importante:** esses comandos deverão ser verificados após a integração das funções. O código ainda está em desenvolvimento e pode apresentar referências a módulos não implementados.

Quando a estrutura definitiva estiver pronta, esta seção será atualizada com instruções de compilação testadas.

---

## 15. Planejamento do desenvolvimento

O projeto seguirá as etapas estabelecidas na disciplina.

| Data | Etapa |
|---|---|
| 20/10/2026 | Checkpoint 1 — Estrutura inicial |
| 27/10/2026 | Checkpoint 2 — Núcleo funcional |
| 03/11/2026 | Estabilização final |
| 06/11/2026 | Início previsto das avaliações técnicas |

Existe uma divergência entre as datas de avaliação apresentadas nas diferentes seções do documento do professor. A equipe deverá confirmar o cronograma definitivo.

### Etapas internas

**Etapa 1 — Inicialização e menus**

- Estrutura do projeto;
- Menu principal;
- Seleção de dificuldade;
- Validação de entradas.

**Etapa 2 — Núcleo do jogo**

- Matriz dinâmica;
- Geração do tabuleiro;
- Coleção dinâmica de tesouros;
- Investigação por coordenadas;
- Controle de tentativas;
- Vitória e derrota.

**Etapa 3 — Funcionalidades obrigatórias restantes**

- Salvamento;
- Carregamento;
- Ranking;
- Tratamento de erros;
- Revisão do encapsulamento.

**Etapa 4 — Diferenciais e estabilização**

- Sistema de pistas;
- Mapa ASCII temático;
- Manual do Explorador;
- Ajustes de interface;
- Balanceamento;
- Testes finais.

A implementação dos requisitos obrigatórios terá prioridade sobre funcionalidades adicionais.

---

## 16. Limitações atuais

A versão 0.1 ainda não representa um jogo funcional completo.

As limitações conhecidas incluem:

- Seleção de dificuldade ainda não completamente integrada;
- Validação de entradas pendente de melhorias;
- Ausência de tabuleiro dinâmico;
- Ausência de geração de tesouros;
- Ausência de investigação de coordenadas;
- Ausência de persistência;
- Ausência de ranking funcional;
- Manual ainda não criado;
- Testes de integração pendentes.

Essas limitações serão atualizadas conforme o desenvolvimento avançar.

---

## 17. Uso de Inteligência Artificial

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

## 18. Contribuições e controle de versão

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

## 19. Próximas tarefas

As atividades prioritárias após o primeiro commit serão:

- Finalizar a implementação de `selectDiff()`.
- Concluir a função `getGameConf()`.
- Integrar a seleção de dificuldade ao menu principal.
- Melhorar a validação das entradas.
- Desenvolver o Manual do Explorador.
- Iniciar o TAD do tabuleiro.
- Implementar a matriz dinâmica.
- Criar a coleção dinâmica de tesouros.

O planejamento poderá ser atualizado conforme as decisões técnicas da equipe.

---

## 20. Visão futura

Após a conclusão da versão acadêmica, a equipe poderá continuar evoluindo o ATLAS como projeto de portfólio.

Entre as possibilidades futuras estão:

- Novos mapas;
- Novas dificuldades;
- Expansão narrativa;
- Charadas e enigmas;
- Eventos especiais;
- Personagens;
- Sistemas de combate;
- Novas mecânicas de exploração.

Essas funcionalidades não fazem parte do escopo obrigatório da versão acadêmica e somente serão desenvolvidas posteriormente, ou mediante autorização quando necessário.

A prioridade atual é entregar um Caça ao Tesouro funcional, organizado e compatível com o tema J4.

---

## 21. Créditos

**ATLAS — A Maldição do Tesouro Perdido**

Desenvolvido por:

- Guilherme de Almeida
- Guilherme Arthur
- Marcio
- Enzo

**FACAPE — Ciência da Computação**  
**Programação Avançada — 2026.2**  
**Professor: Thierry Araujo**

**Desenvolvimento inicial e primeiro marco do projeto:** Guilherme de Almeida.

---

*ATLAS — A Maldição do Tesouro Perdido*

**Nem todo tesouro deve ser encontrado. Mas todos precisam ser procurados.**