# 📊 Guia Rápido - Gerar visualizacao_csacademy.txt

## 🎯 O Que É?

O arquivo `visualizacao_csacademy.txt` é a **representação de um grafo no formato DIMACS estendido**, compatível com ferramentas de visualização online e offline.

**Estrutura:**
```
450 5714
0 0 0 0 0 0 ... (450 cores, inicialmente todas 0 = sem cor)
1 7
1 13
1 54
... (5714 arestas)
```

---

## 📍 Duas Formas de Gerar

### ✅ Forma 1: Automático (Durante Experimentos)

Quando você executa os experimentos:
```bash
scripts\run_all.bat
```

O programa C++ gera automaticamente:
- `visualizacao_csacademy.txt` - Para a **última instância processada**

**Vantagem:** Automático, sem ação extra

**Desvantagem:** Só gera para uma instância por vez

---

### ✅ Forma 2: Manual (Script Python)

Use o novo script `gerar_visualizacoes.py` para gerar para **TODAS** as instâncias:

```bash
python gerar_visualizacoes.py
```

**Resultado:**
```
✅ anna_visualizacao.txt
✅ david_visualizacao.txt
✅ fpsol2.i.1_visualizacao.txt
✅ homer_visualizacao.txt
✅ huck_visualizacao.txt
✅ le450_5a_visualizacao.txt
✅ mulsol.i.1_visualizacao.txt
✅ zeroin.i.1_visualizacao.txt
```

**Vantagem:** Gera tudo de uma vez para todas as instâncias

**Desvantagem:** Requer uma chamada ao script

---

## 🚀 Comandos Rápidos

| Tarefa | Comando |
|--------|---------|
| Gerar todas as visualizações | `python gerar_visualizacoes.py` |
| Gerar para uma instância | `python gerar_visualizacoes.py --instancia instancias/test.col` |
| Gerar em diretório específico | `python gerar_visualizacoes.py --output visualizacoes/` |
| Ver todas as opções | `python gerar_visualizacoes.py --help` |

---

## 📊 Exemplo Real

### Passo 1: Gerar visualizações para todas as instâncias

```bash
python gerar_visualizacoes.py
```

Output:
```
Processando: anna.col... ✅ 138 vértices, 986 arestas
Processando: david.col... ✅ 87 vértices, 812 arestas
Processando: homer.col... ✅ 561 vértices, 3258 arestas
...
RESUMO: 13 ✅ | 0 ❌
```

### Passo 2: Verificar os arquivos gerados

```bash
ls *_visualizacao.txt
```

### Passo 3: Usar em ferramentas

**Online (CSAcademy):**
1. Acesse https://csacademy.com/app/graph_editor/
2. Cole o conteúdo do arquivo
3. Visualize o grafo

**Python:**
```python
import networkx as nx

def ler_csacademy(filename):
    with open(filename, 'r') as f:
        n, m = map(int, f.readline().split())
        cores = list(map(int, f.readline().split()))
        g = nx.Graph()
        for i in range(1, n+1):
            g.add_node(i)
        for line in f:
            u, v = map(int, line.split())
            g.add_edge(u, v)
    return g, cores

g, cores = ler_csacademy('anna_visualizacao.txt')
print(f"Grafo: {g.number_of_nodes()} vértices, {g.number_of_edges()} arestas")
```

---

## 📋 Checklist

- [ ] Ter instâncias DIMACS em `instancias/` (arquivos `.col`)
- [ ] Ter o script `gerar_visualizacoes.py` no diretório raiz
- [ ] Executar: `python gerar_visualizacoes.py`
- [ ] Verificar que foram criados `*_visualizacao.txt`
- [ ] Usar os arquivos para visualização ou análise

---

## 🔗 Referências

- **Script:** `gerar_visualizacoes.py`
- **Guia Completo:** [COMO_GERAR_VISUALIZACOES.md](COMO_GERAR_VISUALIZACOES.md)
- **Formato DIMACS:** http://lcs.ios.ac.cn/~caisw/Coloring/
- **CSAcademy:** https://csacademy.com/

---

**Status:** ✅ Script testado e funcionando!

13 arquivos de visualização gerados com sucesso.
