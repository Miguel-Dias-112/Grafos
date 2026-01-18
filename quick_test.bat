@echo off
setlocal enabledelayedexpansion

REM ================================
REM Automacao de Testes L(p,q)-Coloring
REM ================================

set EXEC=lpq_coloring.exe
set INST_DIR=instancias
set LOG_DIR=logs

set NUM_EXEC=10
set SEED_BASE=1000

REM Parametros dos algoritmos
set ALPHA=0.3
set ITER_RANDOM=30
set ITER_REACTIVE=100
set BLOCO=20

REM Criar pasta de logs
if not exist %LOG_DIR% mkdir %LOG_DIR%

echo ================================
echo COMPILANDO PROJETO
echo ================================

make
if errorlevel 1 (
    echo ERRO NA COMPILACAO
    pause
    exit /b
)

echo Compilacao OK
echo.

REM Loop por instancias
for %%F in (%INST_DIR%\*.col) do (

    set INST=%%F
    set NAME=%%~nF

    echo ====================================
    echo Instancia: !NAME!
    echo ====================================

    REM Algoritmo Guloso
    for /L %%i in (1,1,%NUM_EXEC%) do (
        set /A SEED=%SEED_BASE%+%%i
        set LOG_PATH=%LOG_DIR%\!NAME!\guloso
        if not exist !LOG_PATH! mkdir !LOG_PATH!

        echo [GULOSO] Execucao %%i
        %EXEC% !INST! 1 > !LOG_PATH!\run_%%i.txt 2>&1
    )

    REM Algoritmo Randomizado
    for /L %%i in (1,1,%NUM_EXEC%) do (
        set /A SEED=%SEED_BASE%+%%i
        set LOG_PATH=%LOG_DIR%\!NAME!\randomizado
        if not exist !LOG_PATH! mkdir !LOG_PATH!

        echo [RANDOMIZADO] Execucao %%i (seed=!SEED!)
        %EXEC% !INST! 2 !SEED! %ALPHA% %ITER_RANDOM% > !LOG_PATH!\run_%%i.txt 2>&1
    )

    REM Algoritmo Reativo
    for /L %%i in (1,1,%NUM_EXEC%) do (
        set /A SEED=%SEED_BASE%+%%i
        set LOG_PATH=%LOG_DIR%\!NAME!\reativo
        if not exist !LOG_PATH! mkdir !LOG_PATH!

        echo [REATIVO] Execucao %%i (seed=!SEED!)
        %EXEC% !INST! 3 !SEED! %ALPHA% %ITER_REACTIVE% %BLOCO% > !LOG_PATH!\run_%%i.txt 2>&1
    )
)

echo.
echo ================================
echo TESTES FINALIZADOS
echo Logs gerados em: %LOG_DIR%
echo ================================

pause
