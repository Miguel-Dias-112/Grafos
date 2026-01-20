================================================================================
                    L(p,q)-COLORING - DCC059
================================================================================

DESCRIÇÃO:
Implementação de algoritmos heurísticos para resolver o problema de L(p,q)-coloring,
um problema de otimização combinatória NP-difícil. Três algoritmos foram implementados:
- Guloso
- Guloso Randomizado
- Guloso Randomizado Reativo

================================================================================
COMPILAÇÃO:
================================================================================

--- LINUX / macOS ---

Opção 1 - Compilação direta (recomendado):
  g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring

Opção 2 - Usando makefile (se tiver make instalado):
  make

Resultado esperado:
  Executável 'lpq_coloring' criado sem erros

--- WINDOWS ---

Opção 1 - Com MinGW (g++):
  g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring.exe

Opção 2 - Usando makefile com mingw32-make:
  mingw32-make

Opção 3 - PowerShell (direto):
  g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring.exe

Resultado esperado:
  Executável 'lpq_coloring.exe' criado sem erros

================================================================================
EXECUÇÃO:
================================================================================

Formato geral (LINUX/macOS):
  ./lpq_coloring <arquivo.col> <algoritmo> [semente] [alpha] [iteracoes] [bloco]

Formato geral (WINDOWS):
  .\lpq_coloring.exe <arquivo.col> <algoritmo> [semente] [alpha] [iteracoes] [bloco]

Parâmetros:
  - arquivo.col: grafo em formato DIMACS (p edge n m; e u v)
  - algoritmo: 1 (guloso), 2 (randomizado), 3 (reativo)
  - semente: seed aleatória (opcional, padrão: data/hora)
  - alpha: parâmetro do algoritmo randomizado (0.0-1.0)
  - iteracoes: número de iterações (randomizado e reativo)
  - bloco: tamanho do bloco para atualização (reativo)

--------------------------------------------------------------------------------
ALGORITMO 1 - GULOSO (Greedy)
--------------------------------------------------------------------------------

Comando (LINUX/macOS):
  ./lpq_coloring instancias/seu_grafo.col 1

Comando (WINDOWS):
  .\lpq_coloring.exe instancias/seu_grafo.col 1

Descrição:
  Algoritmo guloso determinístico que ordena vértices por grau decrescente
  e atribui a menor cor válida disponível.

Exemplo (LINUX):
  ./lpq_coloring instancias/test.col 1

Exemplo (WINDOWS):
  .\lpq_coloring.exe instancias/test.col 1

Saída:
  - Exibe grafo e tempo de carregamento
  - Imprime maior cor utilizada
  - Valida restrições L(p,q)
  - Salva coloração em coloracao.txt
  - Registra resultado em resultados.csv

--------------------------------------------------------------------------------
ALGORITMO 2 - GULOSO RANDOMIZADO (Randomized Greedy)
--------------------------------------------------------------------------------

Comando (LINUX/macOS):
  ./lpq_coloring instancias/seu_grafo.col 2 <semente> <alpha> <iteracoes>

Comando (WINDOWS):
  .\lpq_coloring.exe instancias/seu_grafo.col 2 <semente> <alpha> <iteracoes>

Parâmetros:
  - semente: valor para reprodutibilidade (ex: 123)
  - alpha: fração da RCL (0.0=guloso, 1.0=aleatório; recomendado: 0.3)
  - iteracoes: número de construções (recomendado: 30 ou mais)

Exemplo (LINUX):
  ./lpq_coloring instancias/test.col 2 123 0.3 30

Exemplo (WINDOWS):
  .\lpq_coloring.exe instancias/test.col 2 123 0.3 30

Descrição:
  Algoritmo guloso randomizado que constrói múltiplas soluções.
  Usa RCL (Restricted Candidate List) baseada no CUSTO DA COR (c_min e c_max):
    RCL = {v | custo(v) <= c_min + alpha * (c_max - c_min)}
  Onde custo(v) é a menor cor válida para o vértice v.
  Mantém a melhor solução encontrada.

Saída:
  - Exibe média das soluções nas iterações
  - Retorna melhor solução encontrada
  - Valida restrições L(p,q)
  - Salva coloração em coloracao.txt
  - Registra resultado em resultados.csv

