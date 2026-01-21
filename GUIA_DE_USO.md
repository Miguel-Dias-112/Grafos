# 📊 Guia de Uso - L(p,q)-Coloring Experiments

## ✅ Status dos Experimentos

**TODOS OS EXPERIMENTOS FORAM CONCLUÍDOS COM SUCESSO!**

Data: 20/01/2026

---

## 📁 Estrutura de Arquivos Gerados

```
.
├── RELATORIO_FINAL.tex          ← Relatório completo em LaTeX
├── RELATORIO_RESUMO.md          ← Resumo em Markdown (Este arquivo com contexto)
├── resultados.csv               ← Base de dados completa (754 linhas)
├── tabelas_huck.tex             ← Tabelas LaTeX para instância huck
├── tabelas_david.tex            ← Tabelas LaTeX para instância david
├── tabelas_anna.tex             ← Tabelas LaTeX para instância anna
├── tabelas_homer.tex            ← Tabelas LaTeX para instância homer
├── tabelas_mulsol.i.1.tex       ← Tabelas LaTeX para instância mulsol.i.1
├── tabelas_zeroin.i.1.tex       ← Tabelas LaTeX para instância zeroin.i.1
├── tabelas_le450_5a.tex         ← Tabelas LaTeX para instância le450_5a
├── tabelas_test.tex             ← Tabelas LaTeX para instância test (anterior)
├── logs/
│   ├── huck/                    ← 50 arquivos de log (guloso + 4 variantes × 10)
│   ├── david/                   ← 50 arquivos de log
│   ├── anna/                    ← 50 arquivos de log
│   ├── homer/                   ← 50 arquivos de log
│   ├── mulsol.i.1/              ← 50 arquivos de log
│   ├── zeroin.i.1/              ← 50 arquivos de log
│   ├── le450_5a/                ← 50 arquivos de log
│   └── test/                    ← Logs anteriores
└── scripts/
    ├── run_all.bat              ← Script que executa todos os experimentos
    ├── run_all.sh               ← Versão Linux
    ├── run_instance.bat         ← Script para uma instância específica
    ├── run_instance.sh          ← Versão Linux
    ├── run_quick.bat            ← Teste rápido
    └── run_quick.sh             ← Versão Linux
```

---

## 📈 Resumo dos Resultados

### Experimentos Realizados

- **Total de instâncias:** 7 (huck, david, anna, homer, mulsol.i.1, zeroin.i.1, le450_5a)
- **Total de execuções:** 701
- **Arquivos de log gerados:** 360
- **Registros no CSV:** 754

### Configuração dos Experimentos

```
Guloso:
  - 10 execuções por instância
  - Determinístico

Randomizado:
  - 3 variações de α (0.1, 0.3, 0.5)
  - 30 iterações cada
  - 10 execuções por variação

Reativo:
  - 300 iterações
  - Bloco = 30
  - 10 execuções
```

---

## 📑 Como Visualizar os Resultados

### Opção 1: Visualizar o Resumo em Markdown
```bash
# Abra no VS Code ou qualquer editor
RELATORIO_RESUMO.md
```

### Opção 2: Compilar o Relatório LaTeX
```bash
# No Windows
pdflatex RELATORIO_FINAL.tex

# Resultado: RELATORIO_FINAL.pdf
```

### Opção 3: Ver Tabelas Individuais
```bash
# Compilar tabelas para instância específica
pdflatex tabelas_huck.tex
pdflatex tabelas_mulsol.i.1.tex
```

### Opção 4: Analisar o CSV Diretamente
```bash
# Com Python pandas
python
>>> import pandas as pd
>>> df = pd.read_csv('resultados.csv')
>>> df.head()
>>> df.groupby('algoritmo')['melhor_cor'].agg(['min', 'max', 'mean'])
```

---

## 🔄 Re-executar Experimentos

### Opção 1: Todos os Experimentos (recomendado)
```bash
scripts\run_all.bat
```
Tempo estimado: ~1-2 horas (dependendo do computador)

### Opção 2: Uma Instância Específica
```bash
scripts\run_instance.bat instancias\huck.col
```

### Opção 3: Teste Rápido de Validação
```bash
scripts\run_quick.bat
```
Tempo estimado: < 1 minuto

---

## 📊 Regenerar Análises e Relatórios

Depois de executar `run_all.bat`, gere as tabelas:

