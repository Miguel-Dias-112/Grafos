// tests.cpp - Testes completos para L(p,q)-coloring
// Framework: GoogleTest
// Compilar: g++ -std=c++17 tests.cpp -lgtest -lgtest_main -pthread -o tests
// Executar: ./tests

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <numeric>
#include <fstream>
#include <sstream>

using namespace std;

// ============================================================================
// CÓPIA DAS CLASSES E FUNÇÕES NECESSÁRIAS PARA OS TESTES
// ============================================================================

class Grafo {
private:
    int n;
    vector<vector<int>> adj;
    vector<vector<int>> dist2;
    vector<int> grau;
    
public:
    Grafo(int vertices) : n(vertices) {
        adj.resize(n);
        grau.resize(n, 0);
    }
    
    void addAresta(int u, int v) {
        if (u < 0 || v < 0 || u >= n || v >= n) return;
        if (u == v) return;
        adj[u].push_back(v);
        adj[v].push_back(u);
        grau[u]++;
        grau[v]++;
    }
    
    void precomputarDistancia2() {
        dist2.resize(n);
        for (int i = 0; i < n; i++) {
            vector<bool> visitado(n, false);
            queue<pair<int, int>> q;
            q.push({i, 0});
            visitado[i] = true;
            
            while (!q.empty()) {
                int u = q.front().first;
                int d = q.front().second;
                q.pop();
                
                if (d == 2) {
                    dist2[i].push_back(u);
                    continue;
                }
                
                for (int v : adj[u]) {
                    if (!visitado[v]) {
                        visitado[v] = true;
                        q.push({v, d + 1});
                    }
                }
            }
        }
    }
    
    const vector<int>& getVizinhos(int v) const {
        return adj[v];
    }
    
    const vector<int>& getDistancia2(int v) const {
        return dist2[v];
    }
    
    int getGrau(int v) const {
        return grau[v];
    }
    
    int getNumVertices() const {
        return n;
    }
};

// Cópia do algoritmo guloso
vector<int> coloracaoGulosa(const Grafo& g, int p, int q) {
    int n = g.getNumVertices();
    vector<int> coloracao(n, -1);
    
    vector<int> vertices(n);
    iota(vertices.begin(), vertices.end(), 0);
    sort(vertices.begin(), vertices.end(), [&](int a, int b) {
        return g.getGrau(a) > g.getGrau(b);
    });
    
    auto corValida = [&](int v, int cor) -> bool {
        if (cor < 1) return false;
        for (int u : g.getVizinhos(v)) {
            if (coloracao[u] != -1 && abs(coloracao[u] - cor) < p) return false;
        }
        for (int u : g.getDistancia2(v)) {
            if (coloracao[u] != -1 && abs(coloracao[u] - cor) < q) return false;
        }
        return true;
    };

    for (int v : vertices) {
        int corEscolhida = 1;
        while (!corValida(v, corEscolhida)) {
            corEscolhida++;
        }
        coloracao[v] = corEscolhida;
    }
    
    return coloracao;
}

// Cópia do algoritmo guloso randomizado
vector<int> coloracaoGulosaRandomizada(const Grafo& g, int p, int q, double alpha,
                                       int iteracoes, mt19937& rng) {
    int n = g.getNumVertices();
    vector<int> melhorColoracao;
    int melhorMaiorCor = INT_MAX;
    
    for (int iter = 0; iter < iteracoes; iter++) {
        vector<int> coloracao(n, -1);
        int maiorCor = 0;
        
        vector<int> candidatos(n);
        iota(candidatos.begin(), candidatos.end(), 0);
        sort(candidatos.begin(), candidatos.end(), [&](int a, int b) {
            return g.getGrau(a) > g.getGrau(b);
        });
        
        while (!candidatos.empty()) {
            int k = max(1, (int)ceil(alpha * (double)candidatos.size()));
            k = min(k, (int)candidatos.size());
            
            uniform_int_distribution<int> dist(0, k - 1);
            int idx = dist(rng);
            int escolhido = candidatos[idx];
            
            candidatos.erase(candidatos.begin() + idx);
            
            auto corValida = [&](int v, int cor) -> bool {
                if (cor < 1) return false;
                for (int u : g.getVizinhos(v)) {
                    if (coloracao[u] != -1 && abs(coloracao[u] - cor) < p) return false;
                }
                for (int u : g.getDistancia2(v)) {
                    if (coloracao[u] != -1 && abs(coloracao[u] - cor) < q) return false;
                }
                return true;
            };
            int corEscolhida = 1;
            while (!corValida(escolhido, corEscolhida)) {
                corEscolhida++;
            }
            
            coloracao[escolhido] = corEscolhida;
            maiorCor = max(maiorCor, corEscolhida);
        }
        
        if (maiorCor < melhorMaiorCor) {
            melhorMaiorCor = maiorCor;
            melhorColoracao = coloracao;
        }
    }
    return melhorColoracao;
}

