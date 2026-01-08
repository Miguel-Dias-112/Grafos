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
    
    // Ler cabeçalho
    while (getline(file, line)) {
        if (line[0] == 'p') {
            istringstream iss(line);
            string p, edge;
            iss >> p >> edge >> n >> m;
            break;
        }
    }
    
    Grafo g(n);
    
    // Ler arestas
    file.clear();
    file.seekg(0);
    while (getline(file, line)) {
        if (line[0] == 'e') {
            istringstream iss(line);
            string e;
            int u, v;
            iss >> e >> u >> v;
            g.addAresta(u - 1, v - 1);  // -1 para índice 0-based
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
    vector<bool> corUsada;
    int maiorCor = 0;
    
    // Ordenar vértices por grau decrescente
    vector<int> vertices(n);
    for (int i = 0; i < n; i++) vertices[i] = i;
    sort(vertices.begin(), vertices.end(), [&](int a, int b) {
        return g.getGrau(a) > g.getGrau(b);
    });
    
    for (int v : vertices) {
        // Encontrar menor cor disponível
        corUsada.assign(maiorCor + 2, false);
        
        // Verificar restrições com vizinhos (distância 1)
        // Para L(p,q)-coloring: |c(u) - c(v)| >= p para vizinhos
        for (int u : g.getVizinhos(v)) {
            if (coloracao[u] != -1) {
                // Marcar todas as cores que violam a restrição p
                for (int cor = 0; cor <= maiorCor + 1; cor++) {
                    if (abs(coloracao[u] - cor) < p) {
                        if (cor < (int)corUsada.size()) corUsada[cor] = true;
                    }
                }
            }
        }
        
        // Verificar restrições com vértices a distância 2
        // Para L(p,q)-coloring: |c(u) - c(v)| >= q para distância 2
        for (int u : g.getDistancia2(v)) {
            if (coloracao[u] != -1) {
                // Marcar todas as cores que violam a restrição q
                for (int cor = 0; cor <= maiorCor + 1; cor++) {
                    if (abs(coloracao[u] - cor) < q) {
                        if (cor < (int)corUsada.size()) corUsada[cor] = true;
                    }
                }
            }
        }
        
        // Encontrar primeira cor não usada
        int corEscolhida = 0;
        while (corEscolhida < (int)corUsada.size() && corUsada[corEscolhida]) {
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

vector<int> coloracaoGulosaRandomizada(const Grafo& g, int p, int q, int alpha, 
                                        mt19937& rng, int& iteracoes) {
    int n = g.getNumVertices();
    vector<int> melhorColoracao;
    int melhorMaiorCor = INT_MAX;
    
    for (int iter = 0; iter < iteracoes; iter++) {
        vector<int> coloracao(n, -1);
        vector<bool> disponivel(n, true);
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
            
            // Selecionar entre os alpha melhores (ou todos se menos que alpha)
            int k = min(alpha, (int)candidatos.size());
            uniform_int_distribution<int> dist(0, k - 1);
            int escolhido = candidatos[dist(rng)];
            
            // Colorir o vértice escolhido
            vector<bool> corUsada(maiorCor + 2, false);
            
            // Restrições com vizinhos (distância 1)
            // Para L(p,q)-coloring: |c(u) - c(v)| >= p para vizinhos
            for (int u : g.getVizinhos(escolhido)) {
                if (coloracao[u] != -1) {
                    // Marcar todas as cores que violam a restrição p
                    for (int cor = 0; cor <= maiorCor + 1; cor++) {
                        if (abs(coloracao[u] - cor) < p) {
                            if (cor < (int)corUsada.size()) corUsada[cor] = true;
                        }
                    }
                }
            }
            
            // Restrições com distância 2
            // Para L(p,q)-coloring: |c(u) - c(v)| >= q para distância 2
            for (int u : g.getDistancia2(escolhido)) {
                if (coloracao[u] != -1) {
                    // Marcar todas as cores que violam a restrição q
                    for (int cor = 0; cor <= maiorCor + 1; cor++) {
                        if (abs(coloracao[u] - cor) < q) {
                            if (cor < (int)corUsada.size()) corUsada[cor] = true;
                        }
                    }
                }
            }
            
            // Encontrar cor disponível
            int corEscolhida = 0;
            while (corEscolhida < (int)corUsada.size() && corUsada[corEscolhida]) {
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
    
    iteracoes = melhorMaiorCor;  // Retornar o valor da melhor cor encontrada
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
    vector<double> qualidadeMedia(numAlphas, 0);
    vector<int> contador(numAlphas, 0);
    
    vector<int> melhorColoracaoGlobal;
    int melhorCorGlobal = INT_MAX;
    melhorAlpha = alphas[0];
    
    for (int blocoAtual = 0; blocoAtual < iteracoesTotal; blocoAtual += bloco) {
        int iteracoesNesteBloco = min(bloco, iteracoesTotal - blocoAtual);
        
        // Para cada alpha, executar algumas iterações
        vector<double> resultadosAlpha(numAlphas, 0);
        vector<int> melhoresCoresAlpha(numAlphas, INT_MAX);
        vector<vector<int>> melhoresColoracoesAlpha(numAlphas);
        
        for (int idxAlpha = 0; idxAlpha < numAlphas; idxAlpha++) {
            int alphaInt = (int)round(alphas[idxAlpha]);
            int melhorCorLocal = INT_MAX;
            
            for (int iter = 0; iter < iteracoesNesteBloco / numAlphas; iter++) {
                int dummy = iteracoesNesteBloco;
                vector<int> coloracao = coloracaoGulosaRandomizada(g, p, q, alphaInt, rng, dummy);
                
                // Calcular maior cor usada
                int maiorCor = 0;
                for (int cor : coloracao) maiorCor = max(maiorCor, cor);
                
                resultadosAlpha[idxAlpha] += maiorCor;
                
                if (maiorCor < melhorCorLocal) {
                    melhorCorLocal = maiorCor;
                    melhoresColoracoesAlpha[idxAlpha] = coloracao;
                    melhoresCoresAlpha[idxAlpha] = maiorCor;
                }
                
                // Atualizar melhor global
                if (maiorCor < melhorCorGlobal) {
                    melhorCorGlobal = maiorCor;
                    melhorColoracaoGlobal = coloracao;
                    melhorAlpha = alphas[idxAlpha];
                }
            }
            
            qualidadeMedia[idxAlpha] = resultadosAlpha[idxAlpha] / (iteracoesNesteBloco / numAlphas);
            contador[idxAlpha]++;
        }
        
        // Atualizar probabilidades (reativo)
        double somaQualidades = 0;
        for (int i = 0; i < numAlphas; i++) {
            if (contador[i] > 0) {
                // Quanto menor a cor, melhor (inverter)
                double score = 1.0 / (qualidadeMedia[i] + 1);
                probabilidades[i] = score;
                somaQualidades += score;
            }
        }
        
        // Normalizar probabilidades
        if (somaQualidades > 0) {
            for (int i = 0; i < numAlphas; i++) {
                probabilidades[i] /= somaQualidades;
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
            coloracao = coloracaoGulosaRandomizada(g, p, q, (int)round(alpha), rng, iteracoes);
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
