#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"

int check_assembler_first(FILE *newRead, char *macro,int macSize, char **name, char *fileName){
    char tempArr[8];/* copies the name of the data designation */
    int warn = 0;/* indicates a warning was reported */
    char *p; /* pointer of strstr */
    char *word;/* pointer to store the input*/
    char line[81]; /* stores the input from the file */
    char row[81];/* copy of the line input*/
    char labelSave[32];/* will be used to store the label name plus $*/
    char *delim = " \t";/* delimiter to skip over characters */
    char *tok;/*will be used for strtok */
    int entExtClr ;/* flag for checking validity of .extern or .entry instruction*/
    int val;/*stores integer */
    int IC = 0; /* Counts the instructions*/
    int DC = 101; /* Counts the data*/
    int linCount = 0; /*Counts the lines of the file */
    int boolean=0;/* will be used in determining errors */
    int i;/* used in a for loop */
    int insEnd = 29; /* there are 29 invalid label names */
    /* caps and size counter for the symbol chart,data image array and instruction image array */
    int syCap = 50;
    int daCap = 50;
    int insCap= 50;
    int sySize = 0;
    int daSize = 0;
    int insSize = 0;
    char *symbolChart = NULL;
    char *dataIm = NULL;
    char *insIm = NULL;
    /* This will be used as the symbols chart */
    symbolChart = malloc(syCap*sizeof(char));
    if(!symbolChart){
        puts("allocation failed");
        return 1;
    }
    /* This will be used as the data image array */
    dataIm = malloc(daCap*sizeof(char));
    if(!dataIm){
        puts("allocation failed");
	free(symbolChart);
        return 1;
    }
    /* This will be used as the instruction image array */
    insIm = malloc(insCap*sizeof(char));
    if(!insIm){
        puts("allocation failed");
	free(symbolChart);
	free(dataIm);
        return 1;
    }
    /* as long as the file does not reach the end */
    while(fgets(line,81,newRead) != NULL){ 
        if(strstr(line, "\n") != NULL){
            line[strcspn(line, "\n")] = '\0';/*remove the newline character*/
        }
	entExtClr = 0;/* flag for checking ent or ext before label  */
        linCount++; /* increases the line count*/
        memcpy(row, line, 81);
        tok = strtok(line,delim);/*skips over tabs and white spaces */
        p = strstr(tok, ":"); /* pointer to the : symbol */
        /* if it's a label */
        if(p != NULL){
            *p = '\0'; /* replace ':' with null terminator */
            /* if the label name is not valid */
            if(checkValidName(tok,1)==1){
               printf(", in line %d\n", linCount);
               boolean = 1;/* error detected */
               continue;
            }
            /*checks if there is already a label by this name in the macro chart*/
            if(checkDupMac(macro, macSize, tok) != 0){
	       printf(", in line %d\n", linCount);
               boolean = 1;
               continue;
            }
            /*checks if the label name is of the 29 invalid words */
            for(i=0;i<insEnd;i++){
                if(strcmp(tok, name[i]) == 0){
                    printf("Invalid label name, in line %d\n",linCount);
                    boolean = 1;
                    continue;
                }
            }
            /* if the label is followed by an .extern or .entry */
            if(checkEntExt(tok, row, linCount) == 1){
                /* sets flags to 1 to make sure later on to enter the right loop*/
                entExtClr = 1;
                warn = 1;
                tok = strtok(NULL, delim);
                if(tok == NULL){
                    printf("No label name was given, in line %d\n", linCount);
                    boolean = 1;
                    continue;
                }
            }
            
            /*if the label name appears already in the symbol chart*/
            if(sySize != 0){
	        if(checkDupLab(symbolChart, sySize, tok) == 1){
		    printf("The label name appears multiple times in the file, in line %d\n", linCount);
                    boolean = 1;
                    continue;
                }
            }
            /* if no errors were detected while checking for the label name */
            if(warn != 1){
                /* copies the label name into the symbol chart and adds $ for seperation */
                strcpy(labelSave,tok);
                strcat(labelSave,"$");
                /*32 chars are a sufficient amount to store the label and a $ symbol( $ will be used to seperate the arguments in the symbo chart, and each line will end with a # )*/
                if(storeInArray(&syCap, &sySize, &symbolChart, labelSave, strlen(labelSave), IC, 1) == 1){
                    return 1;
                }
                tok = strtok(NULL, delim);/* gets the rest of the input*/
                /*if there an empty line after the label, will print an error and move to next line (do to both tok and word) */
                if(tok == NULL){
                    printf("The label is followed by an empty line, in line %d\n",linCount);
                    boolean = 1;
                    continue;
                }
                /* if its a symbol of data type */
                if(!strcmp(tok,".string")  || !strcmp(tok,".mat")  || !strcmp(tok,".data") ){
                    if(storeInArray(&syCap, &sySize, &symbolChart, "data#", 5, DC, 0) == 1){
                        boolean = 1;
                        continue;
                    }
                    /* copies the data type to an array*/
                    strcpy(tempArr,tok);
                    /* gets the rest of the line input*/
                    tok = strtok(NULL, "");
                    /* if the rest of the line is empty*/
                    if(tok == NULL){
                        printf("The %s instruction is missing values or characters, in line %d\n", tempArr, linCount);
                        boolean = 1;
                        continue;
                    }
                    /* checks if the data instruction was declared properly and if so, inserts the data*/
                    if(checkLabelVal(tok, linCount, tempArr, &DC, &dataIm, &daSize, &daCap, boolean) == 1){
                        boolean = 1;
                    }
                    continue;
                }
                /* checks the instruction's validity */
                else{
                    word = strtok(row, delim);/* gets the label*/
                    word = strtok(NULL, "");/* gets the rest of the input*/
                    /* stores the instruction label in the symbol chart */
                    if(storeInArray(&syCap, &sySize, &symbolChart, "code#", 5, IC + 100 , 0) == 1 ){
                        boolean = 1;
                        continue;
                    }
                    /* checks if the code instrution and is operands are valid*/
                    if (checkInst(word, linCount, &IC, &insIm, &insCap, &insSize, boolean, name) == 1){
                        boolean = 1;
                        continue;
                    }
                } 
	        continue;
            }
        }
        /* if it's a data type */
        if(!strcmp(tok,".string") || !strcmp(tok,".mat") || !strcmp(tok,".data") ){
            /* copies the data type to an array*/
            strcpy(tempArr,tok);
            /* gets the rest of the line input*/
            tok = strtok(NULL, "");
            if(tok == NULL){
                printf("The %s is followed by an empty line, in line %d\n",tempArr, linCount);
                boolean = 1;
                continue;
            } 
            /* checks if the data instruction was declared properly and if so, inserts the data*/
            if(checkLabelVal(tok, linCount, tempArr, &DC, &dataIm, &daSize, &daCap, boolean) == 1){
                boolean = 1;
            }
        }
        /* if it's a proper .entry or .extern instruction, will check validity of the label*/
        else if(entExtClr == 1 || strcmp(tok,".entry") == 0 || strcmp(tok,".extern") == 0){
            /* copies the data type to an array*/
            strcpy(tempArr,tok);
            /* if the label name is not valid */
            tok = strtok(NULL, ""); /* skip tab and spaces*/
	    if(tok == NULL){
	       printf("No label was entered, in line %d\n", linCount);
	       boolean = 1;
	       continue;
	    }
	    /* skip tabs or spaces*/
	    while(isspace(*tok)) tok++;
            /* if the label name is not valid */
            if(checkValidName(tok,1)==1){
               printf(", in line %d\n", linCount);
               boolean = 1;/* error detected */
               continue;
            }
            /*checks if there is already a label by this name in the macro chart*/
            if(checkDupMac(macro,macSize, tok) == 1){
	       printf(", in line %d\n", linCount);
               boolean = 1;
               continue;
            }
            /*checks if the label n"ame is of the 29 invalid words */
            for(i=0;i<insEnd;i++){
                if(strcmp(tok, name[i]) == 0){
                    printf("Invalid label name, in line %d\n",linCount);
                    boolean = 1;
                    continue;
                }
            }
            /*copy the label name + $ for seperation*/
            strcpy(labelSave,tok); 
            tok = strtok(NULL, delim); /* skips over tabs and spaces*/
            /* if there are extra characters after the label name*/
            if(tok != NULL){
                printf("The label is followed by extra characters, in line %d\n",linCount);
                boolean =1;
                continue;
            }
            /* if it's extern instruction */
            if(strcmp(tempArr, ".extern") == 0){
		/*if the same label appears at least twice */
	        if(checkDupLab(symbolChart, sySize, labelSave) == 1){
                    printf("The label name appears multiple times in the file, in line %d\n", linCount);
		    boolean = 1;
                    continue;
                }
	        strcat(labelSave,"$"); /* adds $ for seperation*/
                /* stores the label name in the symbol chart */
                if(storeInArray(&syCap, &sySize, &symbolChart, labelSave, strlen(labelSave), IC  , 1) == 1 ){
                    boolean = 1;
                    continue;
                }
                /* copies the value- 000 and the type-external, seperated by $ and ending with a # symbol*/
                if(storeInArray(&syCap, &sySize, &symbolChart, "external#", 9 , -1 , 0) == 1 ){
                    boolean = 1;
                    continue;
                }
            }
        }
        /* if it's an instruction*/
        else{
            if(checkInst(row,linCount,&IC, &insIm, &insCap,&insSize, boolean, name) == 1){
                boolean = 1;
            }
        }
    } 
    /* if there was at least one mistake in the file*/
    if(boolean == 1){
	free(symbolChart);
        free(dataIm);
        free(insIm); 
        return 1;
    }
    /* if there are more then 156 instructions*/
    if(IC+DC-100 > 156){
        printf("The number of data and code instruction lines exceeds the memory size\n");
        free(symbolChart);
        free(dataIm);
        free(insIm); 
        return 1;
    }
    IC -= 1; /* IC decreases by 1 to make sure there is no overflow*/

    /* goes over the symbol chart to seperate the data instructions from the code instructions*/
    for(i = 0; i < sySize; i++){
        /* if we reached the seperating mark*/
        if(symbolChart[i] == '$' ){
            i++;/* move to the next char to see if we reached an address value*/
            /* if there is a address value*/
            if((symbolChart[i]) == '0'){
                val = atoi(symbolChart + i);/* extracts the number*/
                val += IC ;/* add the final IC to seperate data from code*/
                sprintf(tempArr , "%d", val);/* copies the number into tempArr array */
                val = i+1;/* assigns val to save the index*/
                i = i+5;/* moves over to checks the type*/
                /* check if the address value is attributed to the DC or extern*/
                if(strncmp(symbolChart + i,"data",4) == 0 ){
                    /* stores the updated DC address value in the symbol chart*/
                    strncpy(symbolChart + val,tempArr, 3);  
                }
            }
        }
    }
    /* goes over the data image to seperate the data image from the code image*/
    for(i = 0; i < daSize; i += 16){
        /* if we reached the seperating mark*/
        val = atoi(dataIm + i);/* extracts the number*/
        val += IC ;/* add the final IC to seperate data from code*/
        sprintf(tempArr , "%d", val);/* copies the number into tempArr array */
        val = i+1;/* assigns val to save the index*/
        /* stores the updated DC address value in the data image*/
        strncpy(dataIm + val,tempArr, 3);    
        
    }
    rewind(newRead);/* moves the pointer to the beginning of the file*/
    /* moves to the second pass*/
    if(second_pass_assembler(newRead, &symbolChart, dataIm, &insIm, sySize, daSize, insSize, name, fileName, IC + 1, DC - 101) == 1){
	free(symbolChart);
        free(dataIm);
        free(insIm);
	return 1;
    }
    free(symbolChart);
    free(dataIm);
    free(insIm);    
    return 0;
}



 





