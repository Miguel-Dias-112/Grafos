@echo off
REM ============================================================================
REM L(p,q)-Coloring - Teste Rapido
REM DCC059 - Teoria dos Grafos
REM ============================================================================
REM Executa um teste rapido para validar que tudo funciona.
REM Uso: scripts\run_quick.bat
REM ============================================================================

setlocal
cd /d "%~dp0\.."

echo ============================================================================
echo L(p,q)-COLORING - Teste Rapido
echo ============================================================================
echo.

REM Verificar se o executavel existe
if not exist "lpq_coloring.exe" (
    echo ERRO: lpq_coloring.exe nao encontrado!
    echo Compile primeiro: g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring.exe
    exit /b 1
)

set INST=instancias\huck.col
if not exist "%INST%" (
    echo ERRO: %INST% nao encontrado!
    exit /b 1
)

echo Testando com: %INST%
echo.

echo [1/3] Testando GULOSO...
lpq_coloring.exe %INST% 1 1
echo.

echo [2/3] Testando RANDOMIZADO (alpha=0.3, 5 iteracoes)...
lpq_coloring.exe %INST% 2 1 0.3 5
echo.

echo [3/3] Testando REATIVO (10 iteracoes, bloco=5)...
lpq_coloring.exe %INST% 3 1 0.3 10 5
echo.

echo ============================================================================
echo TESTE RAPIDO CONCLUIDO!
echo ============================================================================
echo Se todos os testes mostraram "Coloracao VALIDA", o programa esta funcionando.
echo.
echo Para executar experimentos completos:
echo   scripts\run_all.bat           - Todas as instancias
echo   scripts\run_instance.bat      - Uma instancia especifica
echo ============================================================================

endlocal
pause
