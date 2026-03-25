/* This will be used in detemining wheter a given line in an input file is longer than the valid length */
# define MAX_CHAR 90

/* This struct represents an instruction chart entry used during the first pass
   of the assembler. It helps in decoding instructions and detecting errors in the code.
   The fields are:
   - ins: the instruction name as a string
   - op: the opcode of the instruction as a string
   - frstOp: a string representing the allowed addressing methods for the first operand
   - scndOp: a string representing the allowed addressing methods for the second operand
*/
typedef struct {char *ins; char *op; char *frstOp; char *scndOp; } instOpcode;


/* This function receives as parameters a pointer to a string of char type (word)
   and a number (num). The function checks if the string is a valid macro or label name
   according to length, starting character, and allowed characters.
   If the string is too long, empty, does not start with a letter, or contains
   illegal characters, the function will determine from the number sent whether the name
   belongs to a macro or a label, print the corresponding error message, and return 1.
   Otherwise, the function will return 0.
*/
int checkValidName(char *word,int num);

/* This function receives as parameters a pointer to a string of char type (word)
   and a number (num). The function checks if the string is a valid macro or label name
   according to length, starting character, and allowed characters.
   If the string is too long, empty, does not start with a letter, or contains
   illegal characters, the function will determine from the number sent whether the name
   belongs to a macro or a label, print the corresponding error message, and return 1.
   Otherwise, the function will return 0.
*/
int check_mcro(FILE* readFile, FILE* writeFile ,char *fileName, char **macro, char **name, int *cap, int *size);

/* This function receives as parameters a pointer to a char array (macro),
   a number representing the array size (size), and a pointer to a string of char type (tok).
   The function goes over the char array, extracts each stored macro name, and compares it
   to the string pointed to by 'tok'. If a macro or label with the same name is found more than once,
   the function will print an error message indicating the duplication and return 1.
   Otherwise, after checking all macros in the array, the function will return 0.
*/
int checkDupMac(char *macro,int size, char *tok);

/* This function receives as parameters a pointer to a string of char type (tok),
   a number (tarNum), a pointer to a char array (binary), a number representing the line count (linCount),
   and a number representing the bit index (index).
   The function converts the given number to its binary representation in two's complement form,
   and stores the result in the char array pointed to by 'binary'.
   If tarNum is -1, the function will first convert the string pointed to by 'tok' into an integer.
   Before conversion, the function checks if the number is in the valid signed range for the given bit size.
   If the number is not in range, the function will print an error message with the line number and return -1.
   Otherwise, it will store the binary string (MSB first, and terminate with '\0') in 'binary'
   and return 0.
*/
int conBaseTwo(char *tok, int tarNum, char *binary, int linCount, int index);

/* This function receives as parameters pointers to two integers (cap and size),
   a pointer to a pointer to char (array), a pointer to a string of char type (binary),
   a number representing the maximum length to copy (maxLen),
   a number representing the counter value (counter), and a number (val).
   The function stores the given binary string into the dynamic array pointed to by 'array',
   optionally preceded by the counter value and a '$' symbol depending on 'val' and 'counter'.
   If there is not enough allocated space in the array to store the new data,
   the function will reallocate the array with double the current capacity.
   If the counter exceeds the memory limit (256) or the reallocation fails,
   the function will print an error message and return 1.
   Otherwise, the function will copy the data into the array, update 'size',
   and return 0.
*/
int storeInArray(int *cap, int *size, char **array, char *binary, int maxLen, int counter, int val);

/* This function receives as parameters a pointer to a string of char type (tok),
   a pointer to a string of char type representing a line of text (row),
   and a number representing the line count (linCount).
   The function checks if the label name given in 'tok' is immediately followed(optionally with spaces in between)
   by the instruction ".entry" or ".extern" in the same line.
   If so, the function will print a warning message with the line number and return 1.
   Otherwise, the function will return 0.
*/
int checkEntExt(char *tok, char *row, int linCount);

/* This function receives as parameters a pointer to a string of char type (tok),
   a pointer to a char indicating the end of the number string (endNum),
   a number representing the current line count (linCount),
   and a pointer to a string of char type representing the array name (array).
   The function parses the string from 'tok' up to 'endNum' to verify it represents a valid integer.
   If no number is found or if a decimal point is detected,
   the function prints an error message with the line number and returns 1.
   Otherwise, it returns 0 indicating a valid integer.
*/
int parseInt(char *tok, char *endNum, int linCount, char *array);

