# 🔧 Dicas Avançadas de Debugging e Extensão dos Testes (Documento escrito com ajuda de IA)

## 🎯 Debugging Avançado

### Executar com GDB/LLDB

```bash
# Compilar com símbolos de debug
g++ -std=c++17 -g -O0 tests.cpp -lgtest -lgtest_main -lpthread -o tests_debug

# Executar com gdb
gdb ./tests_debug

# Dentro do gdb:
(gdb) run --gtest_filter=TestDistance1.CaminhoSimples
(gdb) break verificarColoracao
(gdb) continue
(gdb) print coloracao
(gdb) print p
(gdb) print q
```

### Adicionar Prints de Debug

Adicione prints temporários nos testes:

```cpp
TEST_F(TestDistance1, DebugCaminho) {
    Grafo g = criarCaminho(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Debug: Imprimir coloração
    cout << "\nColoracao: ";
    for (int i = 0; i < coloracao.size(); i++) {
        cout << "v" << i << "=" << coloracao[i] << " ";
    }
    cout << "\n";
    
    // Debug: Verificar vizinhos
    for (int u = 0; u < g.getNumVertices(); u++) {
        cout << "v" << u << " vizinhos: ";
        for (int v : g.getVizinhos(u)) {
            cout << v << "(" << coloracao[v] << ") ";
        }
        cout << "\n";
    }
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}
```

### Usar ASSERT vs EXPECT

```cpp
// EXPECT: continua executando mesmo se falhar
EXPECT_EQ(a, b);
EXPECT_TRUE(condition);

// ASSERT: para a execução se falhar
ASSERT_EQ(a, b);  // Para aqui se falhar
EXPECT_TRUE(condition);  // Não executa se ASSERT falhar
```

### Testes Parametrizados

Para testar múltiplos valores automaticamente:

```cpp
class TestDistanceParametrizado : public ::testing::TestWithParam<tuple<int, int>> {
protected:
    void SetUp() override {}
};

TEST_P(TestDistanceParametrizado, CaminhoComDiferentesPQ) {
    auto [p, q] = GetParam();
    
    Grafo g = criarCaminho(10);
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

INSTANTIATE_TEST_SUITE_P(
    ValoresPQ,
    TestDistanceParametrizado,
    ::testing::Values(
        make_tuple(2, 1),
        make_tuple(3, 2),
        make_tuple(5, 3),
        make_tuple(10, 5)
    )
);
```

## 📊 Métricas e Cobertura

### Adicionar Medição de Cobertura

```bash
# Compilar com cobertura
g++ -std=c++17 --coverage tests.cpp -lgtest -lgtest_main -lpthread -o tests

# Executar testes
./tests

# Gerar relatório de cobertura
gcov tests.cpp
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
```

### Adicionar Profiling

```bash
# Compilar com profiling
g++ -std=c++17 -pg tests.cpp -lgtest -lgtest_main -lpthread -o tests

# Executar
./tests

# Gerar relatório
gprof tests gmon.out > profile.txt
```

## 🚀 Estendendo os Testes

### Adicionar Teste de Regressão

```cpp
// Salvar solução conhecida em arquivo
TEST_F(TestMinimization, RegressaoCaminho5) {
    Grafo g = criarCaminho(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Solução conhecida (pode variar dependendo da heurística)
    // Mas a maior cor deve estar em um range específico
    int maiorCor = calcularMaiorCor(coloracao);
    
    // Se a implementação mudar e piorar, isso vai detectar
    EXPECT_LE(maiorCor, 5) << "Regressao detectada! Solucao piorou.";
}
```

### Testes de Stress

```cpp
TEST_F(TestPerformance, StressTestGrandes) {
    // Testar múltiplos tamanhos
    vector<int> tamanhos = {100, 200, 500, 1000};
    
    for (int n : tamanhos) {
        int lado = (int)sqrt(n);
        Grafo g = criarGrade(lado, lado);
        
        auto inicio = chrono::high_resolution_clock::now();
        vector<int> coloracao = coloracaoGulosa(g, 2, 1);
        auto fim = chrono::high_resolution_clock::now();
        
        double tempo = chrono::duration<double>(fim - inicio).count();
        
        EXPECT_TRUE(verificarColoracao(g, coloracao, 2, 1, false))
            << "Falha para n=" << n;
        
        EXPECT_LT(tempo, 10.0) 
            << "Tempo excessivo para n=" << n << ": " << tempo << "s";
        
        cout << "n=" << n << " -> tempo=" << tempo << "s, cor=" 
             << calcularMaiorCor(coloracao) << "\n";
    }
}
```

### Testes de Propriedades

