// main.cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <climits>
#include <cmath>

using namespace std;

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

class Grafo {
private:
    int n;  // número de vértices
    vector<vector<int>> adj;
    vector<vector<int>> dist2;  // vértices a distância 2
    vector<int> grau;
    
public:
    Grafo(int vertices) : n(vertices) {
        adj.resize(n);
        grau.resize(n, 0);
    }
    
    void addAresta(int u, int v) {
        // Validar índices antes de adicionar
        if (u < 0 || v < 0 || u >= n || v >= n) return;
        if (u == v) return; // evitar laços
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
    
    void printInfo() const {
        cout << "Grafo com " << n << " vertices" << endl;
        int arestas = 0;
        for (int i = 0; i < n; i++) arestas += adj[i].size();
        cout << "Arestas: " << arestas / 2 << endl;
    }
};

// ============================================================================
// LEITURA DE ARQUIVOS
// ============================================================================

Grafo lerGrafoDoArquivo(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo: " << filename << endl;
        exit(1);
    }
    
    string line;
    int n = 0, m = 0;
    bool headerFound = false;
    
    // Ler cabeçalho
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == 'c' || line[0] == 'C') continue; // comentários
        if (line[0] == 'p' || line[0] == 'P') {
            istringstream iss(line);
            string p, edge;
            iss >> p >> edge >> n >> m;
            headerFound = true;
            break;
        }
    }
    if (!headerFound || n <= 0) {
        cerr << "Arquivo de instância inválido: cabeçalho 'p edge n m' não encontrado." << endl;
        exit(1);
    }
    
    Grafo g(n);
    
    // Ler arestas
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
            // Validar intervalo 1..n
            if (u >= 1 && v >= 1 && u <= n && v <= n) {
                g.addAresta(u - 1, v - 1);  // -1 para índice 0-based
            }
        }
    }
    
    file.close();
    g.precomputarDistancia2();
    return g;
}

// ============================================================================
// ALGORITMO GULOSO
// ============================================================================

vector<int> coloracaoGulosa(const Grafo& g, int p, int q) {
    int n = g.getNumVertices();
    vector<int> coloracao(n, -1);
    int maiorCor = 0;
    
    // Ordenar vértices por grau decrescente
    vector<int> vertices(n);
    for (int i = 0; i < n; i++) vertices[i] = i;
    sort(vertices.begin(), vertices.end(), [&](int a, int b) {
        return g.getGrau(a) > g.getGrau(b);
    });
    
    // Helper para validar cor
    auto corValida = [&](int v, int cor) -> bool {
        if (cor < 1) return false;
        // Vizinhos distância 1
        for (int u : g.getVizinhos(v)) {
            if (coloracao[u] != -1 && abs(coloracao[u] - cor) < p) return false;
        }
        // Vizinhos distância 2
        for (int u : g.getDistancia2(v)) {
            if (coloracao[u] != -1 && abs(coloracao[u] - cor) < q) return false;
        }
        return true;
    };

    for (int v : vertices) {
        // Escolher a menor cor válida (≥1), sem limite superior fixo
        int corEscolhida = 1;
        while (!corValida(v, corEscolhida)) {
            corEscolhida++;
        }
        coloracao[v] = corEscolhida;
        maiorCor = max(maiorCor, corEscolhida);
    }
    
    return coloracao;
}

// ============================================================================
// ALGORITMO GULOSO RANDOMIZADO
// ============================================================================

vector<int> coloracaoGulosaRandomizada(const Grafo& g, int p, int q, double alpha,
                                       int iteracoes, mt19937& rng) {
    int n = g.getNumVertices();
    vector<int> melhorColoracao;
    int melhorMaiorCor = INT_MAX;
    
    for (int iter = 0; iter < iteracoes; iter++) {
        vector<int> coloracao(n, -1);
        int maiorCor = 0;
        int verticesColoridos = 0;
        
        while (verticesColoridos < n) {
            // Criar lista de candidatos (vértices não coloridos)
            vector<int> candidatos;
            for (int v = 0; v < n; v++) {
                if (coloracao[v] == -1) candidatos.push_back(v);
            }
            
            // Ordenar por grau
            sort(candidatos.begin(), candidatos.end(), [&](int a, int b) {
                return g.getGrau(a) > g.getGrau(b);
            });
            
            // Construir RCL com base em alpha ∈ [0,1]: top-k por grau
            int k = max(1, (int)ceil(alpha * (double)candidatos.size()));
            k = min(k, (int)candidatos.size());
            uniform_int_distribution<int> dist(0, k - 1);
            int escolhido = candidatos[dist(rng)];
            
            // Escolher a menor cor válida (≥1) para o escolhido
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
            verticesColoridos++;
        }
        
        if (maiorCor < melhorMaiorCor) {
            melhorMaiorCor = maiorCor;
            melhorColoracao = coloracao;
        }
    }
    return melhorColoracao;
}

