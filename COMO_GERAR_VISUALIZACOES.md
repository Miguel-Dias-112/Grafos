# 📊 Como Gerar Arquivos de Visualização CSAcademy

O arquivo `visualizacao_csacademy.txt` é um arquivo de grafo no formato DIMACS estendido, compatível com a plataforma CSAcademy para visualização de grafos.

## 🎯 Funcionamento

Os arquivos de visualização podem ser gerados de duas formas:

### 1️⃣ **Automático durante os experimentos** (já ocorre)

Quando você executa `scripts\run_all.bat`, o programa C++ gera automaticamente:
- `visualizacao_csacademy.txt` - Para a **última instância** processada

### 2️⃣ **Manual com script Python** (novo)

Use o script `gerar_visualizacoes.py` para gerar arquivos de visualização para **todas as instâncias** de uma vez.

---

## 🚀 Como Usar o Script

### Gerar para TODAS as instâncias

```bash
python gerar_visualizacoes.py
```

**Resultado:**
```
Processando: anna.col... ✅ 152 vértices, 736 arestas (11.2 KB)
Processando: david.col... ✅ 87 vértices, 406 arestas (6.1 KB)
Processando: huck.col... ✅ 74 vértices, 301 arestas (4.5 KB)
...
RESUMO: 7 ✅ | 0 ❌
```

Cria arquivos como:
- `anna_visualizacao.txt`
- `david_visualizacao.txt`
- `huck_visualizacao.txt`
- `le450_5a_visualizacao.txt`
- ... (um para cada instância)

### Gerar para uma instância específica

```bash
python gerar_visualizacoes.py --instancia instancias/test.col
```

Cria: `test_visualizacao.txt`

### Especificar diretório de saída

```bash
python gerar_visualizacoes.py --output visualizacoes/
```

Cria os arquivos em `visualizacoes/` em vez do diretório atual.

### Gerar com coloração (dados de uma solução)

Se você tem um arquivo `coloracao.txt` com a coloração de uma instância:

```bash
python gerar_visualizacoes.py --instancia instancias/test.col --coloracao coloracao.txt
```

Cria: `test_colorido.txt` com as cores já atribuídas.

---

## 📋 Formato do Arquivo Gerado

**Estrutura do arquivo de visualização:**

```
450 5714
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ...
1 330
1 367
1 389
...
448 450
```

**Linhas:**
- **Linha 1**: `n m` = número de vértices (n) e arestas (m)
- **Linha 2**: Cores de cada vértice (0 = sem cor atribuída)
- **Linhas 3+**: Arestas em formato `u v`

---

## 🔄 Fluxo Completo de Trabalho

```
instancias/*.col (arquivos DIMACS)
         ↓
   gerar_visualizacoes.py
         ↓
*_visualizacao.txt (arquivos CSAcademy)
         ↓
   Importar em ferramentas de visualização
```

---

## 💾 Todos os Arquivos Gerados

Ao rodar `python gerar_visualizacoes.py`, são criados:

```
anna_visualizacao.txt          (152v, 736a)
david_visualizacao.txt         (87v, 406a)
fpsol2.i.1_visualizacao.txt    (496v, 19315a)
homer_visualizacao.txt         (561v, 3258a)
huck_visualizacao.txt          (74v, 301a)
inithx.i.1_visualizacao.txt    (864v, 18632a)
le450_15a_visualizacao.txt     (450v, 8169a)
le450_25a_visualizacao.txt     (450v, 8260a)
le450_5a_visualizacao.txt      (450v, 5714a)
mulsol.i.1_visualizacao.txt    (197v, 3925a)
school1_visualizacao.txt       (385v, 19095a)
test_visualizacao.txt          (5v, 4a)
zeroin.i.1_visualizacao.txt    (211v, 4100a)
```

---

## 🛠️ Opções Completas do Script

```bash
python gerar_visualizacoes.py --help

Opções:
  -o, --output DIR      Diretório de saída (padrão: .)
  -i, --instancia FILE  Arquivo de instância específico
  -c, --coloracao FILE  Arquivo de coloração para visualizar com cores
  -d, --dir DIR         Diretório com instâncias (padrão: instancias)
  -h, --help            Mostrar ajuda
```