/* This function receives as parameters a pointer to a string of char type (tok),
   the current line count (linCount),
   a pointer to a string representing the instruction type (tempArr),
   pointers to integers for data counters and sizes (DC, daSize, daCap),
   a pointer to a dynamic array of strings (dataIm),
   and a boolean flag (boolean).
   The function first validates the syntax of the instruction following a label,
   such as checking matrix size declarations for .mat or the presence of quotes for .string.
   If the instruction is .data or .mat and validation passes,
   it calls the 'checkNumbers' function to further validate and store the corresponding data.
   The function prints error messages with the line number if errors are found,
   and returns 1 on error, or 0 on successful processing.
*/
int checkLabelVal(char *tok, int linCount, char *tempArr, int *DC, char **dataIm, int *daSize, int *daCap, int boolean);

/* This function receives as parameters a pointer to a string of char type (word)
   and a number indicating the type (type).
   The function checks if the string represents a valid register name.
   If 'type' is 0, it extracts the register string from matrix access syntax,
   otherwise it directly checks the given string.
   The function returns 0 if the string is a legal register ("r0" to "r7"),
   and 1 otherwise.
*/
int checkReg(char *word, int type);

/* This function receives as parameters a pointer to a dynamic char array (image),
   pointers to integers for capacity (cap), current size (size), a counter (counter),
   and a flag indicating whether a '#' symbol was recently stored (isShtrudel).
   The function decides whether to store a '#' symbol before writing a new address line
   into the dynamic array(data or instruction image) to separate from the value.
   It avoids storing '#' before the first address line and after the last,
   as those are either unnecessary or separated by '@' symbols.
   The function returns 1 if storing '#' fails, or 0 otherwise.
*/
int noErrStore(char **image, int *cap, int *size, int *counter, int *isShtrudel);

/* This function receives as parameters a pointer to a char array (array),
   a number representing the array size (size), and a pointer to a string of char type (tok).
   The function goes over the char array, extracts each stored label name, and compares it
   to the string pointed to by 'tok'. If a label with the same name is found,
   the function will return 1. Otherwise, after checking all labels in the array,
   the function will return 0.
*/
int checkDupLab(char *array, int size, char *tok);

/* This function receives as parameters a file pointer (newRead) to the assembly source file,
   the macro chart array (macro) with its size (macSize), an array of reserved language words (name),
   and the source file name (fileName).
   The function performs the first pass of the assembler by going through each line of the file to:

   Process labels that appear before instructions or data declarations, validating label names
   and storing them in the symbol chart using storeInArray with '#' separators between entries
   Handle data declarations (.data, .string, .mat) by storing label information and processing
   the data values into the data image array
   Process instruction lines by validating syntax and storing binary representations in the
   instruction image array
   Handle .extern declarations by storing external labels in the symbol chart with "external" type
   Validate that label names are not duplicates, reserved words, or already defined macros
   Maintain instruction counter (IC) and data counter (DC) to track memory addresses

   At the end, if no errors were found (boolean == 0), the function adds the final IC value
   to all data-type entries in the symbol chart and data image to separate data from code
   addresses. It then calls the second pass assembler to complete the assembly process.
   The function allocates and manages three dynamic arrays: symbolChart for labels,
   dataIm for data storage, and insIm for instruction storage.
   Returns 0 on successful completion of first pass and second pass, 1 if any validation
   errors occur, memory allocation fails, or memory size exceeds 156 instruction/data lines.
*/
int check_assembler_first(FILE *newRead, char *macro, int macSize, char **name, char *fileName);