// Cópia do algoritmo reativo
vector<int> coloracaoGulosaRandomizadaReativa(const Grafo& g, int p, int q,
                                              const vector<double>& alphas,
                                              int iteracoesTotal, int bloco,
                                              mt19937& rng, double& melhorAlpha) {
    int n = g.getNumVertices();
    int numAlphas = alphas.size();
    
    vector<double> probabilidades(numAlphas, 1.0 / numAlphas);
    vector<double> somaQualidade(numAlphas, 0.0);
    vector<int> contador(numAlphas, 0);
    
    vector<int> melhorColoracaoGlobal;
    int melhorCorGlobal = INT_MAX;
    melhorAlpha = alphas[0];
    
    int realizadas = 0;
    while (realizadas < iteracoesTotal) {
        int limite = min(bloco, iteracoesTotal - realizadas);
        for (int it = 0; it < limite; ++it) {
            double r = uniform_real_distribution<double>(0.0, 1.0)(rng);
            double acc = 0.0;
            int idxAlpha = 0;
            for (int i = 0; i < numAlphas; i++) {
                acc += probabilidades[i];
                if (r <= acc) { idxAlpha = i; break; }
            }
            double alphaEscolhido = alphas[idxAlpha];
            vector<int> coloracao = coloracaoGulosaRandomizada(g, p, q, alphaEscolhido, 1, rng);
            int maiorCor = 0;
            for (int c : coloracao) maiorCor = max(maiorCor, c);
            somaQualidade[idxAlpha] += maiorCor;
            contador[idxAlpha] += 1;
            if (maiorCor < melhorCorGlobal) {
                melhorCorGlobal = maiorCor;
                melhorColoracaoGlobal = coloracao;
                melhorAlpha = alphaEscolhido;
            }
        }
        realizadas += limite;
        
        double somaScores = 0.0;
        vector<double> scores(numAlphas, 0.0);
        for (int i = 0; i < numAlphas; i++) {
            if (contador[i] > 0) {
                double media = somaQualidade[i] / (double)contador[i];
                scores[i] = 1.0 / (media + 1.0);
            } else {
                scores[i] = probabilidades[i];
            }
            somaScores += scores[i];
        }
        if (somaScores > 0.0) {
            for (int i = 0; i < numAlphas; i++) {
                probabilidades[i] = scores[i] / somaScores;
            }
        }
    }
    
    return melhorColoracaoGlobal;
}

// ============================================================================
// FUNÇÕES AUXILIARES PARA TESTES
// ============================================================================

/**
 * Verifica se a coloração satisfaz todas as restrições L(p,q)
 * Retorna true se válida, false caso contrário
 * Se verbose=true, imprime erros detalhados
 */