--------------------------------------------------------------------------------
ALGORITMO 3 - GULOSO RANDOMIZADO REATIVO
--------------------------------------------------------------------------------

Comando (LINUX/macOS):
  ./lpq_coloring instancias/seu_grafo.col 3 <semente> <alpha> <iteracoes> <bloco>

Comando (WINDOWS):
  .\lpq_coloring.exe instancias/seu_grafo.col 3 <semente> <alpha> <iteracoes> <bloco>

Parâmetros:
  - semente: valor para reprodutibilidade
  - alpha: parâmetro inicial (não usado no reativo, apenas para compatibilidade)
  - iteracoes: número total de construções (recomendado: 300 ou mais)
  - bloco: tamanho do bloco para atualizar probabilidades (recomendado: 30-50)

Exemplo (LINUX):
  ./lpq_coloring instancias/test.col 3 123 0.3 300 30

Exemplo (WINDOWS):
  .\lpq_coloring.exe instancias/test.col 3 123 0.3 300 30

Descrição:
  Algoritmo guloso randomizado reativo (GRASP). Mantém múltiplos valores de alpha
  e adapta suas probabilidades baseado na qualidade das soluções obtidas.
  Requer mais iterações para convergência.

Saída:
  - Exibe melhor alpha encontrado
  - Retorna melhor solução encontrada
  - Valida restrições L(p,q)
  - Salva coloração em coloracao.txt
  - Registra resultado em resultados.csv

================================================================================
RESULTADOS:
================================================================================

Arquivos gerados após cada execução:

1. coloracao.txt
   Contém a última coloração gerada no formato:
   <vértice> <cor>
   1 4
   2 1
   3 3
   ...

2. resultados.csv
   Tabela acumulativa com histórico de todas as execuções:
   data_hora,instancia,algoritmo,alpha,iteracoes,bloco,semente,tempo,melhor_cor

   Colunas:
   - data_hora: data e hora da execução
   - instancia: nome do arquivo
   - algoritmo: GULOSO, RANDOMIZADO ou REATIVO
   - alpha: parâmetro alpha (vazio para guloso)
   - iteracoes: iterações (vazio para guloso)
   - bloco: tamanho do bloco (vazio para guloso/randomizado)
   - semente: seed aleatória usada
   - tempo: tempo em segundos
   - melhor_cor: maior cor utilizada

================================================================================
VALIDAÇÃO DAS RESTRIÇÕES:
================================================================================

O programa valida automaticamente as restrições L(p,q):

1. Distância 1 (adjacentes):
   |f(u) - f(v)| >= p para todo par (u,v) adjacente

2. Distância 2:
   |f(u) - f(w)| >= q para todo par (u,w) a distância 2

3. Cores válidas:
   Todas as cores devem ser inteiros positivos (>= 1)

Se houver violação, o programa exibe mensagem de erro detalhada e retorna
status de falha.

================================================================================
DETALHES TÉCNICOS - RCL (Restricted Candidate List)
================================================================================

A RCL é implementada conforme a especificação clássica do GRASP, usando o
CUSTO DA COR como critério:

1. Para cada vértice candidato v não colorido, calcula-se custo(v) = a menor
   cor válida que pode ser atribuída a v respeitando as restrições L(p,q).

2. Seja c_min = min{custo(v)} e c_max = max{custo(v)} entre os candidatos.

3. A RCL é construída como:
   RCL = {v | custo(v) <= c_min + alpha * (c_max - c_min)}

4. Um vértice é escolhido aleatoriamente da RCL.

5. Este critério (custo da cor) é mais apropriado que apenas grau, pois:
   - Reflete a viabilidade real (qual cor pode ser atribuída)
   - Equilibra exploração (alpha alto) e exploração de boas soluções (alpha baixo)
   - Permite que o algoritmo adapte-se à estrutura específica do problema

Valores de alpha:
  - alpha = 0.0: RCL contém apenas vértices com custo mínimo (modo guloso)
  - alpha = 1.0: RCL contém todos os candidatos (modo aleatório)
  - alpha = 0.3: Valor recomendado para equilíbrio

