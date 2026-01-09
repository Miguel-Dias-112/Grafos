# Testes para L(p,q)-Coloring (Documento escrito com ajuda de IA)

Este projeto contém uma suite completa de testes para validar a implementação do problema L(p,q)-coloring em C++.

## 📋 Requisitos

- **Compilador C++17**: GCC 7+ ou Clang 5+
- **GoogleTest**: Framework de testes do Google
- **Sistema operacional**: Windows, Linux ou macOS

## 🔧 Instalação do GoogleTest

### Windows (usando vcpkg)
```powershell
# Instalar vcpkg se ainda não tiver
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Instalar GoogleTest
.\vcpkg install gtest:x64-windows

# Integrar com Visual Studio (opcional)
.\vcpkg integrate install
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libgtest-dev cmake

# Compilar GoogleTest
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp lib/*.a /usr/lib
```

### macOS
```bash
brew install googletest
```

## 🔨 Compilação

### Opção 1: Usando g++ diretamente (Linux/macOS)
```bash
g++ -std=c++17 tests.cpp -lgtest -lgtest_main -pthread -o tests
```

### Opção 2: Usando g++ no Windows (MinGW)
```powershell
g++ -std=c++17 tests.cpp -lgtest -lgtest_main -lpthread -o tests.exe
```

### Opção 3: Usando o Makefile
```bash
make tests
```

## ▶️ Executando os Testes

### Executar todos os testes
```bash
./tests              # Linux/macOS
tests.exe            # Windows
```

### Executar uma suite específica
```bash
./tests --gtest_filter=TestDistance1.*
./tests --gtest_filter=TestDistance2.*
./tests --gtest_filter=TestMinimization.*
./tests --gtest_filter=TestGreedyAlgorithms.*
./tests --gtest_filter=TestRandomized.*
./tests --gtest_filter=TestInputs.*
./tests --gtest_filter=TestPerformance.*
./tests --gtest_filter=TestEdgeCases.*
```

### Executar um teste específico
```bash
./tests --gtest_filter=TestDistance1.CaminhoSimples
```

### Opções úteis
```bash
# Executar em modo verbose
./tests --gtest_verbose

# Listar todos os testes sem executar
./tests --gtest_list_tests

# Repetir testes múltiplas vezes
./tests --gtest_repeat=10

# Embaralhar ordem dos testes
./tests --gtest_shuffle
```

## 📊 Estrutura dos Testes

### Suite 1: TestDistance1
Valida a restrição de distância 1: para todo par de vértices adjacentes (u, v), |f(u) - f(v)| ≥ p

**Testes:**
- `CaminhoSimples`: Testa caminho P_5
- `CicloOdd`: Testa ciclo ímpar C_7
- `GrafoCompleto`: Testa grafo completo K_5
- `Estrela`: Testa grafo estrela S_10

### Suite 2: TestDistance2
Valida a restrição de distância 2: para vértices u, w com distância 2, |f(u) - f(w)| ≥ q

**Testes:**
- `CaminhoDistancia2`: Testa pares a distância 2 em caminho
- `CicloDistancia2`: Testa ciclo C_5
- `EstrelaSemDistancia2`: Testa folhas de estrela
- `GradeDistancia2`: Testa grade 3×3

### Suite 3: TestMinimization
Valida se a heurística produz valores razoáveis em instâncias conhecidas

**Testes:**
- `CaminhoValorOtimo`: Verifica bounds para caminho
- `CicloImparValorRazoavel`: Verifica ciclo ímpar
- `EstrelaBaixaCor`: Verifica eficiência em estrela
- `CompletoLimiteSuperior`: Verifica limites em grafo completo

### Suite 4: TestGreedyAlgorithms
Valida que os três algoritmos geram soluções válidas

**Testes:**
- `GulosoGeraValida`: Testa algoritmo guloso
- `RandomizadoGeraValida`: Testa algoritmo randomizado
- `ReativoGeraValida`: Testa algoritmo reativo
- `ComparacaoAlgoritmos`: Compara os três algoritmos

### Suite 5: TestRandomized
Testa com múltiplas sementes e avalia estabilidade

**Testes:**
- `MultiplasExecucoesSaoValidas`: 20 execuções com sementes diferentes
- `EstabilidadeRandomizado`: Verifica variação entre execuções

### Suite 6: TestInputs
Valida leitura de arquivos de instâncias