bool verificarColoracao(const Grafo& g, const vector<int>& coloracao, 
                       int p, int q, bool verbose = false) {
    int n = g.getNumVertices();
    
    // Verificar tamanho
    if ((int)coloracao.size() != n) {
        if (verbose) cout << "ERRO: tamanho da coloracao != numero de vertices\n";
        return false;
    }
    
    // Verificar cores válidas (>=1)
    for (int i = 0; i < n; ++i) {
        if (coloracao[i] < 1) {
            if (verbose) cout << "ERRO: cor invalida no vertice " << i << ": " << coloracao[i] << "\n";
            return false;
        }
    }

    // Verificar restrição de distância 1
    for (int u = 0; u < n; u++) {
        for (int v : g.getVizinhos(u)) {
            if (abs(coloracao[u] - coloracao[v]) < p) {
                if (verbose) {
                    cout << "ERRO: vertices adjacentes " << u << " e " << v 
                         << " violam p=" << p 
                         << " (cores: " << coloracao[u] << ", " << coloracao[v] << ")\n";
                }
                return false;
            }
        }
    }
    
    // Verificar restrição de distância 2
    for (int u = 0; u < n; u++) {
        for (int v : g.getDistancia2(u)) {
            if (abs(coloracao[u] - coloracao[v]) < q) {
                if (verbose) {
                    cout << "ERRO: vertices a distancia 2 " << u << " e " << v 
                         << " violam q=" << q 
                         << " (cores: " << coloracao[u] << ", " << coloracao[v] << ")\n";
                }
                return false;
            }
        }
    }
    
    return true;
}

/**
 * Calcula a maior cor usada em uma coloração
 */
int calcularMaiorCor(const vector<int>& coloracao) {
    int maior = 0;
    for (int cor : coloracao) maior = max(maior, cor);
    return maior;
}

/**
 * Cria um grafo caminho P_n
 * Estrutura: 0-1-2-...-n-1
 */
Grafo criarCaminho(int n) {
    Grafo g(n);
    for (int i = 0; i < n - 1; i++) {
        g.addAresta(i, i + 1);
    }
    g.precomputarDistancia2();
    return g;
}

/**
 * Cria um grafo ciclo C_n
 * Estrutura: 0-1-2-...-n-1-0
 */
Grafo criarCiclo(int n) {
    Grafo g(n);
    for (int i = 0; i < n - 1; i++) {
        g.addAresta(i, i + 1);
    }
    g.addAresta(n - 1, 0);
    g.precomputarDistancia2();
    return g;
}

/**
 * Cria um grafo estrela S_n (1 centro + n folhas)
 * Estrutura: centro 0 conectado a 1,2,...,n
 */
Grafo criarEstrela(int n) {
    Grafo g(n + 1);
    for (int i = 1; i <= n; i++) {
        g.addAresta(0, i);
    }
    g.precomputarDistancia2();
    return g;
}

/**
 * Cria um grafo completo K_n
 * Estrutura: todos os vértices conectados entre si
 */
Grafo criarCompleto(int n) {
    Grafo g(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            g.addAresta(i, j);
        }
    }
    g.precomputarDistancia2();
    return g;
}

/**
 * Cria um grafo grade 2D (grid) m x n
 */
Grafo criarGrade(int linhas, int colunas) {
    int n = linhas * colunas;
    Grafo g(n);
    
    auto idx = [&](int i, int j) { return i * colunas + j; };
    
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            // Aresta para direita
            if (j + 1 < colunas) {
                g.addAresta(idx(i, j), idx(i, j + 1));
            }
            // Aresta para baixo
            if (i + 1 < linhas) {
                g.addAresta(idx(i, j), idx(i + 1, j));
            }
        }
    }
    g.precomputarDistancia2();
    return g;
}

// ============================================================================
// SUITE 1: TESTES DE DISTÂNCIA 1
// ============================================================================