```cpp
// Propriedade: Em um caminho, cores devem alternar em padrão
TEST_F(TestDistance1, PropriedadePadraoCaminho) {
    Grafo g = criarCaminho(10);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Verificar que cores formam padrão esperado
    for (int i = 0; i < g.getNumVertices() - 1; i++) {
        // Vértices adjacentes devem ter cores diferentes por >= p
        EXPECT_GE(abs(coloracao[i] - coloracao[i+1]), p);
        
        // Em caminho, padrão tende a ser cíclico
        if (i < g.getNumVertices() - 2) {
            // Distância 2: também deve respeitar q
            EXPECT_GE(abs(coloracao[i] - coloracao[i+2]), q);
        }
    }
}
```

### Benchmarking Comparativo

```cpp
class TestBenchmark : public ::testing::Test {
protected:
    void CompararAlgoritmos(const Grafo& g, int p, int q) {
        // Guloso
        auto t1 = chrono::high_resolution_clock::now();
        vector<int> col1 = coloracaoGulosa(g, p, q);
        auto t2 = chrono::high_resolution_clock::now();
        double tempo1 = chrono::duration<double>(t2 - t1).count();
        
        // Randomizado
        mt19937 rng(42);
        auto t3 = chrono::high_resolution_clock::now();
        vector<int> col2 = coloracaoGulosaRandomizada(g, p, q, 0.3, 10, rng);
        auto t4 = chrono::high_resolution_clock::now();
        double tempo2 = chrono::duration<double>(t4 - t3).count();
        
        // Reativo
        mt19937 rng2(42);
        double melhorAlpha;
        vector<double> alphas = {0.1, 0.5, 0.9};
        auto t5 = chrono::high_resolution_clock::now();
        vector<int> col3 = coloracaoGulosaRandomizadaReativa(
            g, p, q, alphas, 15, 5, rng2, melhorAlpha);
        auto t6 = chrono::high_resolution_clock::now();
        double tempo3 = chrono::duration<double>(t6 - t5).count();
        
        // Resultados
        int cor1 = calcularMaiorCor(col1);
        int cor2 = calcularMaiorCor(col2);
        int cor3 = calcularMaiorCor(col3);
        
        cout << "\n--- Benchmark Comparativo ---\n";
        cout << "Guloso:      cor=" << cor1 << " tempo=" << tempo1 << "s\n";
        cout << "Randomizado: cor=" << cor2 << " tempo=" << tempo2 << "s\n";
        cout << "Reativo:     cor=" << cor3 << " tempo=" << tempo3 << "s\n";
        
        // Verificar que todos são válidos
        EXPECT_TRUE(verificarColoracao(g, col1, p, q, false));
        EXPECT_TRUE(verificarColoracao(g, col2, p, q, false));
        EXPECT_TRUE(verificarColoracao(g, col3, p, q, false));
    }
};

TEST_F(TestBenchmark, Grade10x10) {
    Grafo g = criarGrade(10, 10);
    CompararAlgoritmos(g, 2, 1);
}
```

## 🎨 Visualização de Grafos

### Exportar para GraphViz

```cpp
void exportarParaDot(const Grafo& g, const vector<int>& coloracao, 
                     const string& filename) {
    ofstream file(filename);
    file << "graph G {\n";
    
    // Nós com cores
    for (int i = 0; i < g.getNumVertices(); i++) {
        file << "  " << i << " [label=\"" << i << "\\n(" << coloracao[i] 
             << ")\", style=filled, fillcolor=grey" 
             << (coloracao[i] % 10) << "];\n";
    }
    
    // Arestas
    set<pair<int,int>> arestas;
    for (int u = 0; u < g.getNumVertices(); u++) {
        for (int v : g.getVizinhos(u)) {
            if (u < v) {
                file << "  " << u << " -- " << v << ";\n";
            }
        }
    }
    
    file << "}\n";
    file.close();
}

// Usar no teste:
TEST_F(TestMinimization, VisualizarCaminho) {
    Grafo g = criarCaminho(5);
    vector<int> coloracao = coloracaoGulosa(g, 2, 1);
    
    exportarParaDot(g, coloracao, "caminho.dot");
    // Depois: dot -Tpng caminho.dot -o caminho.png
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, 2, 1, false));
}
```

## 🔬 Análise Estatística Avançada

