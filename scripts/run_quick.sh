#!/bin/bash
# ============================================================================
# L(p,q)-Coloring - Teste Rapido
# DCC059 - Teoria dos Grafos
# ============================================================================
# Executa um teste rapido para validar que tudo funciona.
# Uso: ./scripts/run_quick.sh
# ============================================================================

cd "$(dirname "$0")/.."

echo "============================================================================"
echo "L(p,q)-COLORING - Teste Rapido"
echo "============================================================================"
echo ""

# Verificar se o executavel existe
if [ ! -f "./lpq_coloring" ]; then
    echo "ERRO: lpq_coloring nao encontrado!"
    echo "Compile primeiro: g++ -std=c++17 -O2 -Wall main.cpp -o lpq_coloring"
    exit 1
fi

INST="instancias/huck.col"
if [ ! -f "$INST" ]; then
    echo "ERRO: $INST nao encontrado!"
    exit 1
fi

echo "Testando com: $INST"
echo ""

echo "[1/3] Testando GULOSO..."
./lpq_coloring "$INST" 1 1
echo ""

echo "[2/3] Testando RANDOMIZADO (alpha=0.3, 5 iteracoes)..."
./lpq_coloring "$INST" 2 1 0.3 5
echo ""

echo "[3/3] Testando REATIVO (10 iteracoes, bloco=5)..."
./lpq_coloring "$INST" 3 1 0.3 10 5
echo ""

echo "============================================================================"
echo "TESTE RAPIDO CONCLUIDO!"
echo "============================================================================"
echo "Se todos os testes mostraram 'Coloracao VALIDA', o programa esta funcionando."
echo ""
echo "Para executar experimentos completos:"
echo "  ./scripts/run_all.sh           - Todas as instancias"
echo "  ./scripts/run_instance.sh      - Uma instancia especifica"
echo "============================================================================"