class TestDistance1 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TestDistance1, CaminhoSimples) {
    // Caminho P_5: 0-1-2-3-4
    // Para p=2, cores devem diferir em pelo menos 2
    Grafo g = criarCaminho(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Verificar que nenhum par de vértices adjacentes viola p
    for (int u = 0; u < g.getNumVertices(); u++) {
        for (int v : g.getVizinhos(u)) {
            EXPECT_GE(abs(coloracao[u] - coloracao[v]), p)
                << "Vertices adjacentes " << u << " e " << v 
                << " violam restricao de distancia 1 (p=" << p << ")";
        }
    }
}

TEST_F(TestDistance1, CicloOdd) {
    // Ciclo C_7 (ímpar)
    Grafo g = criarCiclo(7);
    int p = 3, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Verificar restrição de distância 1
    for (int u = 0; u < g.getNumVertices(); u++) {
        for (int v : g.getVizinhos(u)) {
            EXPECT_GE(abs(coloracao[u] - coloracao[v]), p)
                << "Falha na restricao de distancia 1 entre " << u << " e " << v;
        }
    }
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

TEST_F(TestDistance1, GrafoCompleto) {
    // K_5 - todos os vértices são adjacentes
    Grafo g = criarCompleto(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Em K_5 com p=2, todas as cores devem ser diferentes e espaçadas por >= 2
    for (int u = 0; u < g.getNumVertices(); u++) {
        for (int v : g.getVizinhos(u)) {
            EXPECT_GE(abs(coloracao[u] - coloracao[v]), p);
        }
    }
    
    // Maior cor esperada: pelo menos 1 + (n-1)*p = 1 + 4*2 = 9
    int maiorCor = calcularMaiorCor(coloracao);
    EXPECT_GE(maiorCor, 9) << "Grafo completo deveria usar cores mais altas";
}

TEST_F(TestDistance1, Estrela) {
    // Estrela S_10: centro conectado a 10 folhas
    Grafo g = criarEstrela(10);
    int p = 3, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Centro (vértice 0) deve ter cor diferente de todas as folhas por >= p
    int corCentro = coloracao[0];
    for (int i = 1; i <= 10; i++) {
        EXPECT_GE(abs(corCentro - coloracao[i]), p)
            << "Centro e folha " << i << " violam p=" << p;
    }
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

// ============================================================================
// SUITE 2: TESTES DE DISTÂNCIA 2
// ============================================================================

class TestDistance2 : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TestDistance2, CaminhoDistancia2) {
    // Caminho P_6: 0-1-2-3-4-5
    // Vértices a distância 2: (0,2), (1,3), (2,4), (3,5)
    Grafo g = criarCaminho(6);
    int p = 3, q = 2;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Verificar todas as restrições de distância 2
    for (int u = 0; u < g.getNumVertices(); u++) {
        for (int w : g.getDistancia2(u)) {
            EXPECT_GE(abs(coloracao[u] - coloracao[w]), q)
                << "Vertices a distancia 2 " << u << " e " << w 
                << " violam q=" << q 
                << " (cores: " << coloracao[u] << ", " << coloracao[w] << ")";
        }
    }
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

TEST_F(TestDistance2, CicloDistancia2) {
    // Ciclo C_5
    Grafo g = criarCiclo(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Em um ciclo, cada vértice tem exatamente 2 vértices a distância 2
    for (int u = 0; u < g.getNumVertices(); u++) {
        vector<int> dist2 = g.getDistancia2(u);
        for (int w : dist2) {
            EXPECT_GE(abs(coloracao[u] - coloracao[w]), q)
                << "Violacao de distancia 2 entre " << u << " e " << w;
        }
    }
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

TEST_F(TestDistance2, EstrelaSemDistancia2) {
    // Em estrela, folhas não têm vizinhos a distância 2 (exceto outras folhas via centro)
    Grafo g = criarEstrela(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Folhas (1-5) estão todas a distância 2 entre si
    for (int i = 1; i <= 5; i++) {
        for (int j : g.getDistancia2(i)) {
            EXPECT_GE(abs(coloracao[i] - coloracao[j]), q)
                << "Folhas " << i << " e " << j << " violam q=" << q;
        }
    }
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

TEST_F(TestDistance2, GradeDistancia2) {
    // Grade 3x3
    Grafo g = criarGrade(3, 3);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    // Verificar todas as restrições
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

// ============================================================================
// SUITE 3: TESTES DE MINIMIZAÇÃO E VALORES CONHECIDOS
// ============================================================================

class TestMinimization : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TestMinimization, CaminhoValorOtimo) {
    // Para caminho P_n com p=2, q=1:
    // Solução ótima conhecida: cores {1, 3, 2, 4, 3, 5, 4, ...}
    // Maior cor para P_5 ≈ 4
    Grafo g = criarCaminho(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    int maiorCor = calcularMaiorCor(coloracao);
    
    // Limite superior razoável
    EXPECT_LE(maiorCor, 5) << "Coloracao de caminho P_5 deveria usar no maximo 5 cores";
    EXPECT_GE(maiorCor, 3) << "Coloracao de caminho P_5 deveria usar pelo menos 3 cores";
}

TEST_F(TestMinimization, CicloImparValorRazoavel) {
    // Ciclo C_5 com p=2, q=1
    Grafo g = criarCiclo(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    int maiorCor = calcularMaiorCor(coloracao);
    
    // Para ciclo ímpar, esperamos valor razoável
    EXPECT_LE(maiorCor, 6) << "Ciclo C_5 deveria usar cores razoaveis";
    EXPECT_GE(maiorCor, 3);
}

TEST_F(TestMinimization, EstrelaBaixaCor) {
    // Estrela S_5 com p=2, q=1
    // Centro usa uma cor, folhas podem compartilhar cores respeitando q
    Grafo g = criarEstrela(5);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    int maiorCor = calcularMaiorCor(coloracao);
    
    // Estrela deveria ser eficiente
    EXPECT_LE(maiorCor, 7) << "Estrela deveria usar poucas cores";
}

TEST_F(TestMinimization, CompletoLimiteSuperior) {
    // K_4 com p=2, q=1
    Grafo g = criarCompleto(4);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    int maiorCor = calcularMaiorCor(coloracao);
    
    // K_4 precisa: 1, 3, 5, 7 (mínimo) -> maior cor = 7
    EXPECT_GE(maiorCor, 7) << "K_4 com p=2 precisa pelo menos cor 7";
    EXPECT_LE(maiorCor, 10) << "K_4 nao deveria ultrapassar cor 10";
}

// ============================================================================
// SUITE 4: TESTES DOS TRÊS ALGORITMOS
// ============================================================================

class TestGreedyAlgorithms : public ::testing::Test {
protected:
    void SetUp() override {
        rng.seed(12345);
    }
    mt19937 rng;
};

TEST_F(TestGreedyAlgorithms, GulosoGeraValida) {
    Grafo g = criarCaminho(10);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true))
        << "Algoritmo guloso deveria gerar coloracao valida";
}

TEST_F(TestGreedyAlgorithms, RandomizadoGeraValida) {
    Grafo g = criarCiclo(8);
    int p = 2, q = 1;
    double alpha = 0.5;
    int iteracoes = 10;
    
    vector<int> coloracao = coloracaoGulosaRandomizada(g, p, q, alpha, iteracoes, rng);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true))
        << "Algoritmo randomizado deveria gerar coloracao valida";
}

TEST_F(TestGreedyAlgorithms, ReativoGeraValida) {
    Grafo g = criarEstrela(8);
    int p = 2, q = 1;
    vector<double> alphas = {0.1, 0.3, 0.5, 0.7, 0.9};
    int iteracoes = 20;
    int bloco = 5;
    double melhorAlpha = 0;
    
    vector<int> coloracao = coloracaoGulosaRandomizadaReativa(
        g, p, q, alphas, iteracoes, bloco, rng, melhorAlpha);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true))
        << "Algoritmo reativo deveria gerar coloracao valida";
    
    // Verificar que melhorAlpha foi setado
    EXPECT_GT(melhorAlpha, 0.0);
    EXPECT_LE(melhorAlpha, 1.0);
}

TEST_F(TestGreedyAlgorithms, ComparacaoAlgoritmos) {
    // Comparar os três algoritmos no mesmo grafo
    Grafo g = criarGrade(4, 4);
    int p = 2, q = 1;
    
    vector<int> coloracaoGulosa1 = coloracaoGulosa(g, p, q);
    
    mt19937 rng2(54321);
    vector<int> coloracaoRand = coloracaoGulosaRandomizada(g, p, q, 0.3, 15, rng2);
    
    mt19937 rng3(99999);
    double melhorAlpha = 0;
    vector<double> alphas = {0.2, 0.5, 0.8};
    vector<int> coloracaoReativo = coloracaoGulosaRandomizadaReativa(
        g, p, q, alphas, 15, 5, rng3, melhorAlpha);
    
    // Todos devem ser válidos
    EXPECT_TRUE(verificarColoracao(g, coloracaoGulosa1, p, q, false));
    EXPECT_TRUE(verificarColoracao(g, coloracaoRand, p, q, false));
    EXPECT_TRUE(verificarColoracao(g, coloracaoReativo, p, q, false));
    
    // Verificar que as soluções podem ser diferentes
    int cor1 = calcularMaiorCor(coloracaoGulosa1);
    int cor2 = calcularMaiorCor(coloracaoRand);
    int cor3 = calcularMaiorCor(coloracaoReativo);
    
    EXPECT_GT(cor1, 0);
    EXPECT_GT(cor2, 0);
    EXPECT_GT(cor3, 0);
}

// ============================================================================
// SUITE 5: TESTES COM MÚLTIPLAS SEMENTES
// ============================================================================

class TestRandomized : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TestRandomized, MultiplasExecucoesSaoValidas) {
    // Executar 20 vezes com sementes diferentes
    Grafo g = criarCiclo(10);
    int p = 2, q = 1;
    double alpha = 0.4;
    int numExecucoes = 20;
    
    vector<int> maioresCores;
    
    for (int seed = 1; seed <= numExecucoes; seed++) {
        mt19937 rng(seed);
        vector<int> coloracao = coloracaoGulosaRandomizada(g, p, q, alpha, 5, rng);
        
        // Verificar validade
        EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, false))
            << "Execucao com semente " << seed << " gerou coloracao invalida";
        
        // Registrar maior cor
        int maiorCor = calcularMaiorCor(coloracao);
        maioresCores.push_back(maiorCor);
    }
    
    // Verificar que nenhuma violação ocorreu
    EXPECT_EQ(maioresCores.size(), numExecucoes);
    
    // Calcular estatísticas
    int minCor = *min_element(maioresCores.begin(), maioresCores.end());
    int maxCor = *max_element(maioresCores.begin(), maioresCores.end());
    double media = accumulate(maioresCores.begin(), maioresCores.end(), 0.0) / numExecucoes;
    
    EXPECT_GT(minCor, 0);
    EXPECT_GT(maxCor, 0);
    EXPECT_GT(media, 0.0);
    
    // Imprimir estatísticas para referência
    cout << "\n--- Estatisticas de " << numExecucoes << " execucoes ---\n";
    cout << "Menor cor: " << minCor << "\n";
    cout << "Maior cor: " << maxCor << "\n";
    cout << "Media: " << media << "\n";
    cout << "Variacao: " << (maxCor - minCor) << "\n";
}

TEST_F(TestRandomized, EstabilidadeRandomizado) {
    // Verificar estabilidade em grafo pequeno
    Grafo g = criarCaminho(7);
    int p = 2, q = 1;
    
    vector<int> resultados;
    for (int seed = 100; seed < 120; seed++) {
        mt19937 rng(seed);
        vector<int> coloracao = coloracaoGulosaRandomizada(g, p, q, 0.5, 10, rng);
        EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, false));
        resultados.push_back(calcularMaiorCor(coloracao));
    }
    
    // Verificar que os resultados não variam muito
    int minVal = *min_element(resultados.begin(), resultados.end());
    int maxVal = *max_element(resultados.begin(), resultados.end());
    
    EXPECT_LE(maxVal - minVal, 3) 
        << "Variacao entre execucoes deveria ser pequena para grafos simples";
}

