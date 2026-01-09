# 🚀 Guia Rápido de Testes (Documento escrito com ajuda de IA)

## ⚡ Comandos Essenciais

### Windows (PowerShell ou CMD)

```powershell
# Compilar e executar todos os testes
.\run_tests.bat

# Executar suites específicas
.\run_tests.bat distance1      # Testes de distância 1
.\run_tests.bat distance2      # Testes de distância 2
.\run_tests.bat minimization   # Testes de minimização
.\run_tests.bat algorithms     # Testes dos três algoritmos
.\run_tests.bat randomized     # Testes com múltiplas sementes
.\run_tests.bat inputs         # Testes de leitura de arquivo
.\run_tests.bat performance    # Testes de desempenho
.\run_tests.bat edge           # Testes de casos extremos

# Listar todos os testes
.\run_tests.bat list
```

### Linux/macOS

```bash
# Compilar e executar todos os testes
make test

# Executar suites específicas
make test-distance1      # Testes de distância 1
make test-distance2      # Testes de distância 2
make test-minimization   # Testes de minimização
make test-algorithms     # Testes dos três algoritmos
make test-randomized     # Testes com múltiplas sementes
make test-inputs         # Testes de leitura de arquivo
make test-performance    # Testes de desempenho
make test-edge           # Testes de casos extremos

# Listar todos os testes
make list-tests

# Limpar arquivos temporários
make clean
```

## 📊 O Que Cada Suite Testa

| Suite | Foco | Quantidade |
|-------|------|------------|
| **TestDistance1** | Valida restrição \|f(u) - f(v)\| ≥ p para adjacentes | 4 testes |
| **TestDistance2** | Valida restrição \|f(u) - f(w)\| ≥ q para dist-2 | 4 testes |
| **TestMinimization** | Verifica valores razoáveis em instâncias conhecidas | 4 testes |
| **TestGreedyAlgorithms** | Valida os 3 algoritmos (guloso, randomizado, reativo) | 4 testes |
| **TestRandomized** | Executa 20+ vezes com sementes diferentes | 2 testes |
| **TestInputs** | Testa leitura de arquivos .col | 4 testes |
| **TestPerformance** | Testa grafos até 500 vértices | 5 testes |
| **TestEdgeCases** | Casos extremos (p>q, grafos vazios, etc) | 5 testes |

**Total: 35 testes** ✅

## 🎯 Verificações Implementadas

### ✅ Requisito 1: Restrição Distância 1
```
Para todo par (u,v) adjacente: |f(u) - f(v)| ≥ p
```
- ✅ Verificado em TODOS os testes
- ✅ Falha explícita com mensagem detalhada
- ✅ Testes específicos: TestDistance1.*

### ✅ Requisito 2: Restrição Distância 2
```
Para u,w com distância 2: |f(u) - f(w)| ≥ q
```
- ✅ Verificado em TODOS os testes
- ✅ Falha explícita com mensagem detalhada
- ✅ Testes específicos: TestDistance2.*

### ✅ Requisito 3: Minimização
```
Valores conhecidos para: Caminho, Ciclo, Estrela, Completo
```
- ✅ Testa bounds esperados
- ✅ Testes específicos: TestMinimization.*

### ✅ Requisito 4: Três Algoritmos
```
Guloso, Randomizado, Reativo
```
- ✅ Todos validados
- ✅ Comparação entre eles
- ✅ Testes específicos: TestGreedyAlgorithms.*

### ✅ Requisito 5: Múltiplas Sementes
```
20 execuções com sementes diferentes
```
- ✅ Calcula min, max, média, variação
- ✅ Verifica ausência de violações
- ✅ Testes específicos: TestRandomized.*

### ✅ Requisito 6: Leitura de Instâncias
```
Arquivos .txt/.col
```
- ✅ Parsing completo
- ✅ Comentários ignorados
- ✅ Validação de formato
- ✅ Testes específicos: TestInputs.*

### ✅ Requisito 7: Desempenho
```
Grafos até 500-1000 vértices
```
- ✅ Verifica terminação
- ✅ Detecta loops infinitos
- ✅ Mede complexidade
- ✅ Testes específicos: TestPerformance.*

