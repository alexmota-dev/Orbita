# Projeto Órbita

## Como executar o programa

### Pré-requisitos
- MSYS64 instalado no sistema
- Compilador C++ (gcc/g++)

### Passos para execução

1. Abra o terminal MSYS64

2. No terminal MSYS64, execute:
```bash
bash
```

3. Navegue até o diretório do projeto (se necessário)

4. Compile o projeto (se ainda não foi compilado):
```bash
g++ -o orbit *.cc
```

5. Execute o programa:
```bash
./orbit.exe
```

### Compilação alternativa

Se precisar recompilar o projeto:
```bash
g++ main.cc Planet.cc Point.cc Vector.cc draw.cc -o orbit
```

## Descrição

Projeto de simulação de órbitas planetárias desenvolvido em C++.