// ============================================================================
// SUITE 6: TESTES DE LEITURA DE INSTÂNCIAS
// ============================================================================

class TestInputs : public ::testing::Test {
protected:
    void SetUp() override {
        // Criar diretório temporário para testes
        system("mkdir test_instances 2>nul");
    }
    
    void TearDown() override {
        // Limpar arquivos de teste
        system("del /Q test_instances\\* 2>nul");
        system("rmdir test_instances 2>nul");
    }
    
    void criarArquivoGrafo(const string& filename, int n, 
                          const vector<pair<int,int>>& arestas) {
        ofstream file(filename);
        file << "p edge " << n << " " << arestas.size() << "\n";
        for (auto [u, v] : arestas) {
            file << "e " << u << " " << v << "\n";
        }
        file.close();
    }
    
    Grafo lerGrafoDoArquivo(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Erro ao abrir arquivo");
        }
        
        string line;
        int n = 0, m = 0;
        bool headerFound = false;
        
        while (getline(file, line)) {
            if (line.empty()) continue;
            if (line[0] == 'c' || line[0] == 'C') continue;
            if (line[0] == 'p' || line[0] == 'P') {
                istringstream iss(line);
                string p, edge;
                iss >> p >> edge >> n >> m;
                headerFound = true;
                break;
            }
        }
        