/* This function receives as parameters a pointer to a string (tok) containing an assembly instruction line,
   the current line number (linCount) for error reporting, a pointer to the instruction counter (IC),
   a pointer to the instruction image dynamic array (insIm), pointers to its capacity (insCap) and size (insSize),
   a boolean flag indicating if prior errors occurred, and an array of reserved language words (name).
   The function validates the instruction syntax, operand types, and addressing methods against the supported
   instruction set (mov, cmp, add, sub, lea, clr, not, inc, dec, jmp, bne, jsr, red, prn, rts, stop).
   It supports four addressing methods:

   Immediate addressing (#value)
   Direct addressing (label)
   Matrix addressing (label[reg][reg])
   Register addressing (r0-r7)

   When boolean is 0 (no prior errors), it converts the instruction to binary format and stores it
   in the instruction image plus the address line(IC), and incrementing IC.
   When boolean is 1 (prior errors exist), it only validates without storing.
   Returns 0 on successful validation/processing, 1 if any syntax or compatibility errors are found.
*/
int checkInst(char *tok, int linCount, int *IC, char **insIm, int *insCap, int *insSize, int boolean, char **name);

/* This function receives as parameters a pointer to a string of char type (store)
   and an integer (count).
   It converts the integer value (either from 'count' if >=0, or from the string if count == -1)
   into a custom base-4 representation using the letters 'a' for 0, 'b' for 1, 'c' for 2, and 'd' for 3.
   The converted result is stored in 'store' as a null-terminated string.
   The function returns 0 on success.
*/
int conDecFour(char *store, int count);

/* This function receives as a parameter a pointer to a string of char type (store),
   representing a 10-character binary string (each character is '0' or '1').
   It converts every two binary digits into a custom base-4 character:
   '00' → 'a', '01' → 'b', '10' → 'c', and '11' → 'd'.
   The converted 5-character string is stored back in 'store' and null-terminated.
   The function returns 0 on success.
*/
int conBinFour(char *store);

/* This function receives as a parameter a pointer to a string of char type (store)
   representing a decimal number as a string.
   It converts this number into an 8-bit binary string consisting of '0' and '1' characters,
   left-padded with zeros to fill 8 characters.
   The converted binary string is stored in 'store' and null-terminated.
   The function returns 0 on success.
*/
int conBinUn(char *store);

/* This function receives as parameters a file pointer (newRead) to the assembly source file,
   a pointer to the symbol chart dynamic array (symbolChart), the data image array (dataIm),
   a pointer to the instruction image dynamic array (insIm), the sizes of symbol chart (sySize),
   data image (daSize) and instruction image (insSize), an array of reserved language words (name),
   the source file name (fileName), and the final instruction counter (IC) and data counter (DC).
   The function performs the second pass of the assembler by going through each line of the file to:

   Process .entry directives, validating that entry labels exist in the symbol chart and marking
   them with '1' flag, ensuring they are not already marked as external labels
   Resolve label references in instructions by finding labels in the symbol chart and replacing
   placeholder '?' characters in the instruction image with actual addresses plus A.R.E bits
   Handle external label references by storing them in a temporary extern array for .ext file generation
   Generate three output files after converting addresses and binary data to special base-4 format:

   .ob (object file containing IC/DC counts, instruction image, and data image)
   .ext (external references file with label names and their usage addresses)
   .ent (entry points file with entry label names and their addresses)

   The function processes each instruction line to extract operand labels, looks them up in the
   symbol chart, and updates the instruction image with resolved addresses including appropriate
   A.R.E (Absolute/Relocatable/External) encoding. All numeric values are converted to the
   special base-4 format before writing to output files.
   Returns 0 on successful completion with all output files generated and converted to base-4,
   1 if any validation errors occur or file operations fail.
*/
int second_pass_assembler(FILE *newRead, char **symbolChart, char *dataIm, char **insIm, int sySize, int daSize, int insSize, char **name, char *fileName, int IC, int DC);

/* This function receives as parameters a pointer to a string of char type (tok),
   a number indicating the type of data (number),
   pointers to integers for data counters and sizes (DC, daSize, daCap),
   a pointer to a dynamic array of strings (dataIm),
   the matrix size (matsize), a boolean flag (boolean),
   and the current line count (linCount).
   The function parses the given line of data or matrix values from 'tok',
   validates the format, checks for errors like missing commas or out-of-range values,
   and if all checks pass, stores the corresponding binary representations
   into the dynamic data array using the 'storeInArray' function.
   It handles different rules depending on whether the input is for .data or .mat,
   and prints appropriate error messages including the line number when errors occur.
   The function returns 1 if an error is found, or 0 if the input is valid and data is stored successfully.
*/
int checkNumbers(char *tok, int number, int *DC, int matsize, char **dataIm, int *daSize, int *daCap, int boolean, int linCount);





