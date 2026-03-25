# Custom-Assembler-C
# Two-Pass Assembler Project - ממ"ן 14

## Overview
This project is an implementation of a **Multi-Pass Assembler** developed for the "Lab in Systems Programming" course (Course 20465). It translates a symbolic assembly language into a unique 10-bit binary representation, eventually encoded into a specialized base-4 system (using 'a', 'b', 'c', and 'd').

The assembler handles:
* **Pre-processor (Macro Expansion):** Identifies and expands `mcro` definitions[cite: 3, 18].
* **First Pass:** Validates syntax, builds the symbol table, and determines memory addresses for Instructions (IC) and Data (DC).
* **Second Pass:** Resolves label addresses, handles external/entry references, and generates final output files[cite: 14].

---

## 📂 Project Structure
This tree illustrates how `main.c` coordinates the different logic modules:


.
├── Makefile                # Automated build script using gcc -ansi -Wall -pedantic 
├── main.c                  # Core logic: Manages file loops and calls assembler passes 
├── data.h                  # Header: Shared structures (instOpcode) and function prototypes
│
├── kdamassembler.c         # Pre-processor: Handles macro expansion (.as -> .am) 
├── check_assembler_first.c # First Pass: Building symbols and instruction image 
├── check_assembler_second.c# Second Pass: Resolving addresses and generating .ob, .ext, .ent 
│
├── functions.c             # Utility: Label validation and duplicate checks 
|  instFunction.c          # Instruction Logic: Opcode decoding and addressing methods 
├── funcSecPass.c           # Conversion: Logic for base-4 and binary formatting 
└── tests/                  # Test files (kelet1 - kelet7) and reference images 

🚀 How to Run and Test:
This assembler is designed for Linux/Unix environments
.1. Compilation - Run the provided makefile to build the executable. This ensures all flags (-ansi, -Wall, -pedantic) are applied strictly.
Bash 
make
Note: Successful compilation results should look like compilSuccess.png.2. ExecutionRun the assembler by passing the name of the test files without their extensions:Bash./assembler kelet1 kelet4
🧪 Detailed Test Cases:
The project includes several "kelet" (input) files to test success and failure scenarios. Use these to verify your local output.Input FileTypeExpected BehaviorReferencekelet1SuccessCorrect macro expansion and object file generation.
`kelet1.am`, `kelet1.ob`

kelet2FailureFails due to extra characters following a macro name.



`kelet2faill.png`

kelet3WarningSuccess, but generates a warning for labels preceding `.extern`.



`kelet3Warn.png`

kelet4SuccessFull output: generates `.ob`, `.ent` (entries), and `.ext` (externals).



`kelet4.ob`, `.ext`, `.ent`

kelet5FailureFails due to a duplicate macro name in the file.



`kelet5fail.png`

kelet6FailureFails due to duplicate macro/label definitions.



`kelet6fail.png`

kelet7FailureComprehensive failure test: multiple syntax and operand errors.



`kelet7fail.png`

Understanding Success Output