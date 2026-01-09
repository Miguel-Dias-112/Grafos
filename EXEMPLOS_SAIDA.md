# Exemplos de Saída dos Testes (Documento escrito com ajuda de IA)

Este documento mostra exemplos de saída esperada ao executar os testes.

## Execução Completa com Sucesso

```
=========================================
  TESTES L(p,q)-COLORING - SUITE COMPLETA
=========================================

[==========] Running 35 tests from 8 test suites.
[----------] Global test environment set-up.
[----------] 4 tests from TestDistance1
[ RUN      ] TestDistance1.CaminhoSimples
[       OK ] TestDistance1.CaminhoSimples (1 ms)
[ RUN      ] TestDistance1.CicloOdd
[       OK ] TestDistance1.CicloOdd (0 ms)
[ RUN      ] TestDistance1.GrafoCompleto
[       OK ] TestDistance1.GrafoCompleto (0 ms)
[ RUN      ] TestDistance1.Estrela
[       OK ] TestDistance1.Estrela (0 ms)
[----------] 4 tests from TestDistance1 (1 ms total)

[----------] 4 tests from TestDistance2
[ RUN      ] TestDistance2.CaminhoDistancia2
[       OK ] TestDistance2.CaminhoDistancia2 (0 ms)
[ RUN      ] TestDistance2.CicloDistancia2
[       OK ] TestDistance2.CicloDistancia2 (0 ms)
[ RUN      ] TestDistance2.EstrelaSemDistancia2
[       OK ] TestDistance2.EstrelaSemDistancia2 (0 ms)
[ RUN      ] TestDistance2.GradeDistancia2
[       OK ] TestDistance2.GradeDistancia2 (1 ms)
[----------] 4 tests from TestDistance2 (1 ms total)

[----------] 4 tests from TestMinimization
[ RUN      ] TestMinimization.CaminhoValorOtimo
[       OK ] TestMinimization.CaminhoValorOtimo (0 ms)
[ RUN      ] TestMinimization.CicloImparValorRazoavel
[       OK ] TestMinimization.CicloImparValorRazoavel (0 ms)
[ RUN      ] TestMinimization.EstrelaBaixaCor
[       OK ] TestMinimization.EstrelaBaixaCor (0 ms)
[ RUN      ] TestMinimization.CompletoLimiteSuperior
[       OK ] TestMinimization.CompletoLimiteSuperior (0 ms)
[----------] 4 tests from TestMinimization (0 ms total)

[----------] 4 tests from TestGreedyAlgorithms
[ RUN      ] TestGreedyAlgorithms.GulosoGeraValida
[       OK ] TestGreedyAlgorithms.GulosoGeraValida (0 ms)
[ RUN      ] TestGreedyAlgorithms.RandomizadoGeraValida
[       OK ] TestGreedyAlgorithms.RandomizadoGeraValida (2 ms)
[ RUN      ] TestGreedyAlgorithms.ReativoGeraValida
[       OK ] TestGreedyAlgorithms.ReativoGeraValida (5 ms)
[ RUN      ] TestGreedyAlgorithms.ComparacaoAlgoritmos
[       OK ] TestGreedyAlgorithms.ComparacaoAlgoritmos (8 ms)
[----------] 4 tests from TestGreedyAlgorithms (15 ms total)

[----------] 2 tests from TestRandomized
[ RUN      ] TestRandomized.MultiplasExecucoesSaoValidas

--- Estatisticas de 20 execucoes ---
Menor cor: 4
Maior cor: 5
Media: 4.45
Variacao: 1
[       OK ] TestRandomized.MultiplasExecucoesSaoValidas (12 ms)
[ RUN      ] TestRandomized.EstabilidadeRandomizado
[       OK ] TestRandomized.EstabilidadeRandomizado (15 ms)
[----------] 2 tests from TestRandomized (27 ms total)

[----------] 4 tests from TestInputs
[ RUN      ] TestInputs.LeituraCaminhoSimples
[       OK ] TestInputs.LeituraCaminhoSimples (1 ms)
[ RUN      ] TestInputs.LeituraCiclo
[       OK ] TestInputs.LeituraCiclo (0 ms)
[ RUN      ] TestInputs.LeituraComComentarios
[       OK ] TestInputs.LeituraComComentarios (0 ms)
[ RUN      ] TestInputs.LeituraEColoracao
[       OK ] TestInputs.LeituraEColoracao (0 ms)
[----------] 4 tests from TestInputs (1 ms total)

[----------] 5 tests from TestPerformance
[ RUN      ] TestPerformance.GrafoMedio500Vertices

--- Desempenho para 500 vertices ---
Tempo: 0.0234 segundos
Maior cor: 7
[       OK ] TestPerformance.GrafoMedio500Vertices (25 ms)
[ RUN      ] TestPerformance.SemLoopInfinito
[       OK ] TestPerformance.SemLoopInfinito (1 ms)
[ RUN      ] TestPerformance.ComplexidadeDistancia2

--- Precomputacao de distancia 2 ---
Vertices: 225
Total pares dist-2: 580
Tempo: 0.0045 segundos
[       OK ] TestPerformance.ComplexidadeDistancia2 (5 ms)
[ RUN      ] TestPerformance.RandomizadoGrafoMedio

--- Randomizado em grafo medio ---
Tempo: 0.1523 segundos
[       OK ] TestPerformance.RandomizadoGrafoMedio (153 ms)
[----------] 5 tests from TestPerformance (184 ms total)

[----------] 5 tests from TestEdgeCases
[ RUN      ] TestEdgeCases.GrafoVazio
[       OK ] TestEdgeCases.GrafoVazio (0 ms)
[ RUN      ] TestEdgeCases.DoisVerticesSemAresta
[       OK ] TestEdgeCases.DoisVerticesSemAresta (0 ms)
[ RUN      ] TestEdgeCases.ValoresPGrandesCompletoK3
[       OK ] TestEdgeCases.ValoresPGrandesCompletoK3 (0 ms)
[ RUN      ] TestEdgeCases.PEqualsQ
[       OK ] TestEdgeCases.PEqualsQ (0 ms)
[ RUN      ] TestEdgeCases.QMaiorQueP
[       OK ] TestEdgeCases.QMaiorQueP (0 ms)
[----------] 5 tests from TestEdgeCases (0 ms total)

[----------] Global test environment tear-down
[==========] 35 tests from 8 test suites ran. (230 ms total)
[  PASSED  ] 35 tests.

=========================================
  FIM DOS TESTES
=========================================
```