        if (!headerFound || n <= 0) {
            throw runtime_error("Cabecalho invalido");
        }
        
        Grafo g(n);
        
        file.clear();
        file.seekg(0);
        while (getline(file, line)) {
            if (line.empty()) continue;
            if (line[0] == 'c' || line[0] == 'C') continue;
            if (line[0] == 'e' || line[0] == 'E') {
                istringstream iss(line);
                string e;
                int u, v;
                iss >> e >> u >> v;
                if (u >= 1 && v >= 1 && u <= n && v <= n) {
                    g.addAresta(u - 1, v - 1);
                }
            }
        }
        
        file.close();
        g.precomputarDistancia2();
        return g;
    }
};

TEST_F(TestInputs, LeituraCaminhoSimples) {
    // Criar arquivo com caminho P_4
    vector<pair<int,int>> arestas = {{1,2}, {2,3}, {3,4}};
    criarArquivoGrafo("test_instances/caminho.txt", 4, arestas);
    
    Grafo g = lerGrafoDoArquivo("test_instances/caminho.txt");
    
    EXPECT_EQ(g.getNumVertices(), 4);
    EXPECT_EQ(g.getGrau(0), 1);  // vértice 0 tem grau 1
    EXPECT_EQ(g.getGrau(1), 2);  // vértice 1 tem grau 2
    EXPECT_EQ(g.getGrau(2), 2);  // vértice 2 tem grau 2
    EXPECT_EQ(g.getGrau(3), 1);  // vértice 3 tem grau 1
}

