@echo off
REM ============================================================================
REM L(p,q)-Coloring - Executar TODOS os Experimentos
REM DCC059 - Teoria dos Grafos
REM ============================================================================
REM Este script executa todos os experimentos para TODAS as instancias.
REM Uso: scripts\run_all.bat
REM ============================================================================

setlocal enabledelayedexpansion
cd /d "%~dp0\.."

echo ============================================================================
echo L(p,q)-COLORING - Experimentos Completos
echo ============================================================================
echo.

REM Configuracoes
set NUM_EXEC=10
set ITER_RAND=30
set ITER_REATIVO=300
set BLOCO=30
set ALPHAS=0.1 0.3 0.5

REM Limpar resultados anteriores (opcional - comentar se quiser manter)
REM del resultados.csv 2>nul

REM Criar diretorio de logs
if not exist "logs" mkdir logs

REM Lista de instancias
set INSTANCIAS=huck david anna homer mulsol.i.1 zeroin.i.1 le450_5a

echo Instancias a processar: %INSTANCIAS%
echo Execucoes por configuracao: %NUM_EXEC%
echo.

for %%I in (%INSTANCIAS%) do (
    echo ============================================================================
    echo Processando: %%I.col
    echo ============================================================================
    
    if not exist "logs\%%I" mkdir "logs\%%I"
    
    REM GULOSO
    echo [1/5] Guloso...
    for /L %%S in (1,1,%NUM_EXEC%) do (
        lpq_coloring.exe instancias\%%I.col 1 %%S > "logs\%%I\guloso_%%S.log" 2>&1
    )
    
    REM RANDOMIZADO com diferentes alphas
    set ALPHA_COUNT=2
    for %%A in (%ALPHAS%) do (
        echo [!ALPHA_COUNT!/5] Randomizado alpha=%%A...
        for /L %%S in (1,1,%NUM_EXEC%) do (
            lpq_coloring.exe instancias\%%I.col 2 %%S %%A %ITER_RAND% > "logs\%%I\rand_a%%A_%%S.log" 2>&1
        )
        set /a ALPHA_COUNT+=1
    )
    
    REM REATIVO
    echo [5/5] Reativo...
    for /L %%S in (1,1,%NUM_EXEC%) do (
        lpq_coloring.exe instancias\%%I.col 3 %%S 0.3 %ITER_REATIVO% %BLOCO% > "logs\%%I\reativo_%%S.log" 2>&1
    )
    
    echo Concluido: %%I.col
    echo.
)

echo ============================================================================
echo TODOS OS EXPERIMENTOS CONCLUIDOS!
echo ============================================================================
echo Resultados salvos em: resultados.csv
echo Logs salvos em: logs\
echo.
echo Para analisar os resultados:
echo   python analyze_results.py resultados.csv huck 11
echo   python analyze_results.py resultados.csv anna 11
echo   python analyze_results.py resultados.csv le450_5a 5
echo ============================================================================

endlocal
pause