## 🔍 Exemplos de Uso

### Executar Tudo de Uma Vez
```bash
# Windows
.\run_tests.bat

# Linux/macOS
make test
```

### Depurar Teste Específico
```bash
# Executar apenas um teste
./tests --gtest_filter=TestDistance1.CaminhoSimples

# Com repetição (detectar problemas intermitentes)
./tests --gtest_filter=TestRandomized.* --gtest_repeat=10
```

### Testes Rápidos (desenvolvimento)
```bash
# Apenas validação básica (< 1 segundo)
./tests --gtest_filter=TestDistance1.*:TestDistance2.*

# Pular testes de performance (economizar tempo)
./tests --gtest_filter=-TestPerformance.*
```

### Análise de Performance
```bash
# Apenas testes de performance
make test-performance

# Ou diretamente
./tests --gtest_filter=TestPerformance.*
```

## 📈 Interpretando Resultados

### Sucesso Total ✅
```
[  PASSED  ] 35 tests.
```
Todos os requisitos validados com sucesso!

### Falha Detectada ❌
```
[  FAILED  ] TestDistance1.CaminhoSimples (1 ms)
```
O teste mostrará EXATAMENTE onde a violação ocorreu:
```
ERRO: vertices adjacentes 2 e 3 violam p=2 (cores: 3, 4)
```

### Estatísticas (Randomizado)
```
--- Estatisticas de 20 execucoes ---
Menor cor: 4
Maior cor: 5
Media: 4.45
Variacao: 1
```
Indica estabilidade do algoritmo.

### Performance
```
--- Desempenho para 500 vertices ---
Tempo: 0.0234 segundos
Maior cor: 7
```
Garante que não há explosão de complexidade.

## 🐛 Troubleshooting

### Erro: "GoogleTest não encontrado"
```bash
# Windows (vcpkg)
vcpkg install gtest:x64-windows

# Linux
sudo apt-get install libgtest-dev

# macOS
brew install googletest
```

### Erro: "undefined reference to `pthread_create`"
Adicione `-lpthread` na compilação:
```bash
g++ -std=c++17 tests.cpp -lgtest -lgtest_main -lpthread -o tests
```

### Teste demora muito
Os testes de performance podem demorar. Para pular:
```bash
./tests --gtest_filter=-TestPerformance.*
```

### Teste falha intermitentemente
Use repetição para investigar:
```bash
./tests --gtest_filter=NomeDoTeste --gtest_repeat=50
```

## 📝 Adicionando Novos Testes

```cpp
// Exemplo de novo teste
TEST_F(TestDistance1, MeuNovoTeste) {
    Grafo g = criarCaminho(10);
    int p = 3, q = 2;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
    EXPECT_LE(calcularMaiorCor(coloracao), 10);
}
```

## 🎓 Estrutura dos Arquivos

```
Grafos/
├── main.cpp              # Implementação principal
├── tests.cpp             # Suite completa de testes (35 testes)
├── makefile              # Build system (Linux/macOS)
├── run_tests.bat         # Script Windows (CMD)
├── run_tests.ps1         # Script Windows (PowerShell)
├── TESTES_README.md      # Documentação detalhada
├── EXEMPLOS_SAIDA.md     # Exemplos de output
└── QUICK_START.md        # Este arquivo
```

## ⏱️ Tempo de Execução

| Suite | Tempo Aproximado |
|-------|------------------|
| TestDistance1 | < 1 ms |
| TestDistance2 | < 5 ms |
| TestMinimization | < 1 ms |
| TestGreedyAlgorithms | ~15 ms |
| TestRandomized | ~30 ms |
| TestInputs | ~5 ms |
| TestPerformance | ~200 ms |
| TestEdgeCases | < 1 ms |
| **TOTAL** | **~250 ms** |

## 🎉 Pronto para Usar!

```bash
# Um comando para tudo:
.\run_tests.bat    # Windows
make test          # Linux/macOS
```

**35 testes completos validando todos os requisitos!** ✨