```cpp
TEST_F(TestRandomized, AnaliseEstatisticaCompleta) {
    Grafo g = criarCiclo(15);
    int p = 2, q = 1;
    int numExecucoes = 100;
    
    vector<int> resultados;
    vector<double> tempos;
    
    for (int seed = 0; seed < numExecucoes; seed++) {
        mt19937 rng(seed);
        
        auto inicio = chrono::high_resolution_clock::now();
        vector<int> coloracao = coloracaoGulosaRandomizada(g, p, q, 0.3, 10, rng);
        auto fim = chrono::high_resolution_clock::now();
        
        resultados.push_back(calcularMaiorCor(coloracao));
        tempos.push_back(chrono::duration<double>(fim - inicio).count());
        
        EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, false));
    }
    
    // Estatísticas
    sort(resultados.begin(), resultados.end());
    sort(tempos.begin(), tempos.end());
    
    double mediaRes = accumulate(resultados.begin(), resultados.end(), 0.0) / numExecucoes;
    double mediaTempo = accumulate(tempos.begin(), tempos.end(), 0.0) / numExecucoes;
    
    int minRes = resultados[0];
    int maxRes = resultados[numExecucoes - 1];
    int mediana = resultados[numExecucoes / 2];
    
    // Desvio padrão
    double variancia = 0;
    for (int r : resultados) {
        variancia += (r - mediaRes) * (r - mediaRes);
    }
    double desvio = sqrt(variancia / numExecucoes);
    
    cout << "\n--- Analise Estatistica (" << numExecucoes << " execucoes) ---\n";
    cout << "Resultados:\n";
    cout << "  Min:      " << minRes << "\n";
    cout << "  Max:      " << maxRes << "\n";
    cout << "  Mediana:  " << mediana << "\n";
    cout << "  Media:    " << mediaRes << "\n";
    cout << "  Desvio:   " << desvio << "\n";
    cout << "Tempo medio: " << mediaTempo << "s\n";
    
    // Distribuição (histograma simples)
    cout << "\nDistribuicao:\n";
    map<int, int> hist;
    for (int r : resultados) hist[r]++;
    for (auto [cor, freq] : hist) {
        cout << "  cor " << cor << ": " << string(freq / 2, '*') << " (" << freq << ")\n";
    }
}
```

## 🔄 Testes de Integração

```cpp
TEST_F(TestInputs, IntegracaoCompletaArquivo) {
    // 1. Criar arquivo
    vector<pair<int,int>> arestas = {{1,2}, {2,3}, {3,4}, {4,5}};
    criarArquivoGrafo("test_instances/integracao.txt", 5, arestas);
    
    // 2. Ler arquivo
    Grafo g = lerGrafoDoArquivo("test_instances/integracao.txt");
    
    // 3. Colorir com todos os algoritmos
    int p = 2, q = 1;
    
    vector<int> col1 = coloracaoGulosa(g, p, q);
    
    mt19937 rng(123);
    vector<int> col2 = coloracaoGulosaRandomizada(g, p, q, 0.5, 10, rng);
    
    double melhorAlpha;
    vector<double> alphas = {0.3, 0.7};
    mt19937 rng2(456);
    vector<int> col3 = coloracaoGulosaRandomizadaReativa(
        g, p, q, alphas, 10, 5, rng2, melhorAlpha);
    
    // 4. Verificar todos
    EXPECT_TRUE(verificarColoracao(g, col1, p, q, true));
    EXPECT_TRUE(verificarColoracao(g, col2, p, q, true));
    EXPECT_TRUE(verificarColoracao(g, col3, p, q, true));
    
    // 5. Comparar qualidade
    int cor1 = calcularMaiorCor(col1);
    int cor2 = calcularMaiorCor(col2);
    int cor3 = calcularMaiorCor(col3);
    
    cout << "\nIntegracao completa:\n";
    cout << "  Guloso: " << cor1 << "\n";
    cout << "  Random: " << cor2 << "\n";
    cout << "  Reativo: " << cor3 << "\n";
}
```

## 📝 Documentação dos Testes

### Gerar Relatório Markdown

```cpp
void gerarRelatorioTestes(const string& filename) {
    ofstream file(filename);
    
    file << "# Relatório de Testes - " << __DATE__ << " " << __TIME__ << "\n\n";
    file << "## Suites Executadas\n\n";
    file << "| Suite | Status | Tempo |\n";
    file << "|-------|--------|-------|\n";
    
    // Informações dos testes...
    
    file.close();
}
```

## 🎓 Boas Práticas

1. **Nomes descritivos**: `TestDistance1.CaminhoComP2Q1` é melhor que `Test1`

2. **Um conceito por teste**: Não teste múltiplas coisas no mesmo teste

3. **Arrange-Act-Assert**: 
   ```cpp
   // Arrange: preparar dados
   Grafo g = criarCaminho(5);
   
   // Act: executar ação
   vector<int> coloracao = coloracaoGulosa(g, 2, 1);
   
   // Assert: verificar resultado
   EXPECT_TRUE(verificarColoracao(g, coloracao, 2, 1));
   ```

4. **Fixtures para setup comum**: Use classes de teste para compartilhar setup

5. **Testes independentes**: Um teste não deve depender de outro

6. **Limpar recursos**: Use TearDown() para limpar arquivos temporários

7. **Mensagens claras**: 
   ```cpp
   EXPECT_EQ(a, b) << "Esperava " << b << " mas obteve " << a;
   ```

8. **Testes rápidos**: Mantenha testes unitários < 1ms quando possível

9. **Separar testes lentos**: Use fixtures diferentes para performance tests

10. **Versionamento**: Commite os testes junto com o código

---

**Documentação completa em**: [TESTES_README.md](TESTES_README.md)
