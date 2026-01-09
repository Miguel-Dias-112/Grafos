@echo off
REM run_tests.bat - Script para compilar e executar testes no Windows
REM Uso: run_tests.bat [opcao]

setlocal

set TEST_SUITE=%1
if "%TEST_SUITE%"=="" set TEST_SUITE=all

echo ========================================
echo   L(p,q)-COLORING - SUITE DE TESTES
echo ========================================
echo.

echo Compilando testes...
g++ -std=c++17 -O2 -Wall tests.cpp -lgtest -lgtest_main -lpthread -o tests.exe

if %ERRORLEVEL% neq 0 (
    echo Erro na compilacao!
    echo Certifique-se de que o GoogleTest esta instalado.
    echo Veja TESTES_README.md para instrucoes de instalacao.
    exit /b 1
)

echo Compilacao bem-sucedida!
echo.

if /i "%TEST_SUITE%"=="all" (
    echo Executando TODOS os testes...
    tests.exe
    goto :end
)

if /i "%TEST_SUITE%"=="distance1" (
    echo Executando testes de Distancia 1...
    tests.exe --gtest_filter=TestDistance1.*
    goto :end
)

if /i "%TEST_SUITE%"=="distance2" (
    echo Executando testes de Distancia 2...
    tests.exe --gtest_filter=TestDistance2.*
    goto :end
)

if /i "%TEST_SUITE%"=="minimization" (
    echo Executando testes de Minimizacao...
    tests.exe --gtest_filter=TestMinimization.*
    goto :end
)

if /i "%TEST_SUITE%"=="algorithms" (
    echo Executando testes de Algoritmos...
    tests.exe --gtest_filter=TestGreedyAlgorithms.*
    goto :end
)

if /i "%TEST_SUITE%"=="randomized" (
    echo Executando testes Randomizados...
    tests.exe --gtest_filter=TestRandomized.*
    goto :end
)

if /i "%TEST_SUITE%"=="inputs" (
    echo Executando testes de Entrada...
    tests.exe --gtest_filter=TestInputs.*
    goto :end
)

if /i "%TEST_SUITE%"=="performance" (
    echo Executando testes de Performance...
    tests.exe --gtest_filter=TestPerformance.*
    goto :end
)

if /i "%TEST_SUITE%"=="edge" (
    echo Executando testes de Edge Cases...
    tests.exe --gtest_filter=TestEdgeCases.*
    goto :end
)

if /i "%TEST_SUITE%"=="list" (
    echo Listando todos os testes disponiveis...
    tests.exe --gtest_list_tests
    goto :end
)

echo Opcao invalida: %TEST_SUITE%
echo.
echo Opcoes disponiveis:
echo   all          - Executar todos os testes
echo   distance1    - Testes de restricao distancia 1
echo   distance2    - Testes de restricao distancia 2
echo   minimization - Testes de minimizacao
echo   algorithms   - Testes dos tres algoritmos
echo   randomized   - Testes com multiplas sementes
echo   inputs       - Testes de leitura de arquivo
echo   performance  - Testes de desempenho
echo   edge         - Testes de casos extremos
echo   list         - Listar todos os testes
exit /b 1

:end
echo.
echo ========================================
echo   FIM DOS TESTES
echo ========================================

endlocal
