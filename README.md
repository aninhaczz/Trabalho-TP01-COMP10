# 🔎 Analisador Léxico em C — MicroPascal

## 📌 Descrição
Este projeto implementa um **Analisador Léxico** em linguagem C para a linguagem **MicroPascal**.  
O programa é capaz de:
- Reconhecer **tokens** da linguagem.
- Construir e exibir a **Tabela de Símbolos**.
- Detectar e reportar **erros léxicos**.
- Gerar um arquivo de saída `.lex` com a lista de tokens reconhecidos.

Este trabalho corresponde ao **TP01** da disciplina **Linguagens Formais, Autômatos e Compiladores (COMP10)**.

---

## ⚙️ Como compilar

### 🔹 Linux / WSL
```bash
make
# ou manualmente:
gcc trabalho.c symtable.c -o lexer
```

### 🔹 Windows (MinGW)
```powershell
gcc trabalho.c symtable.c -o lexer.exe
```

---

## ▶️ Como executar
O analisador recebe um arquivo `.pas` de entrada (programa em MicroPascal):

```bash
./lexer teste1.pas
```

Ou no Windows:

```powershell
.\lexer.exe teste1.pas
```

### 📂 Saídas
- **Arquivo `teste1.pas.lex`** → lista de tokens no formato `<TOKEN, lexema, linha, coluna>`.
- **Terminal** → impressão da **Tabela de Símbolos** (e mensagens de erro léxico, se houver).

---

## 🧪 Testes incluídos

Na pasta estão **6 programas de teste**:

✅ **Programas corretos**
- `teste1.pas` → Exemplo completo (declarações, if/else, while).
- `teste2.pas` → Programa de soma simples.
- `teste3.pas` → Laço `while` básico.

❌ **Programas com erro**
- `erro1.pas` → Caractere inválido `@`.
- `erro2.pas` → String não suportada.
- `erro3.pas` → Comentário `{ ... }` não suportado.

---

## 📑 Relatório
O documento **Relatorio_Final.pdf** contém:
- Introdução
- Implementação (structs, funções, AFD)
- Figura do AFD
- Testes (3 corretos e 3 com erro, com saídas)
- Conclusão
- Referências
- Anexos (arquivos fonte)

---

## 📂 Estrutura do projeto
```
.
├── Makefile
├── trabalho.c
├── symtable.c
├── symtable.h
├── token.h
├── teste1.pas
├── teste2.pas
├── teste3.pas
├── erro1.pas
├── erro2.pas
├── erro3.pas
├── Relatorio_Final.pdf
└── README.md
```

---

## 👨‍💻 Autores
- Alunos(as): **Ana Beatriz Alves, Artur Coelho, David Cordeiro, Eduarda Alves e Guilherme Andrade**
- Professor: Marcelo Eustáquio
- Disciplina: Linguagens Formais, Autômatos e Compiladores — COMP10
