@echo off
echo Running L(p,q)-Coloring experiments...

REM Clear old results
del resultados.csv 2>nul

REM GULOSO (10 executions)
echo Running GULOSO...
for /L %%S in (1,1,10) do (
    lpq_coloring.exe instancias\test.col 1 %%S
)

REM RANDOMIZADO alpha=0.1 (10 executions)
echo Running RANDOMIZADO alpha=0.1...
for /L %%S in (1,1,10) do (
    lpq_coloring.exe instancias\test.col 2 %%S 0.1 30
)

REM RANDOMIZADO alpha=0.3 (10 executions)  
echo Running RANDOMIZADO alpha=0.3...
for /L %%S in (1,1,10) do (
    lpq_coloring.exe instancias\test.col 2 %%S 0.3 30
)

REM RANDOMIZADO alpha=0.5 (10 executions)
echo Running RANDOMIZADO alpha=0.5...
for /L %%S in (1,1,10) do (
    lpq_coloring.exe instancias\test.col 2 %%S 0.5 30
)

REM REATIVO (10 executions with 300 iterations)
echo Running REATIVO...
for /L %%S in (1,1,10) do (
    lpq_coloring.exe instancias\test.col 3 %%S 0.3 300 30
)

echo Done! Results saved to resultados.csv
