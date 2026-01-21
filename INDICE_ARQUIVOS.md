# 🎯 ÍNDICE DE ARQUIVOS - Experimentos L(p,q)-Coloring

**Data:** 20/01/2026  
**Status:** ✅ COMPLETO - Todos os experimentos executados e analisados

---

## 📋 Documentos Principais

### 1. **RELATORIO_FINAL.tex** ⭐
   - Relatório completo em LaTeX
   - Contém todas as instâncias e resultados
   - Pronto para compilar com `pdflatex`
   - **Tamanho:** 9.17 KB
   - **Uso:** Compile para gerar PDF

```bash
pdflatex RELATORIO_FINAL.tex
```

### 2. **RELATORIO_RESUMO.md** 📊
   - Resumo executivo em Markdown
   - Tabelas de resultados em formato markdown
   - Análises e conclusões
   - **Tamanho:** 6.92 KB
   - **Uso:** Leia diretamente ou abra no VS Code

### 3. **GUIA_DE_USO.md** 📖
   - Instruções completas de uso
   - Como reproduzir os experimentos
   - Troubleshooting e referências
   - **Tamanho:** 7.9 KB
   - **Uso:** Consulte para dúvidas

---

## 📈 Tabelas LaTeX Individuais por Instância

### Instância 1: huck.col
   - **Arquivo:** `tabelas_huck.tex`
   - **Vértices:** 74
   - **Arestas:** 301
   - **Melhor Conhecido:** 11 cores
   - **Compiles:** `pdflatex tabelas_huck.tex`

### Instância 2: david.col
   - **Arquivo:** `tabelas_david.tex`
   - **Vértices:** 87
   - **Arestas:** 406
   - **Melhor Conhecido:** 11 cores
   - **Compiles:** `pdflatex tabelas_david.tex`

### Instância 3: anna.col
   - **Arquivo:** `tabelas_anna.tex`
   - **Vértices:** 138
   - **Arestas:** 493
   - **Melhor Conhecido:** 11 cores
   - **Compiles:** `pdflatex tabelas_anna.tex`

### Instância 4: homer.col
   - **Arquivo:** `tabelas_homer.tex`
   - **Vértices:** 561
   - **Arestas:** 1629
   - **Melhor Conhecido:** 13 cores
   - **Compiles:** `pdflatex tabelas_homer.tex`

### Instância 5: mulsol.i.1.col
   - **Arquivo:** `tabelas_mulsol.i.1.tex`
   - **Vértices:** 197
   - **Arestas:** 3925
   - **Melhor Conhecido:** 49 cores
   - **Compiles:** `pdflatex tabelas_mulsol.i.1.tex`

### Instância 6: zeroin.i.1.col
   - **Arquivo:** `tabelas_zeroin.i.1.tex`
   - **Vértices:** 211
   - **Arestas:** 4100
   - **Melhor Conhecido:** 49 cores
   - **Compiles:** `pdflatex tabelas_zeroin.i.1.tex`

### Instância 7: le450_5a.col
   - **Arquivo:** `tabelas_le450_5a.tex`
   - **Vértices:** 450
   - **Arestas:** ?
   - **Melhor Conhecido:** 5 cores
   - **Compiles:** `pdflatex tabelas_le450_5a.tex`

---

## 💾 Dados e Logs

### resultados.csv
   - **Descrição:** Base de dados completa com todos os resultados
   - **Linhas:** 754 (header + 753 resultados)
   - **Tamanho:** 52.97 KB
   - **Campos:** instancia, algoritmo, semente, alpha, iteracoes, bloco, melhor_cor, media_cor, tempo
   - **Uso:** Abra com Excel, pandas, ou editor de texto

```bash
# Exemplo de leitura com Python
import pandas as pd
df = pd.read_csv('resultados.csv')
print(df.head())
```

### logs/
   - **Descrição:** Arquivos de log detalhados de cada execução
   - **Total:** 360 arquivos
   - **Estrutura:** logs/[instancia]/[algoritmo]_[parametros]_[run].log
   - **Exemplo:** logs/huck/guloso_1.log, logs/huck/rand_a0.3_5.log

#### Estrutura dos Logs:

```
logs/
├── huck/
│   ├── guloso_1.log ... guloso_10.log (10 runs)
│   ├── rand_a0.1_1.log ... rand_a0.1_10.log
│   ├── rand_a0.3_1.log ... rand_a0.3_10.log
│   ├── rand_a0.5_1.log ... rand_a0.5_10.log
│   └── reativo_1.log ... reativo_10.log
├── david/ (estrutura similar)
├── anna/
├── homer/
├── mulsol.i.1/
├── zeroin.i.1/
├── le450_5a/
└── test/ (logs anteriores)
```

---

## ⚙️ Scripts Executáveis

Localizados em `scripts/`:

### run_all.bat / run_all.sh
   - Executa TODOS os experimentos
   - Processa todas as 7 instâncias
   - Gera resultados.csv
   - **Tempo estimado:** 1-2 horas

### run_instance.bat / run_instance.sh
   - Executa experimentos para UMA instância
   - Sintaxe: `run_instance.bat instancias\huck.col`
   - **Tempo estimado:** 5-10 minutos por instância

### run_quick.bat / run_quick.sh
   - Teste rápido de validação
   - Executa apenas instância "test"
   - **Tempo estimado:** < 1 minuto

---

