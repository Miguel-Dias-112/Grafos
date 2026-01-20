#!/bin/bash
# ============================================================================
# L(p,q)-Coloring - Executar Experimentos para UMA Instancia
# DCC059 - Teoria dos Grafos
# ============================================================================
# Uso: ./scripts/run_instance.sh <instancia.col> [alphas]
# Exemplo: ./scripts/run_instance.sh instancias/huck.col 0.1,0.3,0.5
# ============================================================================

cd "$(dirname "$0")/.."

if [ -z "$1" ]; then
    echo "Erro: Instancia nao especificada!"
    echo "Uso: ./scripts/run_instance.sh <instancia.col> [alphas]"
    echo "Exemplo: ./scripts/run_instance.sh instancias/huck.col 0.1,0.3,0.5"
    exit 1
fi

INSTANCIA=$1
ALPHAS=${2:-"0.1,0.3,0.5"}
ALPHAS=$(echo $ALPHAS | tr ',' ' ')

# Configuracoes
NUM_EXEC=10
ITER_RAND=30
ITER_REATIVO=300
BLOCO=30

# Extrair nome da instancia
NOME=$(basename "$INSTANCIA" .col)

mkdir -p "logs/$NOME"

echo "============================================================================"
echo "L(p,q)-COLORING - Experimentos para: $NOME"
echo "============================================================================"
echo "Instancia: $INSTANCIA"
echo "Alphas: $ALPHAS"
echo "Execucoes: $NUM_EXEC"
echo "============================================================================"
echo ""

# GULOSO
echo "[1/5] Executando GULOSO (${NUM_EXEC}x)..."
for S in $(seq 1 $NUM_EXEC); do
    ./lpq_coloring "$INSTANCIA" 1 $S > "logs/$NOME/guloso_$S.log" 2>&1
done
echo "  Concluido!"

# RANDOMIZADO
echo "[2-4/5] Executando RANDOMIZADO..."
for A in $ALPHAS; do
    echo "  Alpha=$A (${NUM_EXEC}x com $ITER_RAND iteracoes)..."
    for S in $(seq 1 $NUM_EXEC); do
        ./lpq_coloring "$INSTANCIA" 2 $S $A $ITER_RAND > "logs/$NOME/rand_a${A}_$S.log" 2>&1
    done
done
echo "  Concluido!"

# REATIVO
echo "[5/5] Executando REATIVO (${NUM_EXEC}x com $ITER_REATIVO iteracoes)..."
for S in $(seq 1 $NUM_EXEC); do
    ./lpq_coloring "$INSTANCIA" 3 $S 0.3 $ITER_REATIVO $BLOCO > "logs/$NOME/reativo_$S.log" 2>&1
done
echo "  Concluido!"

echo ""
echo "============================================================================"
echo "Experimentos finalizados para: $NOME"
echo "Resultados em: resultados.csv"
echo "Logs em: logs/$NOME/"
echo ""
echo "Para analisar: python analyze_results.py resultados.csv $NOME"
echo "============================================================================"