---

## 📊 Exemplos Práticos

### Exemplo 1: Gerar todas as visualizações em um diretório específico

```bash
mkdir -p visualizacoes_csacademy
python gerar_visualizacoes.py --output visualizacoes_csacademy/
```

Resultado:
```
📁 visualizacoes_csacademy/
   ├─ anna_visualizacao.txt
   ├─ david_visualizacao.txt
   ├─ homer_visualizacao.txt
   └─ ... (13 arquivos)
```

### Exemplo 2: Gerar para uma instância com coloração específica

```bash
# Primeiro, execute um experimento para gerar coloracao.txt
python main.exe instancias/test.col

# Depois, gere a visualização com cores
python gerar_visualizacoes.py --instancia instancias/test.col --coloracao coloracao.txt
```

### Exemplo 3: Script em lote para processar tudo

```bash
@echo off
REM Gerar visualizações de todas as instâncias
python gerar_visualizacoes.py --output visualizacoes/

REM Gerar resultados consolidados
python analyze_results.py resultados.csv test
python analyze_results.py resultados.csv anna
python analyze_results.py resultados.csv david

echo Pronto!
```

---

## 🔍 Como Usar os Arquivos Gerados

### Em Python (NetworkX)

```python
import networkx as nx
import matplotlib.pyplot as plt

def ler_csacademy(filename):
    with open(filename, 'r') as f:
        n, m = map(int, f.readline().split())
        cores = list(map(int, f.readline().split()))
        
        g = nx.Graph()
        g.add_nodes_from(range(1, n+1))
        
        for line in f:
            u, v = map(int, line.split())
            g.add_edge(u, v)
    
    return g, cores

# Carregar e visualizar
g, cores = ler_csacademy('anna_visualizacao.txt')
nx.draw_spring_layout(g, node_color=cores, with_labels=True)
plt.show()
```

### Online (CSAcademy)

1. Acesse https://csacademy.com/app/graph_editor/
2. Cole o conteúdo do arquivo `.txt` na aba "Input"
3. Clique em "Visualize"

### Em Gephi

1. Abra Gephi
2. Converta o arquivo TXT para formato apropriado (CSV ou GraphML)
3. Importe em Gephi

---

## ✅ Checklist de Uso

- [ ] Ter instâncias em `instancias/` (arquivos `.col`)
- [ ] Ter `gerar_visualizacoes.py` no diretório
- [ ] Executar: `python gerar_visualizacoes.py`
- [ ] Verificar se foram criados `*_visualizacao.txt`
- [ ] Conferir número de vértices e arestas
- [ ] Usar os arquivos para visualização/análise

---

## 🐛 Possíveis Problemas

### Problema: "Nenhuma instância encontrada"

**Causa:** Diretório `instancias/` vazio ou não existe

**Solução:**
```bash
# Verificar se tem arquivos
ls instancias/

# Se não tiver, copie instâncias DIMACS para lá
```

### Problema: Arquivo não gerado

**Causa:** Erro ao processar o arquivo DIMACS

**Solução:**
1. Verificar se o arquivo `.col` é DIMACS válido
2. Tentar gerar uma instância específica para ver o erro:
   ```bash
   python gerar_visualizacoes.py --instancia instancias/test.col
   ```

### Problema: Coloração não carregada

**Causa:** Formato do arquivo de coloração incorreto

**Solução:**
O arquivo deve ter:
- Uma cor por linha, OU
- Cores separadas por espaço (tudo em uma linha)

Exemplos válidos:
```
1
2
3
4
5
```

Ou:
```
1 2 3 4 5 6 7 8 9 10
```

---

## 📚 Referências

- **DIMACS Format**: http://lcs.ios.ac.cn/~caisw/Coloring/
- **CSAcademy**: https://csacademy.com/
- **NetworkX (Python)**: https://networkx.org/

---

**Resumo:** Use `gerar_visualizacoes.py` para converter seus arquivos DIMACS em visualizações CSAcademy prontas para análise e visualização! 📊✨