**Testes:**
- `LeituraCaminhoSimples`: Lê arquivo com caminho
- `LeituraCiclo`: Lê arquivo com ciclo
- `LeituraComComentarios`: Testa parsing com comentários
- `LeituraEColoracao`: Lê e colore imediatamente

### Suite 7: TestPerformance
Testes de desempenho e complexidade

**Testes:**
- `GrafoMedio500Vertices`: Testa grade 20×25 (500 vértices)
- `SemLoopInfinito`: Verifica terminação
- `ComplexidadeDistancia2`: Testa precomputação
- `RandomizadoGrafoMedio`: Testa randomizado em 100 vértices

### Suite 8: TestEdgeCases
Testes de casos extremos e edge cases

**Testes:**
- `GrafoVazio`: Um vértice isolado
- `DoisVerticesSemAresta`: Componentes desconexos
- `ValoresPGrandesCompletoK3`: p e q muito grandes
- `PEqualsQ`: Caso onde p = q
- `QMaiorQueP`: Caso onde q > p

## 🎯 Cobertura dos Requisitos

### ✅ 1. Corretude da restrição de distância 1
- Verificação explícita em todos os testes
- Testes específicos em `TestDistance1`
- Função `verificarColoracao()` valida todos os pares adjacentes

### ✅ 2. Corretude da restrição de distância 2
- Verificação explícita em `TestDistance2`
- Testa diferentes topologias (caminho, ciclo, estrela, grade)
- Valida precomputação de distância 2

### ✅ 3. Minimização da maior cor
- `TestMinimization` com valores conhecidos
- Testa caminho, ciclo, estrela, grafo completo
- Verifica bounds superiores e inferiores

### ✅ 4. Testar os três algoritmos
- `TestGreedyAlgorithms` valida todos os algoritmos
- Verifica que todos geram soluções válidas
- Compara resultados entre algoritmos

### ✅ 5. Testar com múltiplas sementes
- `TestRandomized.MultiplasExecucoesSaoValidas`: 20 execuções
- Calcula estatísticas: min, max, média, variação
- Verifica que nenhuma violação ocorre

### ✅ 6. Testes de leitura de instância
- `TestInputs` cria arquivos temporários
- Valida parsing de formato .col
- Testa comentários e casos válidos

### ✅ 7. Testar desempenho básico
- `TestPerformance` testa até 500 vértices
- Verifica ausência de loop infinito
- Mede complexidade de distância 2
- Timeout de 10 segundos

## 🔍 Funções Auxiliares

### `verificarColoracao(g, coloracao, p, q, verbose)`
Valida todas as restrições L(p,q):
- Tamanho da coloração
- Cores válidas (≥1)
- Restrição de distância 1
- Restrição de distância 2

### `calcularMaiorCor(coloracao)`
Retorna a maior cor usada na coloração

### Geradores de grafos:
- `criarCaminho(n)`: Caminho P_n
- `criarCiclo(n)`: Ciclo C_n
- `criarEstrela(n)`: Estrela S_n
- `criarCompleto(n)`: Grafo completo K_n
- `criarGrade(linhas, colunas)`: Grade 2D

## 📈 Saída Esperada

```
[==========] Running 35 tests from 8 test suites.
[----------] Global test environment set-up.
[----------] 4 tests from TestDistance1
[ RUN      ] TestDistance1.CaminhoSimples
[       OK ] TestDistance1.CaminhoSimples (0 ms)
...
[----------] 4 tests from TestDistance2
...
[  PASSED  ] 35 tests.
```

## 🐛 Debugging

Se algum teste falhar, use o modo verbose:
```bash
./tests --gtest_filter=NomeDoTeste* --gtest_verbose
```

Os testes imprimem informações detalhadas sobre violações:
```
ERRO: vertices adjacentes 2 e 3 violam p=2 (cores: 3, 4)
```

## 📝 Notas

- Os testes criam arquivos temporários em `test_instances/` (limpeza automática)
- Estatísticas detalhadas são impressas para testes de randomização
- Testes de performance medem tempo de execução
- Todos os testes são independentes e podem rodar em qualquer ordem

## 🤝 Contribuindo

Para adicionar novos testes:

1. Escolha a suite apropriada ou crie uma nova
2. Use as funções auxiliares fornecidas
3. Adicione verificações com `EXPECT_*` ou `ASSERT_*`
4. Documente o propósito do teste com comentários

## 📄 Licença

Este código de testes segue a mesma licença do projeto principal.