## Exemplo de Teste Falhando

Quando um teste falha, você verá saída detalhada:

```
[ RUN      ] TestDistance1.CaminhoSimples
tests.cpp:456: Failure
Expected: (abs(coloracao[u] - coloracao[v])) >= (p), actual: 1 vs 2
Vertices adjacentes 2 e 3 violam restricao de distancia 1 (p=2)
[  FAILED  ] TestDistance1.CaminhoSimples (1 ms)
```

## Execução de Suite Específica

```powershell
> .\run_tests.ps1 distance1

========================================
  L(p,q)-COLORING - SUITE DE TESTES
========================================

Compilando testes...
Compilação bem-sucedida!

Executando testes de Distância 1...
[==========] Running 4 tests from 1 test suite.
[----------] 4 tests from TestDistance1
[ RUN      ] TestDistance1.CaminhoSimples
[       OK ] TestDistance1.CaminhoSimples (0 ms)
[ RUN      ] TestDistance1.CicloOdd
[       OK ] TestDistance1.CicloOdd (0 ms)
[ RUN      ] TestDistance1.GrafoCompleto
[       OK ] TestDistance1.GrafoCompleto (0 ms)
[ RUN      ] TestDistance1.Estrela
[       OK ] TestDistance1.Estrela (0 ms)
[----------] 4 tests from TestDistance1 (0 ms total)

[==========] 4 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 4 tests.

========================================
  FIM DOS TESTES
========================================
```

## Listando Testes Disponíveis

```bash
> ./tests --gtest_list_tests

TestDistance1.
  CaminhoSimples
  CicloOdd
  GrafoCompleto
  Estrela
TestDistance2.
  CaminhoDistancia2
  CicloDistancia2
  EstrelaSemDistancia2
  GradeDistancia2
TestMinimization.
  CaminhoValorOtimo
  CicloImparValorRazoavel
  EstrelaBaixaCor
  CompletoLimiteSuperior
TestGreedyAlgorithms.
  GulosoGeraValida
  RandomizadoGeraValida
  ReativoGeraValida
  ComparacaoAlgoritmos
TestRandomized.
  MultiplasExecucoesSaoValidas
  EstabilidadeRandomizado
TestInputs.
  LeituraCaminhoSimples
  LeituraCiclo
  LeituraComComentarios
  LeituraEColoracao
TestPerformance.
  GrafoMedio500Vertices
  SemLoopInfinito
  ComplexidadeDistancia2
  RandomizadoGrafoMedio
TestEdgeCases.
  GrafoVazio
  DoisVerticesSemAresta
  ValoresPGrandesCompletoK3
  PEqualsQ
  QMaiorQueP
```

