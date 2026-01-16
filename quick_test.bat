@echo off
REM quick_test.bat - Script rápido para testar o programa
REM Este script testa os 3 algoritmos com um grafo simples

setlocal

echo ========================================
echo   L(p,q)-COLORING - TESTE RAPIDO
echo ========================================
echo.

REM Criar diretório de instâncias se não existir
if not exist instancias mkdir instancias

REM Criar arquivo de teste se não existir
if not exist instancias\test.col (
    echo Criando arquivo de teste...
    (
        echo p edge 5 4
        echo e 1 2
        echo e 2 3
        echo e 3 4
        echo e 4 5
    ) > instancias\test.col
)

REM Compilar programa principal
echo.
echo Compilando programa principal...
g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring.exe

if %ERRORLEVEL% neq 0 (
    echo Erro na compilacao!
    exit /b 1
)

echo Compilacao bem-sucedida!
echo.

REM Teste 1: Algoritmo Guloso
echo ========================================
echo TESTE 1: ALGORITMO GULOSO
echo ========================================
.\lpq_coloring.exe instancias\test.col 1

echo.
echo.

REM Teste 2: Algoritmo Randomizado
echo ========================================
echo TESTE 2: ALGORITMO RANDOMIZADO
echo ========================================
.\lpq_coloring.exe instancias\test.col 2 123 0.3 10

echo.
echo.

REM Teste 3: Algoritmo Reativo
echo ========================================
echo TESTE 3: ALGORITMO REATIVO
echo ========================================
.\lpq_coloring.exe instancias\test.col 3 123 0.3 20 10

echo.
echo ========================================
echo TESTES CONCLUIDOS!
echo ========================================
echo.
echo Arquivos gerados:
echo   - coloracao.txt (ultima coloracao)
echo   - resultados.csv (historico de resultados)
echo ========================================

endlocal