// ============================================================================
// ALGORITMO GULOSO RANDOMIZADO REATIVO
// ============================================================================

vector<int> coloracaoGulosaRandomizadaReativa(const Grafo& g, int p, int q,
                                              const vector<double>& alphas,
                                              int iteracoesTotal, int bloco,
                                              mt19937& rng, double& melhorAlpha) {
    int n = g.getNumVertices();
    int numAlphas = alphas.size();
    
    // Inicializar probabilidades iguais
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
            // Amostrar índice de alpha segundo distribuição atual
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
        // Atualizar probabilidades com base na qualidade média observada
        double somaScores = 0.0;
        vector<double> scores(numAlphas, 0.0);
        for (int i = 0; i < numAlphas; i++) {
            if (contador[i] > 0) {
                double media = somaQualidade[i] / (double)contador[i];
                scores[i] = 1.0 / (media + 1.0); // menor cor => score maior
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
// FUNÇÕES AUXILIARES
// ============================================================================

int calcularMaiorCor(const vector<int>& coloracao) {
    int maior = 0;
    for (int cor : coloracao) maior = max(maior, cor);
    return maior;
}

bool verificarColoracao(const Grafo& g, const vector<int>& coloracao, int p, int q) {
    int n = g.getNumVertices();
    
    if ((int)coloracao.size() != n) {
        cout << "Erro: tamanho da coloracao difere do numero de vertices." << endl;
        return false;
    }
    for (int i = 0; i < n; ++i) {
        if (coloracao[i] < 1) {
            cout << "Erro: cor inválida (não positiva) no vertice " << i << ": " << coloracao[i] << endl;
            return false;
        }
    }

    for (int u = 0; u < n; u++) {
        for (int v : g.getVizinhos(u)) {
            if (abs(coloracao[u] - coloracao[v]) < p) {
                cout << "Erro: vertices " << u << " e " << v << " violam p=" << p 
                     << " (cores: " << coloracao[u] << ", " << coloracao[v] << ")" << endl;
                return false;
            }
        }
        
        for (int v : g.getDistancia2(u)) {
            if (abs(coloracao[u] - coloracao[v]) < q) {
                cout << "Erro: vertices " << u << " e " << v << " a distancia 2 violam q=" << q 
                     << " (cores: " << coloracao[u] << ", " << coloracao[v] << ")" << endl;
                return false;
            }
        }
    }
    
    return true;
}

void salvarColoracao(const string& filename, const vector<int>& coloracao) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro ao salvar coloracao" << endl;
        return;
    }
    
    for (size_t i = 0; i < coloracao.size(); i++) {
        file << i + 1 << " " << coloracao[i] << endl;
    }
    
    file.close();
}

void salvarResultadosCSV(const string& filename, const string& instancia, 
                         const string& algoritmo, double tempo, int melhorCor,
                         int semente = 0, double alpha = 0, int iteracoes = 0, 
                         int bloco = 0) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Erro ao abrir arquivo CSV" << endl;
        return;
    }
    
    // Cabeçalho (se arquivo vazio)
    file.seekp(0, ios::end);
    if (file.tellp() == 0) {
        file << "data_hora,instancia,algoritmo,alpha,iteracoes,bloco,semente,tempo,melhor_cor" << endl;
    }
    
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    
    file << put_time(localtime(&now_time), "%Y-%m-%d %H:%M:%S") << ","
         << instancia << ","
         << algoritmo << ","
         << alpha << ","
         << iteracoes << ","
         << bloco << ","
         << semente << ","
         << fixed << setprecision(4) << tempo << ","
         << melhorCor << endl;
    
    file.close();
}

// ============================================================================
// MAIN
// ============================================================================