TEST_F(TestInputs, LeituraCiclo) {
    // Criar arquivo com ciclo C_5
    vector<pair<int,int>> arestas = {{1,2}, {2,3}, {3,4}, {4,5}, {5,1}};
    criarArquivoGrafo("test_instances/ciclo.txt", 5, arestas);
    
    Grafo g = lerGrafoDoArquivo("test_instances/ciclo.txt");
    
    EXPECT_EQ(g.getNumVertices(), 5);
    // Todos os vértices têm grau 2 em um ciclo
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(g.getGrau(i), 2);
    }
}

TEST_F(TestInputs, LeituraComComentarios) {
    // Criar arquivo com comentários
    ofstream file("test_instances/com_comentarios.txt");
    file << "c Este eh um comentario\n";
    file << "c Outro comentario\n";
    file << "p edge 3 2\n";
    file << "c Mais um comentario\n";
    file << "e 1 2\n";
    file << "e 2 3\n";
    file.close();
    
    Grafo g = lerGrafoDoArquivo("test_instances/com_comentarios.txt");
    
    EXPECT_EQ(g.getNumVertices(), 3);
    EXPECT_EQ(g.getGrau(0), 1);
    EXPECT_EQ(g.getGrau(1), 2);
    EXPECT_EQ(g.getGrau(2), 1);
}

TEST_F(TestInputs, LeituraEColoracao) {
    // Criar grafo e testar coloração imediatamente
    vector<pair<int,int>> arestas = {{1,2}, {2,3}, {3,4}};
    criarArquivoGrafo("test_instances/teste_coloracao.txt", 4, arestas);
    
    Grafo g = lerGrafoDoArquivo("test_instances/teste_coloracao.txt");
    
    int p = 2, q = 1;
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

// ============================================================================
// SUITE 7: TESTES DE DESEMPENHO BÁSICO
// ============================================================================

class TestPerformance : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TestPerformance, GrafoMedio500Vertices) {
    // Criar grafo de grade 20x25 = 500 vértices
    auto inicio = chrono::high_resolution_clock::now();
    
    Grafo g = criarGrade(20, 25);
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, false))
        << "Coloracao de grafo grande deveria ser valida";
    
    EXPECT_LT(tempo, 10.0) << "Execucao nao deveria demorar mais de 10 segundos";
    
    cout << "\n--- Desempenho para 500 vertices ---\n";
    cout << "Tempo: " << tempo << " segundos\n";
    cout << "Maior cor: " << calcularMaiorCor(coloracao) << "\n";
}

