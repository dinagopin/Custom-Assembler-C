
# Custom - C - Assembler Project  

## Project Overview
This project is an implementation of a **Two-Pass Assembler** for a symbolic assembly language, developed as part of the "Lab in Systems Programming" course with a final grade of 96. The assembler translates assembly code into a unique 10-bit binary representation, which is then encoded into a specialized base-4 system using the characters 'a', 'b', 'c', and 'd'.

### Core Features:
* **Macro Pre-processor:** The pre-processor scans for `mcro` definitions and replaces macro calls with their stored instructions.
* **Syntax Validation:** Comprehensive checks for label names, instruction formats, and memory limits.
* **Two-Pass Logic:**
  * **First Pass:** Builds the symbol table and validates instruction structure.
  * **Second Pass:** Resolves label addresses and generates machine code.

###  Technical Specs
* **Memory Limit:** 256 words.
* **Addressing:** Immediate (#), Direct, Matrix (label[r][r]), and Register (r0-r7).
* **Base-4 Mapping:** 00=a, 01=b, 10=c, 11=d.

---
##  Success vs.  Failure Criteria

###  Successful Run
A run is successful if the code is logically sound and fits within the memory limit.
* **Output:** Generates `.am` (expanded macros) and `.ob` (object code) files. It only creates `.ent` and `.ext` if those directives are present in the source.
* **Example:** `kelet1` and `kelet4`.

###  Warning State
Occurs for redundant but legal code, such as placing a label before `.extern` or `.entry`. The assembler **will still generate** all output files.

###  Failure States
If an error occurs, the process stops, the **line number** is printed, and **no output files (.ob, .ext, .ent) are created**.

* **Macro Errors:** Extra text after a macro name, duplicate macros, or using reserved names like `mov`.
* **Assembly Errors:** Invalid operands, missing/extra commas, undefined labels, or malformed `.data`/`.string`.
* **Constraint Errors:** Lines exceeding **80 characters** or exceeding the **256-word** memory limit.


---

## How to Run
This project requires a **Linux environment** with `gcc` and `make`.

*Note: This project adheres to **ANSI C** standards using `-ansi -Wall -pedantic` flags.*

### 1. Setup and Build
Download the repository and open your terminal in the project folder. Compile using:
```bash
make
```
*Note: Successful compilation results should look like `compilSuccess.png`.*


### 2. Execution
Run the assembler by providing the path to a single input file (without the extension `.as`).
```bash
./assembler tests/inputs/kelet1 
```
*Note: For the most stable performance and accurate error reporting, please process **one file at a time**. Running multiple files in a single command may lead to memory overlap (Segmentation Faults) due to the strict ANSI C memory management used in this version.*

### 3. Compare Results & Verify

 The command above will create new output files (like `kelet1.ob`) inside the `tests/inputs/` folder.
 Open your generated file and compare it to the version provided in `tests/expected/kelet1.ob`. If they match, the assembler is running perfectly.   
 
 
 For files that are supposed to fail, simply run the command and check if your terminal output matches the reference image in `tests/screenshots`.

---

## Project Structure

The project is modularized to handle different stages of the assembly process:

```text
.
.
├── Makefile                # Build system (gcc -ansi -Wall -pedantic)
├── main.c                  # Program entry point and file management
├── data.h                  # Global structures and function prototypes
│
├── kdamassembler.c         # Macro pre-processor (.as -> .am)
├── check_assembler_first.c # First pass logic and symbol construction
├── check_assembler_second.c# Second pass logic and address resolution
│
├── instFunction.c          # Instruction set and addressing methods
├── functions.c             # Label and name validation utilities
├── funcSecPass.c           # Base-4 and binary conversion logic
│
└── tests/                  # Project testing resources              
    ├── inputs/             # Original .as assembly files
    ├── expected/           # "Perfect" .ob, .am, .ext, .ent files for comparison
    └── screenshots/        # Terminal images (compilSuccess.png, kelet7fail.png, etc.)
```

---

## Detailed Test Cases
The project includes several "kelet" (input) files to test success and failure scenarios. Use these to verify your local output.

| Input File | Type | Expected Behavior | Reference |
| :--- | :--- | :--- | :--- |
| **kelet1** | Success | Correct macro expansion and object file generation. | `kelet1.am`, `kelet1.ob` |
| **kelet2** | Failure | Fails due to extra words following the `mcro` name. | `kelet2faill.png` |
| **kelet3** | Success | Success, but generates a warning for labels preceding `.extern`. | `kelet3Warn.png`, `kelet3.am`, `kelet3.ob` |
| **kelet4** | Success | Full output: generates `.ob`, `.ent` (entries), and `.ext` (externals). | `kelet4.ob`, `.ext`, `.ent` |
| **kelet5** | Failure | Fails due to a duplicate macro name in the file. | `kelet5fail.png` |
| **kelet6** | Failure | Fails due to duplicate macro/label definitions. | `kelet6fail.png` |
| **kelet7** | Failure | Comprehensive failure test: multiple syntax and operand errors. | `kelet7fail.png` |