================================================================================
FORMATO DO ARQUIVO .col (DIMACS):
================================================================================

Formato padrão para instâncias de grafos:

p edge n m
c comentário (opcional)
e u v
e u v
...

Onde:
- 'p edge n m': primeiro token é 'p', depois 'edge', n vértices, m arestas
- 'e u v': aresta entre vértices u e v (indexação 1-based)
- 'c': linhas de comentário (ignoradas)

Exemplo (caminho com 5 vértices):
p edge 5 4
e 1 2
e 2 3
e 3 4
e 4 5

================================================================================
EXEMPLOS DE EXECUÇÃO COMPLETA:
================================================================================

--- LINUX / macOS ---

# Teste rápido com grafo simples (teste)
./lpq_coloring instancias/test.col 1

# Algoritmo guloso com grafo maior
./lpq_coloring instancias/seu_grafo.col 1

# Algoritmo randomizado (seed=123, alpha=0.3, 30 iterações)
./lpq_coloring instancias/seu_grafo.col 2 123 0.3 30

# Algoritmo randomizado (seed=456, alpha=0.5, 50 iterações)
./lpq_coloring instancias/seu_grafo.col 2 456 0.5 50

# Algoritmo reativo (seed=123, 300 iterações, bloco=30)
./lpq_coloring instancias/seu_grafo.col 3 123 0.3 300 30

# Algoritmo reativo com mais iterações
./lpq_coloring instancias/seu_grafo.col 3 789 0.3 500 40

# Executar múltiplas vezes com sementes diferentes (para experimentos)
for seed in {1..10}; do
  ./lpq_coloring instancias/seu_grafo.col 1 $seed
  ./lpq_coloring instancias/seu_grafo.col 2 $seed 0.3 30
  ./lpq_coloring instancias/seu_grafo.col 3 $seed 0.3 300 30
done

# Visualizar resultados
cat resultados.csv

--- WINDOWS (PowerShell) ---

# Teste rápido com grafo simples (teste)
.\lpq_coloring.exe instancias/test.col 1

# Algoritmo guloso com grafo maior
.\lpq_coloring.exe instancias/seu_grafo.col 1

# Algoritmo randomizado (seed=123, alpha=0.3, 30 iterações)
.\lpq_coloring.exe instancias/seu_grafo.col 2 123 0.3 30

# Algoritmo randomizado (seed=456, alpha=0.5, 50 iterações)
.\lpq_coloring.exe instancias/seu_grafo.col 2 456 0.5 50

# Algoritmo reativo (seed=123, 300 iterações, bloco=30)
.\lpq_coloring.exe instancias/seu_grafo.col 3 123 0.3 300 30

# Algoritmo reativo com mais iterações
.\lpq_coloring.exe instancias/seu_grafo.col 3 789 0.3 500 40

# Executar múltiplas vezes com sementes diferentes (para experimentos)
for ($seed = 1; $seed -le 10; $seed++) {
  .\lpq_coloring.exe instancias/seu_grafo.col 1 $seed
  .\lpq_coloring.exe instancias/seu_grafo.col 2 $seed 0.3 30
  .\lpq_coloring.exe instancias/seu_grafo.col 3 $seed 0.3 300 30
}

# Visualizar resultados
type resultados.csv

================================================================================
PARÂMETROS RECOMENDADOS:
================================================================================

Algoritmo Guloso:
  ./lpq_coloring <grafo> 1

Algoritmo Randomizado:
  - alpha = 0.1, 0.3, 0.5, 0.7 ou 0.9
  - iteracoes >= 30

Algoritmo Reativo:
  - iteracoes >= 300 (mínimo)
  - bloco entre 30-50
  - tempo de execução maior, mas melhor qualidade

Para experimentos:
  - Rodar 10 execuções com sementes diferentes (1-10)
  - Variar alpha para randomizado (ex: 0.1, 0.3, 0.7)
  - Registrar tempo e melhor solução no CSV

================================================================================
TROUBLESHOOTING:
================================================================================

Erro: "arquivo de instância inválido"
  - Verifique se o arquivo existe em instancias/
  - Verifique se começa com "p edge n m"
  - Verifique formato das arestas "e u v"