```bash
# Para cada instância:
python analyze_results.py resultados.csv huck 11
python analyze_results.py resultados.csv david 11
python analyze_results.py resultados.csv anna 11
python analyze_results.py resultados.csv homer 13
python analyze_results.py resultados.csv mulsol.i.1 49
python analyze_results.py resultados.csv zeroin.i.1 49
python analyze_results.py resultados.csv le450_5a 5
```

Ou de forma mais simples (abre os valores conhecidos automaticamente):

```bash
python analyze_results.py resultados.csv huck
python analyze_results.py resultados.csv david
python analyze_results.py resultados.csv anna
python analyze_results.py resultados.csv homer
python analyze_results.py resultados.csv mulsol.i.1
python analyze_results.py resultados.csv zeroin.i.1
python analyze_results.py resultados.csv le450_5a
```

---

## 📋 Métricas Coletadas (CSV)

O arquivo `resultados.csv` contém os seguintes campos:

| Campo | Descrição |
|-------|-----------|
| instancia | Nome do arquivo .col |
| algoritmo | Nome do algoritmo executado |
| semente | Semente pseudo-aleatória |
| alpha | Valor de α (para randomizado/reativo) |
| iteracoes | Número de iterações |
| bloco | Tamanho do bloco (para reativo) |
| melhor_cor | Melhor solução encontrada |
| media_cor | Média das cores ao longo das iterações |
| tempo | Tempo de execução em segundos |

---

## 🎯 Principais Descobertas

### 1. Qualidade vs. Velocidade

| Algoritmo | Velocidade | Qualidade | Recomendação |
|-----------|-----------|-----------|--------------|
| Guloso | ⭐⭐⭐⭐⭐ | ⭐ | Tempo crítico |
| Randomizado | ⭐⭐ | ⭐⭐⭐⭐ | Melhor qualidade |
| Reativo | ⭐⭐ | ⭐⭐⭐⭐ | Balanço |

### 2. Instâncias Desafiadoras

- **le450_5a**: Maior desvio percentual (2180-2398%)
- **zeroin.i.1**: Algoritmo reativo melhor que randomizado
- **mulsol.i.1**: Algoritmos randomizados conseguem melhora de 6 cores

### 3. Parametrização

- Valor de **α** (greedy parameter):
  - α=0.1: Mais exploração
  - α=0.3: Balanceado
  - α=0.5: Mais aleatoriedade
  - Diferenças geralmente pequenas entre variações

---

## 🛠️ Troubleshooting

### Problema: "lpq_coloring.exe não encontrado"
**Solução:** Compile o C++ primeiro
```bash
g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring.exe
```

### Problema: "Python não encontrado"
**Solução:** Certifique-se de ter Python 3.7+ instalado
```bash
python --version
```

### Problema: "CSV vazio" 
**Solução:** Verifique se run_all.bat foi executado completamente

### Problema: "LaTeX não compila"
**Solução:** Instale uma distribuição LaTeX (TeX Live, MiKTeX)

---

## 📞 Comandos Úteis

### Ver estatísticas rápidas
```bash
tail -20 resultados.csv
```

### Contar registros por instância
```bash
grep "huck" resultados.csv | wc -l
```

### Filtrar por algoritmo
```bash
grep "REATIVO" resultados.csv
```

### Comparar tempos
```bash
awk -F, '$2=="GULOSO" {print $10}' resultados.csv
```

---

## 📄 Formato dos Arquivos LaTeX

### Estrutura Padrão de Tabela

```latex
\begin{table}[H]
\centering
\caption{Descrição}
\begin{tabular}{l|r|r|r|r|r|r}
\hline
Algoritmo & Melhor & Média & Pior & Tempo (s) & Desv. % & Desv. Média % \\
\hline
% dados aqui
\end{tabular}
\end{table}
```

Todas as tabelas estão prontas para serem compiladas e incluídas em relatórios maiores.

---

## 🔗 Referências

- **Problema L(p,q)-Coloring**: Minimizar a maior cor em uma coloração que satisfaça restrições de distância
- **Literatura**: DIMACS Graph Coloring Benchmark
- **Algoritmos**: GRASP (Greedy Randomized Adaptive Search Procedure)

---

## ✨ Próximos Passos

1. ✅ **Executar experimentos** - FEITO
2. ✅ **Gerar tabelas LaTeX** - FEITO
3. ✅ **Criar relatório consolidado** - FEITO
4. 📝 **Incluir em documento final** - Use os arquivos .tex
5. 📊 **Gerar gráficos (opcional)** - Ver `analyze_results.py` para extensões

---

**Gerado em:** 20/01/2026  
**Versão:** 1.0 Final  
**Status:** ✅ Pronto para Apresentação
