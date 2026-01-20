#!/bin/bash
# ============================================================================
# L(p,q)-Coloring - Executar TODOS os Experimentos
# DCC059 - Teoria dos Grafos
# ============================================================================
# Este script executa todos os experimentos para TODAS as instancias.
# Uso: ./scripts/run_all.sh
# ============================================================================

cd "$(dirname "$0")/.."

echo "============================================================================"
echo "L(p,q)-COLORING - Experimentos Completos"
echo "============================================================================"
echo ""

# Configuracoes
NUM_EXEC=10
ITER_RAND=30
ITER_REATIVO=300
BLOCO=30
ALPHAS="0.1 0.3 0.5"

# Criar diretorio de logs
mkdir -p logs

# Lista de instancias
INSTANCIAS="huck david anna homer mulsol.i.1 zeroin.i.1 le450_5a"

echo "Instancias a processar: $INSTANCIAS"
echo "Execucoes por configuracao: $NUM_EXEC"
echo ""

for INST in $INSTANCIAS; do
    echo "============================================================================"
    echo "Processando: $INST.col"
    echo "============================================================================"
    
    mkdir -p "logs/$INST"
    
    # GULOSO
    echo "[1/5] Guloso..."
    for S in $(seq 1 $NUM_EXEC); do
        ./lpq_coloring "instancias/$INST.col" 1 $S > "logs/$INST/guloso_$S.log" 2>&1
    done
    
    # RANDOMIZADO com diferentes alphas
    COUNT=2
    for A in $ALPHAS; do
        echo "[$COUNT/5] Randomizado alpha=$A..."
        for S in $(seq 1 $NUM_EXEC); do
            ./lpq_coloring "instancias/$INST.col" 2 $S $A $ITER_RAND > "logs/$INST/rand_a${A}_$S.log" 2>&1
        done
        COUNT=$((COUNT + 1))
    done
    
    # REATIVO
    echo "[5/5] Reativo..."
    for S in $(seq 1 $NUM_EXEC); do
        ./lpq_coloring "instancias/$INST.col" 3 $S 0.3 $ITER_REATIVO $BLOCO > "logs/$INST/reativo_$S.log" 2>&1
    done
    
    echo "Concluido: $INST.col"
    echo ""
done

echo "============================================================================"
echo "TODOS OS EXPERIMENTOS CONCLUIDOS!"
echo "============================================================================"
echo "Resultados salvos em: resultados.csv"
echo "Logs salvos em: logs/"
echo ""
echo "Para analisar os resultados:"
echo "  python analyze_results.py resultados.csv huck 11"
echo "  python analyze_results.py resultados.csv anna 11"
echo "  python analyze_results.py resultados.csv le450_5a 5"
echo "============================================================================"