Erro: "Coloracao INVALIDA"
  - Algoritmo encontrou bug
  - Verifique parâmetros p e q
  - Teste com grafo simples (test.col)

Erro: "comando não encontrado"
  - Certifique-se que compilou: g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring
  - Use ./lpq_coloring (não apenas lpq_coloring)

================================================================================
NOTAS IMPORTANTES:
================================================================================

1. Semente aleatória: gerada automaticamente com data/hora se não especificada
2. Reprodutibilidade: use mesma semente para repetir teste exato
3. CSV acumula: resultados.csv cresce a cada execução (não apaga)
4. Coloração: último resultado salvo em coloracao.txt
5. Parâmetros padrão: p=2, q=1 (hardcoded em main.cpp)

Para modificar p ou q, edite a função main() em main.cpp:
  int p = 2, q = 1;

================================================================================
AUTOMAÇÃO DE EXPERIMENTOS:
================================================================================

Scripts disponíveis para executar os experimentos de forma automatizada:

--- SCRIPT RÁPIDO (RECOMENDADO) ---

Windows:
  run_all.bat

Executa automaticamente todos os experimentos para instancias\test.col:
  - 10x Guloso (sementes 1-10)
  - 10x Randomizado α=0.1 (30 iterações)
  - 10x Randomizado α=0.3 (30 iterações)
  - 10x Randomizado α=0.5 (30 iterações)
  - 10x Reativo (300 iterações, bloco=30)

--- SCRIPT COMPLETO ---

Windows:
  run_experiments.bat <instancia.col> [alpha1,alpha2,alpha3]

Exemplo:
  run_experiments.bat instancias\test.col 0.1,0.3,0.5

Linux / macOS:
  chmod +x run_experiments.sh
  ./run_experiments.sh <instancia.col> [alpha1,alpha2,alpha3]

Exemplo:
  ./run_experiments.sh instancias/test.col 0.1,0.3,0.5

================================================================================
ANÁLISE DE RESULTADOS:
================================================================================

Script Python para calcular desvios percentuais e gerar tabelas LaTeX:

Uso:
  python analyze_results.py <arquivo_csv> <instancia> [melhor_conhecido]

Exemplo:
  python analyze_results.py resultados.csv test 5

O script gera:
  1. Tabela resumo no console
  2. Tabela consolidada em LaTeX
  3. Tabelas individuais (conforme especificação do trabalho):
     - Desvio da melhor solução
     - Desvio da média
     - Tempo médio
  4. Arquivo tabelas_<instancia>.tex com todas as tabelas

Fórmulas de desvio utilizadas:
  - Desvio Melhor = ((melhor_encontrada - melhor_conhecida) / melhor_conhecida) * 100%
  - Desvio Média = ((média_encontrada - melhor_conhecida) / melhor_conhecida) * 100%

================================================================================
FLUXO DE TRABALHO COMPLETO:
================================================================================

1. Compilar o programa:
   g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring

2. Executar os experimentos:
   
   Windows:
     run_all.bat
   
   Linux:
     ./run_experiments.sh instancias/test.col 0.1,0.3,0.5

3. Analisar os resultados e gerar tabelas LaTeX:
   python analyze_results.py resultados.csv test 5

4. Incluir as tabelas no relatório:
   \input{tabelas_test.tex}

================================================================================
ARQUIVOS DO PROJETO:
================================================================================

Código fonte:
  - main.cpp           : Implementação dos algoritmos
  - makefile           : Regras de compilação

Scripts de automação:
  - run_all.bat        : Execução rápida de todos os experimentos (Windows)
  - run_experiments.bat: Script completo de experimentos (Windows)
  - run_experiments.sh : Script completo de experimentos (Linux/macOS)
  - analyze_results.py : Análise de resultados e geração de tabelas LaTeX

Arquivos gerados:
  - resultados.csv             : Histórico de todos os experimentos
  - coloracao.txt              : Última coloração gerada
  - visualizacao_csacademy.txt : Formato para CS Academy Graph Editor
  - tabelas_<instancia>.tex    : Tabelas LaTeX geradas

Instâncias:
  - instancias/                : Diretório com arquivos .col

================================================================================
FIM DO README
================================================================================