## Executar Teste Específico

```bash
> ./tests --gtest_filter=TestRandomized.MultiplasExecucoesSaoValidas

[==========] Running 1 test from 1 test suite.
[----------] 1 test from TestRandomized
[ RUN      ] TestRandomized.MultiplasExecucoesSaoValidas

--- Estatisticas de 20 execucoes ---
Menor cor: 4
Maior cor: 5
Media: 4.45
Variacao: 1
[       OK ] TestRandomized.MultiplasExecucoesSaoValidas (12 ms)
[----------] 1 test from TestRandomized (12 ms total)

[==========] 1 test from 1 test suite ran. (12 ms total)
[  PASSED  ] 1 test.
```

## Execução com Repetição

```bash
> ./tests --gtest_filter=TestRandomized.* --gtest_repeat=3

Repeating all tests (iteration 1) . . .

[==========] Running 2 tests from 1 test suite.
...
[  PASSED  ] 2 tests.

Repeating all tests (iteration 2) . . .

[==========] Running 2 tests from 1 test suite.
...
[  PASSED  ] 2 tests.

Repeating all tests (iteration 3) . . .

[==========] Running 2 tests from 1 test suite.
...
[  PASSED  ] 2 tests.
```

## Estatísticas Detalhadas dos Testes de Performance

```
[ RUN      ] TestPerformance.GrafoMedio500Vertices

--- Desempenho para 500 vertices ---
Tempo: 0.0234 segundos
Maior cor: 7
[       OK ] TestPerformance.GrafoMedio500Vertices (25 ms)

[ RUN      ] TestPerformance.ComplexidadeDistancia2

--- Precomputacao de distancia 2 ---
Vertices: 225
Total pares dist-2: 580
Tempo: 0.0045 segundos
[       OK ] TestPerformance.ComplexidadeDistancia2 (5 ms)

[ RUN      ] TestPerformance.RandomizadoGrafoMedio

--- Randomizado em grafo medio ---
Tempo: 0.1523 segundos
[       OK ] TestPerformance.RandomizadoGrafoMedio (153 ms)
```

## Estatísticas dos Testes Randomizados

```
[ RUN      ] TestRandomized.MultiplasExecucoesSaoValidas

--- Estatisticas de 20 execucoes ---
Menor cor: 4
Maior cor: 5
Media: 4.45
Variacao: 1
[       OK ] TestRandomized.MultiplasExecucoesSaoValidas (12 ms)
```

## Verificação com Modo Verbose

Embora o GoogleTest não tenha um modo `--gtest_verbose` oficial, você pode usar `--gtest_print_time=1` para ver tempos detalhados:

```bash
> ./tests --gtest_filter=TestDistance1.* --gtest_print_time=1

[==========] Running 4 tests from 1 test suite.
[----------] 4 tests from TestDistance1
[ RUN      ] TestDistance1.CaminhoSimples
[       OK ] TestDistance1.CaminhoSimples (0 ms)
[ RUN      ] TestDistance1.CicloOdd
[       OK ] TestDistance1.CicloOdd (0 ms)
[ RUN      ] TestDistance1.GrafoCompleto
[       OK ] TestDistance1.GrafoCompleto (0 ms)
[ RUN      ] TestDistance1.Estrela
[       OK ] TestDistance1.Estrela (1 ms)
[----------] 4 tests from TestDistance1 (1 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test suite ran. (1 ms total)
[  PASSED  ] 4 tests.
```

## Resumo de Código de Saída

- **Retorno 0**: Todos os testes passaram ✅
- **Retorno 1**: Um ou mais testes falharam ❌

Você pode usar isso em scripts de CI/CD:

```bash
./tests
if [ $? -eq 0 ]; then
    echo "Todos os testes passaram!"
else
    echo "Alguns testes falharam!"
    exit 1
fi
```

## Usando com Makefile

```bash
# Compilar e executar todos os testes
> make test

# Executar suite específica
> make test-distance1
> make test-randomized
> make test-performance

# Listar testes
> make list-tests
```

## Integração com CI/CD

Os testes podem ser integrados com sistemas de CI/CD:

```yaml
# Exemplo para GitHub Actions
- name: Build and Test
  run: |
    make tests
    ./tests --gtest_output=xml:test_results.xml
```

Isso gera um relatório XML que pode ser processado por ferramentas de CI.
