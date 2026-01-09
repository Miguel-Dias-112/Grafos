# run_tests.ps1 - Script para compilar e executar testes no Windows
# Uso: .\run_tests.ps1 [opcao]
# Opcoes: all, distance1, distance2, minimization, algorithms, randomized, inputs, performance, edge, list

param(
    [string]$TestSuite = "all"
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  L(p,q)-COLORING - SUITE DE TESTES" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Verificar se o GoogleTest está disponível
Write-Host "Verificando GoogleTest..." -ForegroundColor Yellow

# Tentar compilar os testes
Write-Host "Compilando testes..." -ForegroundColor Yellow
$compileCmd = "g++ -std=c++17 -O2 -Wall tests.cpp -lgtest -lgtest_main -lpthread -o tests.exe"

try {
    Invoke-Expression $compileCmd
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Compilação bem-sucedida!" -ForegroundColor Green
    } else {
        Write-Host "Erro na compilação!" -ForegroundColor Red
        Write-Host "Certifique-se de que o GoogleTest está instalado." -ForegroundColor Red
        Write-Host "Veja TESTES_README.md para instruções de instalação." -ForegroundColor Yellow
        exit 1
    }
} catch {
    Write-Host "Erro ao compilar: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""

# Executar os testes baseado na opção
switch ($TestSuite.ToLower()) {
    "all" {
        Write-Host "Executando TODOS os testes..." -ForegroundColor Green
        .\tests.exe
    }
    "distance1" {
        Write-Host "Executando testes de Distância 1..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestDistance1.*
    }
    "distance2" {
        Write-Host "Executando testes de Distância 2..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestDistance2.*
    }
    "minimization" {
        Write-Host "Executando testes de Minimização..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestMinimization.*
    }
    "algorithms" {
        Write-Host "Executando testes de Algoritmos..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestGreedyAlgorithms.*
    }
    "randomized" {
        Write-Host "Executando testes Randomizados..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestRandomized.*
    }
    "inputs" {
        Write-Host "Executando testes de Entrada..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestInputs.*
    }
    "performance" {
        Write-Host "Executando testes de Performance..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestPerformance.*
    }
    "edge" {
        Write-Host "Executando testes de Edge Cases..." -ForegroundColor Green
        .\tests.exe --gtest_filter=TestEdgeCases.*
    }
    "list" {
        Write-Host "Listando todos os testes disponíveis..." -ForegroundColor Green
        .\tests.exe --gtest_list_tests
    }
    default {
        Write-Host "Opção inválida: $TestSuite" -ForegroundColor Red
        Write-Host ""
        Write-Host "Opções disponíveis:" -ForegroundColor Yellow
        Write-Host "  all          - Executar todos os testes"
        Write-Host "  distance1    - Testes de restrição distância 1"
        Write-Host "  distance2    - Testes de restrição distância 2"
        Write-Host "  minimization - Testes de minimização"
        Write-Host "  algorithms   - Testes dos três algoritmos"
        Write-Host "  randomized   - Testes com múltiplas sementes"
        Write-Host "  inputs       - Testes de leitura de arquivo"
        Write-Host "  performance  - Testes de desempenho"
        Write-Host "  edge         - Testes de casos extremos"
        Write-Host "  list         - Listar todos os testes"
        exit 1
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  FIM DOS TESTES" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
