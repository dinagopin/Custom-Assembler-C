# Custom-Assembler-C
# Two-Pass Assembler Project - ממ"ן 14

## Overview
This project is an implementation of a **Multi-Pass Assembler** developed for the "Lab in Systems Programming" course (Course 20465). It translates a symbolic assembly language into a unique 10-bit binary representation, eventually encoded into a specialized base-4 system (using 'a', 'b', 'c', and 'd').

The assembler handles:
* [cite_start]**Pre-processor (Macro Expansion):** Identifies and expands `mcro` definitions[cite: 3, 18].
* [cite_start]**First Pass:** Validates syntax, builds the symbol table, and determines memory addresses for Instructions (IC) and Data (DC).
* [cite_start]**Second Pass:** Resolves label addresses, handles external/entry references, and generates final output files[cite: 14].

---

## 📂 Project Structure
This tree illustrates how `main.c` coordinates the different logic modules:

```text
.
[cite_start]├── Makefile                # Automated build script using gcc -ansi -Wall -pedantic 
[cite_start]├── main.c                  # Core logic: Manages file loops and calls assembler passes [cite: 5]
├── data.h                  # Header: Shared structures (instOpcode) and function prototypes
│
[cite_start]├── kdamassembler.c         # Pre-processor: Handles macro expansion (.as -> .am) [cite: 3, 19]
[cite_start]├── check_assembler_first.c # First Pass: Building symbols and instruction image 
[cite_start]├── check_assembler_second.c# Second Pass: Resolving addresses and generating .ob, .ext, .ent [cite: 14]
│
[cite_start]├── functions.c             # Utility: Label validation and duplicate checks 
[cite_start]├── instFunction.c          # Instruction Logic: Opcode decoding and addressing methods 
[cite_start]├── funcSecPass.c           # Conversion: Logic for base-4 and binary formatting 
[cite_start]└── tests/                  # Test files (kelet1 - kelet7) and reference images [cite: 1, 9, 13]