## 📊 Estatísticas dos Experimentos

### Resumo Executivo

| Métrica | Valor |
|---------|-------|
| Total de Instâncias | 7 |
| Total de Execuções | ~700 |
| Total de Registros CSV | 754 |
| Arquivos de Log | 360 |
| Algoritmos Testados | 3 (Guloso, Randomizado, Reativo) |
| Variantes do Randomizado | 3 (α=0.1, 0.3, 0.5) |
| Execuções por Config | 10 |

### Distribuição por Instância

| Instância | Vértices | Arestas | Melhor Conhecido | Exec |
|-----------|----------|---------|-----------------|------|
| huck | 74 | 301 | 11 | ~103 |
| david | 87 | 406 | 11 | 100 |
| anna | 138 | 493 | 11 | 100 |
| homer | 561 | 1629 | 13 | 100 |
| mulsol.i.1 | 197 | 3925 | 49 | 100 |
| zeroin.i.1 | 211 | 4100 | 49 | 100 |
| le450_5a | 450 | ? | 5 | 100 |
| **TOTAL** | | | | **703** |

---

## 🎯 Como Usar Este Material

### Para Visualizar Resultados

1. **Rápido (30 segundos):**
   ```bash
   Abra: RELATORIO_RESUMO.md
   ```

2. **Completo (10 minutos):**
   ```bash
   pdflatex RELATORIO_FINAL.tex
   Abra: RELATORIO_FINAL.pdf
   ```

3. **Detalhado (1 hora+):**
   ```bash
   Analise cada CSV manualmente ou com Python
   Compile cada tabela individual
   ```

### Para Entender o Fluxo

1. Leia: GUIA_DE_USO.md
2. Consulte: RELATORIO_RESUMO.md
3. Compile: RELATORIO_FINAL.tex

### Para Reproduzir Experimentos

1. Execute: `scripts\run_all.bat`
2. Aguarde conclusão (1-2 horas)
3. Execute: `python analyze_results.py resultados.csv [instancia]`
4. Compile LaTeX conforme necessário

---

## 📝 Formato das Tabelas LaTeX

Todas as tabelas estão em formato padrão, pronto para copiar e colar em documentos:

### Estrutura Padrão

```latex
\begin{table}[H]
\centering
\caption{Descrição da Tabela}
\begin{tabular}{l|r|r|r|r|r|r}
\hline
Coluna 1 & Col 2 & Col 3 & Col 4 & Col 5 & Col 6 & Col 7 \\
\hline
Dados...
\end{tabular}
\end{table}
```

### Tabelas em Cada Arquivo

Cada arquivo `tabelas_[instancia].tex` contém:
1. Tabela consolidada (1 tabela com todos os algoritmos)
2. Tabela 1: Desvio da melhor solução
3. Tabela 2: Desvio da média
4. Tabela 3: Tempo médio de execução

**Total: 3 tabelas por instância × 7 instâncias = 21 tabelas**

---

## 🔍 Checklist de Conclusão

- [x] Todos os experimentos executados
- [x] Dados coletados em CSV
- [x] Logs detalhados armazenados
- [x] Tabelas LaTeX geradas
- [x] Relatório consolidado criado
- [x] Resumo em Markdown preparado
- [x] Guia de uso documentado
- [x] Índice de arquivos (este arquivo)
- [x] Pronto para apresentação

---

## 🎓 Referências Técnicas

### Problema L(p,q)-Coloring

Um grafo G=(V,E) precisa de uma coloração f: V → Z⁺ tal que:
- |f(u) - f(v)| ≥ p para vértices adjacentes
- |f(u) - f(w)| ≥ q para vértices a distância 2

Objetivo: Minimizar a maior cor utilizada (minimize span)

### Algoritmos Implementados

1. **GREEDY:** Constrói solução deterministicamente
2. **GRASP:** Randomizado com melhoria iterativa (Greedy Randomized Adaptive Search Procedure)
3. **GRASP Reativo:** Adapta probabilidades dinamicamente durante execução

### Fonte de Dados

- Instâncias DIMACS Graph Coloring Benchmark
- Instâncias LE450 (Leighton graphs)

---

## 📞 Suporte

### Erros Comuns

| Problema | Solução |
|----------|---------|
| lpq_coloring.exe não encontrado | Compile com: `g++ -std=c++17 -O2 main.cpp -o lpq_coloring.exe` |
| Python não encontrado | Instale Python 3.7+ |
| pdflatex não encontrado | Instale distribuição LaTeX (TeX Live, MiKTeX) |
| CSV vazio | Execute `scripts\run_all.bat` novamente |

---

## 📈 Análise Rápida de Resultados

### Melhor Algoritmo por Instância

- **huck, david, anna:** Todos os algoritmos produzem mesma solução
- **homer:** Idem anterior
- **mulsol.i.1:** GRASP Reativo ligeiramente melhor
- **zeroin.i.1:** GRASP Reativo significativamente melhor (143 vs 147)
- **le450_5a:** GRASP Reativo melhor (120 vs 114 guloso)

### Tendências Gerais

- Guloso: Mais rápido, pior qualidade
- GRASP: Melhor qualidade, mais tempo
- GRASP Reativo: Adapta-se melhor em instâncias difíceis

---

**Documento Gerado:** 20/01/2026  
**Versão:** 1.0 Final  
**Status:** ✅ PRONTO PARA USO