int main(int argc, char* argv[]) {
    // Configuração padrão
    string instancia = "instancias/le450_5a.col";
    int p = 2, q = 1;
    int algoritmoEscolhido = 1;  // 1=guloso, 2=randomizado, 3=reativo
    int semente = chrono::system_clock::now().time_since_epoch().count();
    double alpha = 0.3;
    int iteracoes = 30;
    int bloco = 30;
    
    // Ler parâmetros da linha de comando
    if (argc > 1) instancia = argv[1];
    if (argc > 2) algoritmoEscolhido = stoi(argv[2]);
    if (argc > 3) semente = stoi(argv[3]);
    if (argc > 4) alpha = stod(argv[4]);
    if (argc > 5) iteracoes = stoi(argv[5]);
    if (argc > 6) bloco = stoi(argv[6]);
    
    cout << "=========================================" << endl;
    cout << "L(p,q)-COLORING - DCC059 Teoria dos Grafos" << endl;
    cout << "=========================================" << endl;
    cout << "Instancia: " << instancia << endl;
    cout << "Parametros: p=" << p << ", q=" << q << endl;
    cout << "Semente: " << semente << endl;
    
    // Inicializar gerador de números aleatórios
    mt19937 rng(semente);
    
    // Carregar grafo
    auto inicioCarregamento = chrono::high_resolution_clock::now();
    Grafo g = lerGrafoDoArquivo(instancia);
    auto fimCarregamento = chrono::high_resolution_clock::now();
    double tempoCarregamento = chrono::duration<double>(fimCarregamento - inicioCarregamento).count();
    
    g.printInfo();
    cout << "Tempo carregamento: " << tempoCarregamento << "s" << endl;
    
    // Executar algoritmo escolhido
    vector<int> coloracao;
    int maiorCor = 0;
    double tempoExecucao = 0;
    double alphaUsado = alpha;
    
    auto inicioExecucao = chrono::high_resolution_clock::now();
    
    switch (algoritmoEscolhido) {
        case 1:  // Guloso
            cout << "\nExecutando algoritmo GULOSO..." << endl;
            coloracao = coloracaoGulosa(g, p, q);
            break;
            
        case 2:  // Randomizado
            cout << "\nExecutando algoritmo GULOSO RANDOMIZADO..." << endl;
            cout << "Alpha: " << alpha << ", Iteracoes: " << iteracoes << endl;
            coloracao = coloracaoGulosaRandomizada(g, p, q, alpha, iteracoes, rng);
            break;
            
        case 3:  // Reativo
            cout << "\nExecutando algoritmo GULOSO RANDOMIZADO REATIVO..." << endl;
            cout << "Iteracoes total: " << iteracoes << ", Bloco: " << bloco << endl;
            // Definir lista de alphas (exemplo: 0.1, 0.3, 0.5, 0.7, 0.9)
            vector<double> alphas = {0.1, 0.3, 0.5, 0.7, 0.9};
            coloracao = coloracaoGulosaRandomizadaReativa(g, p, q, alphas, iteracoes, bloco, rng, alphaUsado);
            break;
    }
    
    auto fimExecucao = chrono::high_resolution_clock::now();
    tempoExecucao = chrono::duration<double>(fimExecucao - inicioExecucao).count();
    
    // Calcular maior cor
    maiorCor = calcularMaiorCor(coloracao);
    
    // Verificar validade da coloração
    cout << "\nVerificando coloracao..." << endl;
    if (verificarColoracao(g, coloracao, p, q)) {
        cout << "Coloracao VALIDA!" << endl;
    } else {
        cout << "Coloracao INVALIDA!" << endl;
    }
    
    // Resultados
    cout << "\n========== RESULTADOS ==========" << endl;
    cout << "Maior cor usada: " << maiorCor << endl;
    cout << "Tempo execucao: " << tempoExecucao << "s" << endl;
    if (algoritmoEscolhido == 3) {
        cout << "Melhor alpha encontrado: " << alphaUsado << endl;
    }
    
    // Salvar resultados em CSV
    string nomeInstancia = instancia.substr(instancia.find_last_of("/\\") + 1);
    string nomesAlgoritmos[] = {"", "GULOSO", "RANDOMIZADO", "REATIVO"};
    
    salvarResultadosCSV("resultados.csv", nomeInstancia, 
                       nomesAlgoritmos[algoritmoEscolhido],
                       tempoExecucao, maiorCor, semente, alphaUsado, iteracoes, bloco);
    
    // Salvar coloração em arquivo
    salvarColoracao("coloracao.txt", coloracao);
    
    cout << "\nColoracao salva em 'coloracao.txt'" << endl;
    cout << "Resultados salvos em 'resultados.csv'" << endl;
    cout << "=========================================" << endl;
    
    return 0;
}
