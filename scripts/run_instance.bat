@echo off
REM ============================================================================
REM L(p,q)-Coloring - Executar Experimentos para UMA Instancia
REM DCC059 - Teoria dos Grafos
REM ============================================================================
REM Uso: scripts\run_instance.bat <instancia.col> [alpha1,alpha2,alpha3]
REM Exemplo: scripts\run_instance.bat instancias\huck.col 0.1,0.3,0.5
REM ============================================================================

setlocal enabledelayedexpansion
cd /d "%~dp0\.."

if "%~1"=="" (
    echo Erro: Instancia nao especificada!
    echo Uso: scripts\run_instance.bat ^<instancia.col^> [alphas]
    echo Exemplo: scripts\run_instance.bat instancias\huck.col 0.1,0.3,0.5
    exit /b 1
)

set INSTANCIA=%~1
set ALPHAS=%~2
if "%ALPHAS%"=="" set ALPHAS=0.1,0.3,0.5

REM Configuracoes
set NUM_EXEC=10
set ITER_RAND=30
set ITER_REATIVO=300
set BLOCO=30

REM Extrair nome da instancia
for %%F in ("%INSTANCIA%") do set NOME=%%~nF

if not exist "logs\%NOME%" mkdir "logs\%NOME%"

echo ============================================================================
echo L(p,q)-COLORING - Experimentos para: %NOME%
echo ============================================================================
echo Instancia: %INSTANCIA%
echo Alphas: %ALPHAS%
echo Execucoes: %NUM_EXEC%
echo ============================================================================
echo.

REM GULOSO
echo [1/5] Executando GULOSO (%NUM_EXEC%x)...
for /L %%S in (1,1,%NUM_EXEC%) do (
    lpq_coloring.exe %INSTANCIA% 1 %%S > "logs\%NOME%\guloso_%%S.log" 2>&1
)
echo   Concluido!

REM RANDOMIZADO
echo [2-4/5] Executando RANDOMIZADO...
for %%A in (%ALPHAS%) do (
    echo   Alpha=%%A (%NUM_EXEC%x com %ITER_RAND% iteracoes)...
    for /L %%S in (1,1,%NUM_EXEC%) do (
        lpq_coloring.exe %INSTANCIA% 2 %%S %%A %ITER_RAND% > "logs\%NOME%\rand_a%%A_%%S.log" 2>&1
    )
)
echo   Concluido!

REM REATIVO
echo [5/5] Executando REATIVO (%NUM_EXEC%x com %ITER_REATIVO% iteracoes)...
for /L %%S in (1,1,%NUM_EXEC%) do (
    lpq_coloring.exe %INSTANCIA% 3 %%S 0.3 %ITER_REATIVO% %BLOCO% > "logs\%NOME%\reativo_%%S.log" 2>&1
)
echo   Concluido!

echo.
echo ============================================================================
echo Experimentos finalizados para: %NOME%
echo Resultados em: resultados.csv
echo Logs em: logs\%NOME%\
echo.
echo Para analisar: python analyze_results.py resultados.csv %NOME%
echo ============================================================================

endlocal
