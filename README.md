# L(p,q)-Coloring - DCC059

Implementação de algoritmos heurísticos para o problema de L(p,q)-Coloring.

## Problema

Dado um grafo G=(V,E), encontrar uma coloração f: V → Z⁺ tal que:
- |f(u) - f(v)| ≥ p para vértices adjacentes (distância 1)
- |f(u) - f(w)| ≥ q para vértices a distância 2

**Objetivo**: Minimizar a maior cor utilizada.

## Compilação

### Linux / macOS
```bash
g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring
```

### Windows (MinGW)
```powershell
g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring.exe
```

## Execução

### Formato Geral
```
./lpq_coloring <arquivo.col> <algoritmo> [semente] [alpha] [iteracoes] [bloco]
```

### Algoritmo 1 - Guloso
```bash
./lpq_coloring instancias/huck.col 1
```

### Algoritmo 2 - Guloso Randomizado
```bash
./lpq_coloring instancias/huck.col 2 <semente> <alpha> <iteracoes>

# Exemplo: alpha=0.3, 30 iterações, semente 123
./lpq_coloring instancias/huck.col 2 123 0.3 30
```

### Algoritmo 3 - Guloso Randomizado Reativo
```bash
./lpq_coloring instancias/huck.col 3 <semente> <alpha> <iteracoes> <bloco>

# Exemplo: 300 iterações, bloco=30, semente 123
./lpq_coloring instancias/huck.col 3 123 0.3 300 30
```

## Scripts de Automação

Todos os scripts estão na pasta `scripts/`.

| Script | Descrição |
|--------|-----------|
| `run_all.bat/.sh` | Executa TODOS os experimentos (todas as instâncias) |
| `run_instance.bat/.sh` | Executa experimentos para UMA instância |
| `run_quick.bat/.sh` | Teste rápido de validação |

### Executar Todos os Testes
```bash
# Windows
scripts\run_all.bat

# Linux
./scripts/run_all.sh
```

### Executar Uma Instância Específica
```bash
# Windows
scripts\run_instance.bat instancias\huck.col

# Linux
./scripts/run_instance.sh instancias/huck.col
```

### Teste Rápido
```bash
# Windows
scripts\run_quick.bat

# Linux
./scripts/run_quick.sh
```

## Instâncias Disponíveis

| Arquivo | Vértices | Arestas | χ(G) Conhecido |
|---------|----------|---------|----------------|
| test.col | 5 | 4 | 5 |
| huck.col | 74 | 301 | 11 |
| david.col | 87 | 406 | 11 |
| anna.col | 138 | 493 | 11 |
| homer.col | 561 | 1629 | 13 |
| mulsol.i.1.col | 197 | 3925 | 49 |
| zeroin.i.1.col | 211 | 4100 | 49 |
| fpsol2.i.1.col | 496 | 11654 | 65 |
| inithx.i.1.col | 864 | 18707 | 54 |
| le450_5a.col | 450 | 5714 | 5 |
| le450_15a.col | 450 | 8168 | 15 |
| le450_25a.col | 450 | 8260 | 25 |
| school1.col | 385 | 19095 | ? |

Fonte: [DIMACS Graph Coloring Instances](https://mat.tepper.cmu.edu/COLOR/instances.html)

## Análise de Resultados

Após executar os experimentos, analise com:

```bash
python analyze_results.py resultados.csv <instancia> [melhor_conhecido]

# Exemplo:
python analyze_results.py resultados.csv huck 11
```

O script gera tabelas LaTeX automaticamente em `tabelas_<instancia>.tex`.

## Arquivos de Saída

| Arquivo | Descrição |
|---------|-----------|
| `resultados.csv` | Histórico de todas as execuções |
| `coloracao.txt` | Última coloração gerada |
| `tabelas_*.tex` | Tabelas LaTeX para o relatório |
| `logs/` | Logs detalhados de cada execução |

### Formato do CSV
```
data_hora,instancia,algoritmo,alpha,iteracoes,bloco,semente,tempo,melhor_cor
```

## Parâmetros Recomendados

| Algoritmo | Alpha | Iterações | Bloco |
|-----------|-------|-----------|-------|
| Guloso | - | - | - |
| Randomizado | 0.1, 0.3, 0.5 | ≥30 | - |
| Reativo | - | ≥300 | 30-50 |

## Formato DIMACS (.col)

```
p edge <n_vertices> <n_arestas>
e <u> <v>
...
```

## Visualização

A coloração pode ser visualizada em:
- [CS Academy Graph Editor](https://csacademy.com/app/graph_editor/)

## Estrutura do Projeto

```
Grafos/
├── README.md              # Esta documentação
├── main.cpp               # Código fonte
├── makefile               # Compilação
├── analyze_results.py     # Análise de resultados
├── instancias/            # Grafos de entrada (.col)
├── scripts/               # Scripts de automação
│   ├── run_all.bat/.sh
│   ├── run_instance.bat/.sh
│   └── run_quick.bat/.sh
└── logs/                  # Logs de execução
```