TEST_F(TestPerformance, SemLoopInfinito) {
    // Testar com configuração que poderia causar loop infinito
    Grafo g = criarCompleto(10);
    int p = 5, q = 3;  // valores grandes
    
    auto inicio = chrono::high_resolution_clock::now();
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();
    
    EXPECT_LT(tempo, 5.0) << "Nao deveria entrar em loop infinito";
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, false));
}

TEST_F(TestPerformance, ComplexidadeDistancia2) {
    // Testar que precomputação de distância 2 funciona eficientemente
    auto inicio = chrono::high_resolution_clock::now();
    
    Grafo g = criarGrade(15, 15);  // 225 vértices
    
    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();
    
    EXPECT_LT(tempo, 3.0) << "Precomputacao de distancia 2 nao deveria demorar";
    
    // Verificar que distância 2 foi calculada
    int totalDist2 = 0;
    for (int i = 0; i < g.getNumVertices(); i++) {
        totalDist2 += g.getDistancia2(i).size();
    }
    
    EXPECT_GT(totalDist2, 0) << "Distancia 2 deveria ter sido calculada";
    
    cout << "\n--- Precomputacao de distancia 2 ---\n";
    cout << "Vertices: " << g.getNumVertices() << "\n";
    cout << "Total pares dist-2: " << totalDist2 << "\n";
    cout << "Tempo: " << tempo << " segundos\n";
}

TEST_F(TestPerformance, RandomizadoGrafoMedio) {
    // Testar algoritmo randomizado em grafo médio
    Grafo g = criarGrade(10, 10);  // 100 vértices
    int p = 2, q = 1;
    mt19937 rng(42);
    
    auto inicio = chrono::high_resolution_clock::now();
    
    vector<int> coloracao = coloracaoGulosaRandomizada(g, p, q, 0.3, 10, rng);
    
    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, false));
    EXPECT_LT(tempo, 15.0) << "Randomizado nao deveria demorar excessivamente";
    
    cout << "\n--- Randomizado em grafo medio ---\n";
    cout << "Tempo: " << tempo << " segundos\n";
}

// ============================================================================
// SUITE 8: TESTES DE CASOS EXTREMOS
// ============================================================================

class TestEdgeCases : public ::testing::Test {
protected:
    void SetUp() override {}
};

TEST_F(TestEdgeCases, GrafoVazio) {
    Grafo g(1);  // Um único vértice sem arestas
    g.precomputarDistancia2();
    int p = 2, q = 1;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_EQ(coloracao.size(), 1);
    EXPECT_EQ(coloracao[0], 1) << "Vertice isolado deveria ter cor 1";
}

TEST_F(TestEdgeCases, DoisVerticesSemAresta) {
    Grafo g(2);  // Dois vértices sem conexão
    g.precomputarDistancia2();
    int p = 3, q = 2;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, false));
    // Vértices isolados podem ter a mesma cor
    EXPECT_GE(coloracao[0], 1);
    EXPECT_GE(coloracao[1], 1);
}

TEST_F(TestEdgeCases, ValoresPGrandesCompletoK3) {
    Grafo g = criarCompleto(3);
    int p = 10, q = 5;  // Valores muito grandes
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
    
    // Com p=10, cores devem ser: 1, 11, 21
    int maiorCor = calcularMaiorCor(coloracao);
    EXPECT_GE(maiorCor, 21);
}

TEST_F(TestEdgeCases, PEqualsQ) {
    // Caso onde p = q
    Grafo g = criarCaminho(6);
    int p = 3, q = 3;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

TEST_F(TestEdgeCases, QMaiorQueP) {
    // Caso onde q > p (menos comum)
    Grafo g = criarCiclo(6);
    int p = 1, q = 3;
    
    vector<int> coloracao = coloracaoGulosa(g, p, q);
    
    EXPECT_TRUE(verificarColoracao(g, coloracao, p, q, true));
}

// ============================================================================
// MAIN DO GOOGLETEST
// ============================================================================

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    cout << "=========================================\n";
    cout << "  TESTES L(p,q)-COLORING - SUITE COMPLETA\n";
    cout << "=========================================\n\n";
    
    int result = RUN_ALL_TESTS();
    
    cout << "\n=========================================\n";
    cout << "  FIM DOS TESTES\n";
    cout << "=========================================\n";
    
    return result;
}
