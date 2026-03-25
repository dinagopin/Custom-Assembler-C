#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"

/* This function receives command line arguments (argc, argv) containing assembly file names.
For each .as file, it calls check_mcro() to process macros and generate .am files,
then calls check_assembler_first() to perform both assembly passes and generate
final output files (.ob, .ext, .ent). Manages dynamic memory for macro processing
and continues with next file if errors occur.
Returns 0 on successful processing, 1 if critical errors occur or no files provided.
*/
int main(int argc, char *argv[]) {
    FILE* readFile = NULL; /* Pointer for the input file */
    FILE* writeFile = NULL;/*Pointer for the output file */
    int cap = 100,size = 0; /*constant variables for macros  */
    FILE* newRead = NULL; /*Pointer for the output file after update*/
    int count = 1;/* counter for number of files*/
    char fileName[100],file[100];/* space for the .am or .as output or input file*/ 
    int correct;/* checks the return value of 'check_mcro'*/
    /*char array containing invalid macro/label names*/
    char *name[] ={"data","string","mat","entry","extern","mov","cmp","add","sub","not","clr","lea","inc","dec",
    "jmp","bne","red","prn","jsr","rts","stop","r0","r1","r2","r3","r4","r5","r6","r7","mcro","mcroend"};
    char *macro;
    macro = malloc(cap*sizeof(char));/* dynamically allocated space for the macro chart */
    
    if(!macro){/* if allocation failed */
        fprintf(stderr,"malloc failed\n");
        return 1;
    }

    /* if no file was entered*/
    if(argc == 1){
        puts("No file was entered");
    }
    /* each time a new file is in the command line is opened*/
    for(count=1;count<argc;count++){
	macro = malloc(cap*sizeof(char));/* dynamically allocated space for the macro chart */
        /*opens the '.as' ending file */
        strcpy(file, argv[count]);
        strcat(file, ".as");
        readFile = fopen(file, "r");/*opens the read file */
        if(readFile == NULL){/* if not able to open, prints error*/
            perror("Error opening file");
            continue;
        }
        /* creates the name of the output file*/
        strcpy(fileName, argv[count]);
        strcat(fileName, ".am");
        writeFile = fopen(fileName, "w"); /* open file for writing */
        if (writeFile == NULL) {/* if can't open the write file,prints error*/
            perror("Error opening file");
            return 1;
        }
        correct=check_mcro(readFile, writeFile, fileName, &macro, name, &cap, &size); /*call the func to check the mcro*/
        fclose(readFile);
	/* if the file has errors*/
        if(correct==1){
	    free(macro);
            continue;
	}
	else fclose(writeFile);
        /*open the output file after writing mcro */
        newRead = fopen(fileName, "r");
        if (newRead == NULL) {/* if can't open the write file,prints error*/
            perror("Error opening file");
            return 1;
        }
	
        /*first assembler pass over the '.am' file */
        if(check_assembler_first(newRead, macro,size, name, argv[count]) == 1){
           fclose(newRead); 
	   return 1;
        }
        free(macro);/* frees the space allocated for macro */
	fclose(newRead);
     }
     return 0;
} 

