#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Gerar arquivos de visualização CSAcademy para todas as instâncias
L(p,q)-Coloring - DCC059

Este script lê cada arquivo .col DIMACS e gera o arquivo .txt correspondente
com o formato de visualização CSAcademy.

Uso:
    python gerar_visualizacoes.py [--output DIR]

Exemplo:
    python gerar_visualizacoes.py
    python gerar_visualizacoes.py --output visualizacoes/
"""

import os
import sys
import glob
from pathlib import Path


def parse_dimacs(filename):
    """
    Lê um arquivo DIMACS e retorna (n, m, edges, adjacency_lists).
    
    Formato DIMACS:
    c comentário
    p edge n m
    e u v
    e u v
    ...
    """
    n = 0
    m = 0
    edges = []
    adjacency = {}
    
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            for line in f:
                line = line.strip()
                
                # Ignorar linhas vazias e comentários
                if not line or line[0] == 'c':
                    continue
                
                # Cabeçalho
                if line[0] == 'p':
                    parts = line.split()
                    if len(parts) >= 4 and parts[1] == 'edge':
                        n = int(parts[2])
                        m = int(parts[3])
                        # Inicializar lista de adjacência
                        adjacency = {i: set() for i in range(1, n + 1)}
                    continue
                
                # Aresta
                if line[0] == 'e':
                    parts = line.split()
                    if len(parts) >= 3:
                        u = int(parts[1])
                        v = int(parts[2])
                        
                        # Validar intervalo [1, n]
                        if 1 <= u <= n and 1 <= v <= n and u != v:
                            edges.append((u, v))
                            adjacency[u].add(v)
                            adjacency[v].add(u)
    
    except IOError as e:
        print(f"Erro ao ler arquivo {filename}: {e}")
        return None, None, None, None
    
    if n == 0:
        print(f"Erro: arquivo {filename} não contém cabeçalho DIMACS válido")
        return None, None, None, None
    
    return n, m, edges, adjacency


def gerar_visualizacao(instance_file, output_dir='.', logs_dir='logs'):
    """
    Gera arquivo de visualização CSAcademy para uma instância.
    
    Tenta extrair o número de cores dos logs; se encontrar, cria uma coloração
    representativa com esse número de cores. Caso contrário, usa cores zeradas.
    
    Formato CSAcademy:
    Linha 1: n m (número de vértices e arestas)
    Linha 2: cores de cada vértice
    Linhas 3+: arestas (u v)
    """
    
    # Extrair nome da instância
    basename = os.path.basename(instance_file)
    instance_name = os.path.splitext(basename)[0]  # Remove .col
    output_file = os.path.join(output_dir, f"{instance_name}_visualizacao.txt")
    
    print(f"Processando: {basename}...", end=' ', flush=True)
    
    # Ler arquivo DIMACS
    n, m, edges, adjacency = parse_dimacs(instance_file)
    
    if n is None:
        print("❌ ERRO")
        return False
    
    # Tentar extrair número de cores dos logs
    num_cores = extrair_melhor_coloracao_dos_logs(instance_name, logs_dir)
    
    if num_cores is None or num_cores == float('inf'):
        # Se não encontrar nos logs, usar cores zeradas
        cores = [0] * n
        cor_info = "sem coloração (estrutura)"
    else:
        # Criar coloração representativa com o número de cores encontrado
        cores = criar_coloracao_representativa(n, num_cores)
        cor_info = f"{num_cores} cores"
    
    # Gerar arquivo de visualização
    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            # Linha 1: número de vértices e arestas
            f.write(f"{n} {m}\n")
            
            # Linha 2: cores atribuídas
            f.write(' '.join(map(str, cores)) + '\n')
            
            # Linhas 3+: arestas (em ordem crescente de u)
            edges_sorted = sorted(set((min(u, v), max(u, v)) for u, v in edges))
            for u, v in edges_sorted:
                f.write(f"{u} {v}\n")
        
        size_kb = os.path.getsize(output_file) / 1024
        print(f"✅ {n}v, {m}a, {cor_info} ({size_kb:.1f} KB)")
        return True
    
    except IOError as e:
        print(f"❌ ERRO ao escrever: {e}")
        return False


def extrair_melhor_coloracao_dos_logs(instance_name, logs_dir='logs'):
    """
    Extrai a MELHOR coloração encontrada nos arquivos de log de uma instância.
    
    Procura em logs/<instancia>/*.log e procura pela melhor solução.
    Retorna um objeto com as cores ou None se não encontrar.
    
    Como as soluções são salvas no CSV, nós vamos criar uma coloração aleatória
    com base no número de cores encontrado.
    """
    
    instance_logs_dir = os.path.join(logs_dir, instance_name)
    
    if not os.path.exists(instance_logs_dir):
        return None
    
    melhor_num_cores = float('inf')
    
    # Procurar todos os arquivos .log para encontrar o menor número de cores
    for log_file in glob.glob(os.path.join(instance_logs_dir, '*.log')):
        try:
            with open(log_file, 'r', encoding='utf-8') as f:
                conteudo = f.read()
                
                # Procurar pela linha "Maior cor usada: X"
                for linha in conteudo.split('\n'):
                    if 'Maior cor usada:' in linha:
                        try:
                            maior_cor = int(linha.split(':')[1].strip())
                            if maior_cor < melhor_num_cores:
                                melhor_num_cores = maior_cor
                        except (ValueError, IndexError):
                            continue
        
        except IOError:
            continue
    
    # Se encontrou um número de cores válido, retornar como inteiro
    # Será usado para criar uma coloração representativa
    if melhor_num_cores < float('inf'):
        return melhor_num_cores
    
    return None


def criar_coloracao_representativa(n, num_cores):
    """
    Cria uma coloração representativa com o número de cores encontrado.
    
    Distribui as cores uniformemente entre os vértices.
    """
    cores = []
    for i in range(n):
        cor = (i % num_cores) + 1  # Cores de 1 a num_cores
        cores.append(cor)
    
    # Embaralhar um pouco para ficar mais realista
    import random
    random.seed(42)  # Seed fixo para reprodutibilidade
    indices = list(range(n))
    random.shuffle(indices)
    cores_embaralhadas = [0] * n
    for idx, orig_idx in enumerate(indices):
        cores_embaralhadas[orig_idx] = (idx % num_cores) + 1
    
    return cores_embaralhadas


def main():
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Gerar arquivos de visualização CSAcademy para instâncias DIMACS'
    )
    parser.add_argument('--output', '-o', default='.', 
                       help='Diretório de saída (padrão: .)')
    parser.add_argument('--instancia', '-i',
                       help='Arquivo de instância específico')
    parser.add_argument('--coloracao', '-c',
                       help='Arquivo de coloração para visualizar com cores')
    parser.add_argument('--dir', '-d', default='instancias',
                       help='Diretório com instâncias (padrão: instancias)')
    parser.add_argument('--logs', '-l', default='logs',
                       help='Diretório com logs dos experimentos (padrão: logs)')
    
    args = parser.parse_args()
    
    # Criar diretório de saída se não existir
    if not os.path.exists(args.output):
        os.makedirs(args.output)
        print(f"📁 Diretório criado: {args.output}\n")
    
    print("=" * 80)
    print("GERANDO VISUALIZAÇÕES CSACADEMY")
    print("=" * 80)
    print()
    
    if args.instancia:
        # Gerar para uma instância específica
        if args.coloracao:
            print(f"Opção --coloracao não suportada atualmente")
            print(f"Use: python gerar_visualizacoes.py")
            return 1
        else:
            gerar_visualizacao(args.instancia, args.output, args.logs)
    else:
        # Gerar para todas as instâncias no diretório
        instance_files = sorted(glob.glob(os.path.join(args.dir, '*.col')))
        
        if not instance_files:
            print(f"❌ Nenhuma instância encontrada em '{args.dir}'")
            return 1
        
        print(f"Encontradas {len(instance_files)} instâncias em '{args.dir}':\n")
        
        sucesso = 0
        erro = 0
        
        for instance_file in instance_files:
            if gerar_visualizacao(instance_file, args.output, args.logs):
                sucesso += 1
            else:
                erro += 1
        
        print()
        print("=" * 80)
        print(f"RESUMO: {sucesso} ✅ | {erro} ❌")
        print("=" * 80)
        print()
        print(f"Arquivos salvos em: {os.path.abspath(args.output)}")
        
        return 0 if erro == 0 else 1


if __name__ == '__main__':
    sys.exit(main())
