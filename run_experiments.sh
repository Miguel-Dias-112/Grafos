#!/bin/bash
# ============================================================================
# Script de Automação de Experimentos - L(p,q)-Coloring
# DCC059 - Teoria dos Grafos
# ============================================================================
# Uso: ./run_experiments.sh <instancia.col> [alpha1,alpha2,alpha3]
# Exemplo: ./run_experiments.sh instancias/test.col 0.1,0.3,0.5
# ============================================================================

set -e

# Verificar parâmetros
if [ -z "$1" ]; then
    echo "Erro: Instância não especificada!"
    echo "Uso: ./run_experiments.sh <instancia.col> [alpha1,alpha2,alpha3]"
    echo "Exemplo: ./run_experiments.sh instancias/test.col 0.1,0.3,0.5"
    exit 1
fi

INSTANCIA="$1"
ALPHAS="${2:-0.1,0.3,0.5}"

# Configurações do experimento
NUM_EXECUCOES=10
ITERACOES_RANDOMIZADO=30
ITERACOES_REATIVO=300
BLOCO_REATIVO=30

# Extrair nome base da instância
NOME_INSTANCIA=$(basename "$INSTANCIA" .col)

# Criar diretório de logs
mkdir -p "logs/${NOME_INSTANCIA}"

echo "============================================================================"
echo "L(p,q)-COLORING - Experimentos Automatizados"
echo "============================================================================"
echo "Instância: $INSTANCIA"
echo "Alphas: $ALPHAS"
echo "Execuções por configuração: $NUM_EXECUCOES"
echo "============================================================================"
echo

# ============================================================================
# ALGORITMO 1 - GULOSO
# ============================================================================
echo "[1/3] Executando GULOSO (${NUM_EXECUCOES}x)..."

for SEED in $(seq 1 $NUM_EXECUCOES); do
    echo "  Execução $SEED/$NUM_EXECUCOES..."
    ./lpq_coloring "$INSTANCIA" 1 $SEED > "logs/${NOME_INSTANCIA}/guloso_seed${SEED}.log" 2>&1
done
echo "  Concluído!"
echo

# ============================================================================
# ALGORITMO 2 - GULOSO RANDOMIZADO (para cada alpha)
# ============================================================================
echo "[2/3] Executando GULOSO RANDOMIZADO..."

IFS=',' read -ra ALPHA_ARRAY <<< "$ALPHAS"
for ALPHA in "${ALPHA_ARRAY[@]}"; do
    echo "  Alpha=$ALPHA (${NUM_EXECUCOES}x com $ITERACOES_RANDOMIZADO iterações)..."
    for SEED in $(seq 1 $NUM_EXECUCOES); do
        echo "    Execução $SEED/$NUM_EXECUCOES..."
        ./lpq_coloring "$INSTANCIA" 2 $SEED $ALPHA $ITERACOES_RANDOMIZADO > "logs/${NOME_INSTANCIA}/randomizado_a${ALPHA}_seed${SEED}.log" 2>&1
    done
done
echo "  Concluído!"
echo

# ============================================================================
# ALGORITMO 3 - GULOSO RANDOMIZADO REATIVO
# ============================================================================
echo "[3/3] Executando GULOSO RANDOMIZADO REATIVO..."

for SEED in $(seq 1 $NUM_EXECUCOES); do
    echo "  Execução $SEED/$NUM_EXECUCOES ($ITERACOES_REATIVO iterações, bloco=$BLOCO_REATIVO)..."
    ./lpq_coloring "$INSTANCIA" 3 $SEED 0.3 $ITERACOES_REATIVO $BLOCO_REATIVO > "logs/${NOME_INSTANCIA}/reativo_seed${SEED}.log" 2>&1
done
echo "  Concluído!"
echo

echo "============================================================================"
echo "Experimentos finalizados!"
echo "Resultados salvos em: resultados.csv"
echo "Logs salvos em: logs/${NOME_INSTANCIA}/"
echo "============================================================================"
echo
echo "Para analisar os resultados, execute:"
echo "  python3 analyze_results.py resultados.csv $NOME_INSTANCIA [melhor_conhecido]"
echo "============================================================================"
