#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Análise de Resultados - L(p,q)-Coloring
DCC059 - Teoria dos Grafos

Este script analisa os resultados dos experimentos e gera tabelas formatadas
para o relatório, incluindo cálculo de desvios percentuais.

Uso:
    python analyze_results.py <arquivo_csv> <instancia> [melhor_conhecido]

Exemplo:
    python analyze_results.py resultados.csv test 5
    python analyze_results.py resultados.csv queen5_5 8
"""

import sys
import csv
from collections import defaultdict
from typing import Dict, List, Tuple

# Valores conhecidos (número cromático) para as instâncias DIMACS
KNOWN_VALUES = {
    'test': 5,
    'huck': 11,
    'david': 11,
    'anna': 11,
    'homer': 13,
    'mulsol.i.1': 49,
    'mulsol.i.2': 31,
    'mulsol.i.3': 31,
    'mulsol.i.4': 31,
    'mulsol.i.5': 31,
    'zeroin.i.1': 49,
    'zeroin.i.2': 30,
    'zeroin.i.3': 30,
    'fpsol2.i.1': 65,
    'fpsol2.i.2': 30,
    'fpsol2.i.3': 30,
    'inithx.i.1': 54,
    'inithx.i.2': 31,
    'inithx.i.3': 31,
    'le450_5a': 5,
    'le450_5b': 5,
    'le450_5c': 5,
    'le450_5d': 5,
    'le450_15a': 15,
    'le450_15b': 15,
    'le450_15c': 15,
    'le450_15d': 15,
    'le450_25a': 25,
    'le450_25b': 25,
    'le450_25c': 25,
    'le450_25d': 25,
}


def load_results(csv_file: str, instance_filter: str = None) -> List[dict]:
    """Carrega resultados do arquivo CSV."""
    results = []
    with open(csv_file, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            if instance_filter is None or instance_filter in row.get('instancia', ''):
                results.append(row)
    return results


def group_by_algorithm(results: List[dict]) -> Dict[str, List[dict]]:
    """Agrupa resultados por algoritmo (e alpha para randomizado)."""
    groups = defaultdict(list)
    
    for row in results:
        algo = row.get('algoritmo', '')
        alpha = row.get('alpha', '')
        
        if algo == 'GULOSO':
            key = 'GULOSO'
        elif algo == 'RANDOMIZADO':
            key = f'RANDOMIZADO (α={alpha})'
        elif algo == 'REATIVO':
            key = 'REATIVO'
        else:
            key = algo
        
        groups[key].append(row)
    
    return groups


def calculate_statistics(results: List[dict]) -> dict:
    """Calcula estatísticas para um grupo de resultados."""
    if not results:
        return None
    
    # Extrair valores numéricos
    melhores_cores = []
    tempos = []
    medias_iter = []
    
    for row in results:
        try:
            melhor_cor = int(row.get('melhor_cor', 0))
            tempo = float(row.get('tempo', 0))
            media_cor = float(row.get('media_cor', melhor_cor))
            
            melhores_cores.append(melhor_cor)
            tempos.append(tempo)
            medias_iter.append(media_cor)
        except (ValueError, TypeError):
            continue
    
    if not melhores_cores:
        return None
    
    # Calcular estatísticas
    return {
        'melhor_solucao': min(melhores_cores),
        'pior_solucao': max(melhores_cores),
        'media_solucoes': sum(melhores_cores) / len(melhores_cores),
        'tempo_medio': sum(tempos) / len(tempos),
        'tempo_total': sum(tempos),
        'num_execucoes': len(melhores_cores),
        'todas_solucoes': melhores_cores,
    }


def calculate_deviations(stats: dict, best_known: int) -> dict:
    """Calcula desvios percentuais em relação ao melhor conhecido."""
    if stats is None or best_known <= 0:
        return None
    
    # Desvio da melhor solução encontrada
    melhor = stats['melhor_solucao']
    desvio_melhor = ((melhor - best_known) / best_known) * 100
    
    # Desvio da média das soluções
    media = stats['media_solucoes']
    desvio_media = ((media - best_known) / best_known) * 100
    
    return {
        'desvio_melhor': desvio_melhor,
        'desvio_media': desvio_media,
        'melhor_conhecido': best_known,
    }


def print_summary_table(groups: Dict[str, dict], best_known: int = None):
    """Imprime tabela resumo dos experimentos."""
    print("\n" + "=" * 80)
    print("TABELA RESUMO DOS EXPERIMENTOS")
    print("=" * 80)
    
    # Cabeçalho
    if best_known:
        print(f"Melhor solução conhecida da literatura: {best_known}")
    print()
    
    header = f"{'Algoritmo':<30} {'Melhor':>8} {'Média':>8} {'Pior':>8} {'Tempo(s)':>10} {'Exec':>5}"
    if best_known:
        header += f" {'Desv.Mel%':>10} {'Desv.Méd%':>10}"
    
    print(header)
    print("-" * len(header))
    
    # Dados
    for algo, results in sorted(groups.items()):
        stats = calculate_statistics(results)
        if stats is None:
            continue
        
        row = f"{algo:<30} {stats['melhor_solucao']:>8} {stats['media_solucoes']:>8.2f} {stats['pior_solucao']:>8} {stats['tempo_medio']:>10.4f} {stats['num_execucoes']:>5}"
        
        if best_known:
            devs = calculate_deviations(stats, best_known)
            row += f" {devs['desvio_melhor']:>10.2f} {devs['desvio_media']:>10.2f}"
        
        print(row)
    
    print("=" * 80)


def generate_latex_table(groups: Dict[str, dict], instance_name: str, best_known: int = None) -> str:
    """Gera tabela formatada em LaTeX."""
    
    latex = []
    latex.append(r"% Tabela gerada automaticamente por analyze_results.py")
    latex.append(r"% L(p,q)-Coloring - DCC059")
    latex.append(r"")
    
    # Determinar número de colunas
    if best_known:
        cols = "l|r|r|r|r|r|r"
        header = r"Algoritmo & Melhor & Média & Pior & Tempo (s) & Desv. Melhor (\%) & Desv. Média (\%) \\"
    else:
        cols = "l|r|r|r|r"
        header = r"Algoritmo & Melhor & Média & Pior & Tempo (s) \\"
    
    latex.append(r"\begin{table}[htbp]")
    latex.append(r"\centering")
    latex.append(f"\\caption{{Resultados para instância {instance_name}}}")
    latex.append(f"\\label{{tab:{instance_name}}}")
    latex.append(f"\\begin{{tabular}}{{{cols}}}")
    latex.append(r"\hline")
    latex.append(header)
    latex.append(r"\hline")
    
    # Dados
    for algo, results in sorted(groups.items()):
        stats = calculate_statistics(results)
        if stats is None:
            continue
        
        # Escapar caracteres especiais do LaTeX
        algo_escaped = algo.replace('α', r'$\alpha$').replace('_', r'\_')
        
        if best_known:
            devs = calculate_deviations(stats, best_known)
            row = f"{algo_escaped} & {stats['melhor_solucao']} & {stats['media_solucoes']:.2f} & {stats['pior_solucao']} & {stats['tempo_medio']:.4f} & {devs['desvio_melhor']:.2f} & {devs['desvio_media']:.2f} \\\\"
        else:
            row = f"{algo_escaped} & {stats['melhor_solucao']} & {stats['media_solucoes']:.2f} & {stats['pior_solucao']} & {stats['tempo_medio']:.4f} \\\\"
        
        latex.append(row)
    
    latex.append(r"\hline")
    latex.append(r"\end{tabular}")
    
    if best_known:
        latex.append(f"\\footnotesize{{Melhor valor conhecido: {best_known}}}")
    
    latex.append(r"\end{table}")
    
    return "\n".join(latex)


def generate_individual_tables(groups: Dict[str, dict], instance_name: str, best_known: int = None) -> str:
    """Gera tabelas individuais conforme especificação do trabalho."""
    
    tables = []
    
    # Tabela 1: Desvio da Melhor Solução
    tables.append(r"% ============================================================================")
    tables.append(r"% TABELA 1: Desvio percentual da MELHOR solução encontrada")
    tables.append(r"% ============================================================================")
    tables.append(r"\begin{table}[htbp]")
    tables.append(r"\centering")
    tables.append(f"\\caption{{Desvio da melhor solução - {instance_name}}}")
    tables.append(r"\begin{tabular}{l|r|r}")
    tables.append(r"\hline")
    tables.append(r"Algoritmo & Melhor Solução & Desvio (\%) \\")
    tables.append(r"\hline")
    
    for algo, results in sorted(groups.items()):
        stats = calculate_statistics(results)
        if stats:
            algo_escaped = algo.replace('α', r'$\alpha$').replace('_', r'\_')
            if best_known:
                devs = calculate_deviations(stats, best_known)
                tables.append(f"{algo_escaped} & {stats['melhor_solucao']} & {devs['desvio_melhor']:.2f} \\\\")
            else:
                tables.append(f"{algo_escaped} & {stats['melhor_solucao']} & -- \\\\")
    
    tables.append(r"\hline")
    tables.append(r"\end{tabular}")
    tables.append(r"\end{table}")
    tables.append(r"")
    
    # Tabela 2: Desvio da Média
    tables.append(r"% ============================================================================")
    tables.append(r"% TABELA 2: Desvio percentual da MÉDIA das soluções")
    tables.append(r"% ============================================================================")
    tables.append(r"\begin{table}[htbp]")
    tables.append(r"\centering")
    tables.append(f"\\caption{{Desvio da média - {instance_name}}}")
    tables.append(r"\begin{tabular}{l|r|r}")
    tables.append(r"\hline")
    tables.append(r"Algoritmo & Média & Desvio (\%) \\")
    tables.append(r"\hline")
    
    for algo, results in sorted(groups.items()):
        stats = calculate_statistics(results)
        if stats:
            algo_escaped = algo.replace('α', r'$\alpha$').replace('_', r'\_')
            if best_known:
                devs = calculate_deviations(stats, best_known)
                tables.append(f"{algo_escaped} & {stats['media_solucoes']:.2f} & {devs['desvio_media']:.2f} \\\\")
            else:
                tables.append(f"{algo_escaped} & {stats['media_solucoes']:.2f} & -- \\\\")
    
    tables.append(r"\hline")
    tables.append(r"\end{tabular}")
    tables.append(r"\end{table}")
    tables.append(r"")
    
    # Tabela 3: Tempo Médio
    tables.append(r"% ============================================================================")
    tables.append(r"% TABELA 3: Tempo médio de execução")
    tables.append(r"% ============================================================================")
    tables.append(r"\begin{table}[htbp]")
    tables.append(r"\centering")
    tables.append(f"\\caption{{Tempo médio de execução - {instance_name}}}")
    tables.append(r"\begin{tabular}{l|r|r}")
    tables.append(r"\hline")
    tables.append(r"Algoritmo & Tempo Médio (s) & \# Execuções \\")
    tables.append(r"\hline")
    
    for algo, results in sorted(groups.items()):
        stats = calculate_statistics(results)
        if stats:
            algo_escaped = algo.replace('α', r'$\alpha$').replace('_', r'\_')
            tables.append(f"{algo_escaped} & {stats['tempo_medio']:.4f} & {stats['num_execucoes']} \\\\")
    
    tables.append(r"\hline")
    tables.append(r"\end{tabular}")
    tables.append(r"\end{table}")
    
    return "\n".join(tables)


def main():
    if len(sys.argv) < 3:
        print("Uso: python analyze_results.py <arquivo_csv> <instancia> [melhor_conhecido]")
        print()
        print("Argumentos:")
        print("  arquivo_csv     - Arquivo CSV com os resultados (ex: resultados.csv)")
        print("  instancia       - Nome da instância para filtrar (ex: test, queen5_5)")
        print("  melhor_conhecido - (Opcional) Melhor valor conhecido da literatura")
        print()
        print("Exemplo:")
        print("  python analyze_results.py resultados.csv test 5")
        sys.exit(1)
    
    csv_file = sys.argv[1]
    instance = sys.argv[2]
    
    # Usar valor conhecido do dicionário se não for passado como argumento
    if len(sys.argv) > 3:
        best_known = int(sys.argv[3])
    else:
        best_known = KNOWN_VALUES.get(instance, None)
        if best_known:
            print(f"Usando valor conhecido do dicionário: {best_known}")
    
    print(f"Carregando resultados de: {csv_file}")
    print(f"Filtrando por instância: {instance}")
    
    # Carregar e processar dados
    results = load_results(csv_file, instance)
    
    if not results:
        print(f"Erro: Nenhum resultado encontrado para a instância '{instance}'")
        sys.exit(1)
    
    print(f"Encontrados {len(results)} registros")
    
    # Agrupar por algoritmo
    groups = group_by_algorithm(results)
    
    # Imprimir tabela resumo no console
    print_summary_table(groups, best_known)
    
    # Gerar tabela LaTeX
    print("\n" + "=" * 80)
    print("TABELA LATEX - CONSOLIDADA")
    print("=" * 80)
    latex_consolidated = generate_latex_table(groups, instance, best_known)
    print(latex_consolidated)
    
    # Gerar tabelas individuais (conforme especificação)
    print("\n" + "=" * 80)
    print("TABELAS LATEX - INDIVIDUAIS (conforme especificação)")
    print("=" * 80)
    latex_individual = generate_individual_tables(groups, instance, best_known)
    print(latex_individual)
    
    # Salvar em arquivo
    output_file = f"tabelas_{instance}.tex"
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(f"% Tabelas geradas automaticamente - L(p,q)-Coloring\n")
        f.write(f"% Instância: {instance}\n")
        if best_known:
            f.write(f"% Melhor conhecido: {best_known}\n")
        f.write(f"% Data: gerado por analyze_results.py\n\n")
        f.write(latex_consolidated)
        f.write("\n\n")
        f.write(latex_individual)
    
    print(f"\nTabelas salvas em: {output_file}")
    
    # Resumo final
    print("\n" + "=" * 80)
    print("RESUMO")
    print("=" * 80)
    
    for algo, results in sorted(groups.items()):
        stats = calculate_statistics(results)
        if stats:
            print(f"\n{algo}:")
            print(f"  Melhor solução: {stats['melhor_solucao']}")
            print(f"  Média: {stats['media_solucoes']:.2f}")
            print(f"  Tempo médio: {stats['tempo_medio']:.4f}s")
            if best_known:
                devs = calculate_deviations(stats, best_known)
                print(f"  Desvio melhor: {devs['desvio_melhor']:.2f}%")
                print(f"  Desvio média: {devs['desvio_media']:.2f}%")


if __name__ == "__main__":
    main()
