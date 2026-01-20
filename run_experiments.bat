@echo off
REM ============================================================================
REM Script de Automação de Experimentos - L(p,q)-Coloring
REM DCC059 - Teoria dos Grafos
REM ============================================================================
REM Uso: run_experiments.bat <instancia.col> [alpha1,alpha2,alpha3]
REM Exemplo: run_experiments.bat instancias\test.col 0.1,0.3,0.5
REM ============================================================================

setlocal enabledelayedexpansion

REM Verificar parâmetros
if "%~1"=="" (
    echo Erro: Instancia nao especificada!
    echo Uso: run_experiments.bat ^<instancia.col^> [alpha1,alpha2,alpha3]
    echo Exemplo: run_experiments.bat instancias\test.col 0.1,0.3,0.5
    exit /b 1
)

set INSTANCIA=%~1
set ALPHAS=%~2
if "%ALPHAS%"=="" set ALPHAS=0.1,0.3,0.5

REM Configurações do experimento
set NUM_EXECUCOES=10
set ITERACOES_RANDOMIZADO=30
set ITERACOES_REATIVO=300
set BLOCO_REATIVO=30

REM Extrair nome base da instância para logs
for %%F in ("%INSTANCIA%") do set NOME_INSTANCIA=%%~nF

REM Criar diretório de logs se não existir
if not exist "logs\%NOME_INSTANCIA%" mkdir "logs\%NOME_INSTANCIA%"

echo ============================================================================
echo L(p,q)-COLORING - Experimentos Automatizados
echo ============================================================================
echo Instancia: %INSTANCIA%
echo Alphas: %ALPHAS%
echo Execucoes por configuracao: %NUM_EXECUCOES%
echo ============================================================================
echo.

REM ============================================================================
REM ALGORITMO 1 - GULOSO
REM ============================================================================
echo [1/3] Executando GULOSO (%NUM_EXECUCOES%x)...

for /L %%S in (1,1,%NUM_EXECUCOES%) do (
    echo   Execucao %%S/%NUM_EXECUCOES%...
    lpq_coloring.exe %INSTANCIA% 1 %%S > "logs\%NOME_INSTANCIA%\guloso_seed%%S.log" 2>&1
)
echo   Concluido!
echo.

REM ============================================================================
REM ALGORITMO 2 - GULOSO RANDOMIZADO (para cada alpha)
REM ============================================================================
echo [2/3] Executando GULOSO RANDOMIZADO...

for %%A in (%ALPHAS%) do (
    echo   Alpha=%%A (%NUM_EXECUCOES%x com %ITERACOES_RANDOMIZADO% iteracoes)...
    for /L %%S in (1,1,%NUM_EXECUCOES%) do (
        echo     Execucao %%S/%NUM_EXECUCOES%...
        lpq_coloring.exe %INSTANCIA% 2 %%S %%A %ITERACOES_RANDOMIZADO% > "logs\%NOME_INSTANCIA%\randomizado_a%%A_seed%%S.log" 2>&1
    )
)
echo   Concluido!
echo.

REM ============================================================================
REM ALGORITMO 3 - GULOSO RANDOMIZADO REATIVO
REM ============================================================================
echo [3/3] Executando GULOSO RANDOMIZADO REATIVO...

for /L %%S in (1,1,%NUM_EXECUCOES%) do (
    echo   Execucao %%S/%NUM_EXECUCOES% (%ITERACOES_REATIVO% iteracoes, bloco=%BLOCO_REATIVO%)...
    lpq_coloring.exe %INSTANCIA% 3 %%S 0.3 %ITERACOES_REATIVO% %BLOCO_REATIVO% > "logs\%NOME_INSTANCIA%\reativo_seed%%S.log" 2>&1
)
echo   Concluido!
echo.

echo ============================================================================
echo Experimentos finalizados!
echo Resultados salvos em: resultados.csv
echo Logs salvos em: logs\%NOME_INSTANCIA%\
echo ============================================================================
echo.
echo Para analisar os resultados, execute:
echo   python analyze_results.py resultados.csv %NOME_INSTANCIA% [melhor_conhecido]
echo ============================================================================

endlocal
