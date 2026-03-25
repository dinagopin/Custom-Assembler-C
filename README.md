

# Custom - C - Assembler Project  

## Project Overview
This project is an implementation of a **Two-Pass Assembler** for a symbolic assembly language, developed as part of the "Lab in Systems Programming" course with a final grade of 96. The assembler translates assembly code into a unique 10-bit binary representation, which is then encoded into a specialized base-4 system using the characters 'a', 'b', 'c', and 'd'.

### Core Features:
* **Macro Pre-processor:** Automatically expands macros (`mcro`) into full instructions before assembly.
* **Syntax Validation:** Comprehensive checks for label names, instruction formats, and memory limits.
* **Two-Pass Logic:**
  * **First Pass:** Builds the symbol table and validates instruction structure.
  * **Second Pass:** Resolves label addresses and generates machine code.


---
### 🛠️ Technical Specifications
* **Memory Limit:** 256 words.
* **Addressing Methods:** Immediate (`#`), Direct, Matrix (`label[r][r]`), and Register (`r0-r7`).
* **Base-4 Mapping:** 00=a, 01=b, 10=c, 11=d.

---
## 📂 Project Structure
The project is modularized to handle different stages of the assembly process:

```text
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
└── tests/                  # Test files and terminal reference images
```


---

## 🚀 How to Run and Test

To run this assembler, you must have a **Linux environment** with `gcc` and `make` installed.

#### 1. Download the Project
First, you need to get the code onto your local machine. You can do this by clicking the green **"Code"** button on GitHub and selecting **"Download ZIP"**, or by using the terminal:
```bash
git clone https://github.com/dinagopin/Custom-Assembler-C.git
cd Custom-Assembler-C
```

#### 2. Compilation
Once the files are on your computer, you need to "build" the executable. We use a **Makefile** to automate this process so you don't have to compile each file manually.

1.  **Open your Terminal** inside the folder where you downloaded the files.

2.  Type the following command and press Enter:
    ```bash
    make
    ```



*Note: Successful compilation results should look like `compilSuccess.png`.*

### 2. Execution
Run the assembler by passing the name of the test files **without** their extensions( `.as` )

**Example Command:**
```bash
./assembler kelet1 kelet4
```

---

## 🧪 Detailed Test Cases
The project includes several "kelet" (input) files to test success and failure scenarios. Use these to verify your local output.

| Input File | Type | Expected Behavior | Reference |
| :--- | :--- | :--- | :--- |
| **kelet1** | Success | Correct macro expansion and object file generation. | `kelet1.am`, `kelet1.ob` |
| **kelet2** | Failure | Fails due to extra words following the `mcro` name. | `kelet2faill.png` |
| **kelet3** | Success | Success, but generates a warning for labels preceding `.extern`. | `kelet3Warn.png`, `kelet3.am`, `kelet3.ob`  |
| **kelet4** | Success | Full output: generates `.ob`, `.ent` (entries), and `.ext` (externals). | `kelet4.ob`, `.ext`, `.ent` |
| **kelet5** | Failure | Fails due to a duplicate macro name in the file. | `kelet5fail.png` |
| **kelet6** | Failure | Fails due to duplicate macro/label definitions. | `kelet6fail.png` |
| **kelet7** | Failure | Comprehensive failure test: multiple syntax and operand errors. | `kelet7fail.png` |

---

### Understanding Success Output
For successful runs (like `kelet1` or `kelet4`), the assembler will generate:
* **`.am`**: The file after macro processing.
* **`.ob`**: The object code in special base-4.
* **`.ext` / `.ent`**: Files tracking external and entry symbols (if such exist ).

### Understanding Failure Output
If a file fails (like `kelet7`), the assembler will print the specific line number and error description to the terminal\. No output files (except `.am` for some input files) will be generated for failed runs.

---



