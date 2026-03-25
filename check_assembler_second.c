#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"



int second_pass_assembler(FILE *newRead, char **symbolChart, char *dataIm, char **insIm, int sySize, int daSize, int insSize, char **name, char *fileName, int IC, int DC){
    FILE* extFile = NULL; /* Pointer for the extern file */
    FILE* entFile = NULL;/*Pointer for the entry file */
    FILE* assemFile = NULL; /* Pointer for the final output file */
    char line[81], row[81];/* stores each line*/
    char storeName[100]; /* stores the file name and the extension(.ob etc.) */
    char tempArr[6];/* store the label type  */
    char binary[31], store[31]; /* stores the converted number */
    char *extArr = NULL;/* this will be for the extern array*/
    int cap = 50;/* the meaximus size to store in a line + index to copy to array*/
    int index = 0;/* the index of extArr array*/
    char *tok, *delim = " \t",*ptr;/*pointers for the function*/
    int boolean = 0; /* flag for checking errors*/
    int linCount = 0; /* counter for the line*/
    int i, k ;/* index for loop*/
    int passInst = 0; /*index to go over the insIm array */
    int flagEnt = 0;/* flag for checking if entry label exists*/
    /* This will be used as the extern chart */
    extArr = malloc(cap*sizeof(char));
    if(!extArr){
        puts("allocation failed");
        return 1;
    }
    /* as long as the file does not reach the end */
    while(fgets(line,81,newRead) != NULL){
        linCount++;
        /*remove the newline character*/
        if(strstr(line, "\n") != NULL){
            line[strcspn(line, "\n")] = '\0';
        }
        strcpy(row, line);/* copies the line to another array*/
        tok = strtok(row, delim);/* gets the first token*/
        /* if there is an .entry somewhere in the line*/
        if(strstr(line,".entry") != NULL){
            /* if there is a label prior to that*/
            if(strcmp(tok, ".entry") != 0){
                tok = strtok(NULL, delim);
            }
            tok = strtok(NULL, delim);/*gets the valid label name*/
            /*if the label name does not appear in the symbolChart */
            ptr = strstr((*symbolChart), tok);
            if(ptr == NULL){
                printf("The .entry label name does not appear in the file, in line %d\n", linCount);
                boolean = 1;
                continue;
            }
            /*if we found the label in the symbol chart*/
            else{ 
		/* moves over the symbol chart*/
		for(k = 0; k < sySize-strlen(tok); k++){ 
		    /* if we found the right label name*/
		    if(strncmp((*symbolChart)+k,tok,strlen(tok)) == 0) break; 
		    while((*symbolChart)[k] != '#') k++;/* moves to check the next label*/

		}
                /* goes over the symbol chart to find the designation */  
                for(i = 0; i < 2; i++){
                    /* goes over the $*/
                    while((*symbolChart)[k] != '$'){
                        k++;
                    }
		    k++;/* moves from the $*/ 
                }
                /*if the .entry label already appears as an .extern label*/
                if(strncmp((*symbolChart)+k, "external", 8) == 0){
                    printf("The .entry label already appears as .extern label, in line %d\n", linCount);
                    boolean = 1;
                    continue;  
                }
                k += 3;/* moves to the last char of data\code */
		memcpy((*symbolChart) + k, "1",1); /* this will be used for signifying an entry label name*/
                flagEnt = 1;/* sets the flag to 1*/
            }
            continue;
        }
        /* if it's a label name skip it */
	    if(strchr(tok, ':') != NULL){
            tok = strtok(NULL, delim);
        }
        /*checks if it's instruction with at least one operand */
        for(i = 0; i < 19; i++){
            if(strcmp(tok, name[i]) == 0){
                break;
            }
        }
        /* if its an instruction with at least one operand*/
        if(i < 19){
            tok = strtok(NULL, "");/*skip to get the operands*/
            for(i = 0; i < 2 && boolean == 0 ; i++){   
                /* skips tabs and spaces*/
                while(isspace(*tok)) tok++;
                /* if its an instruction that's has only one operand*/
                if(*tok == '\0'){
                    break; 
                }
                k = 0;
                /* moves until it possibly encounters a label*/
                while(*tok != ',' && *tok != '[' && !isspace(*tok) && *tok != '\0'){
                    store[k++] = *tok; /* stores a char*/
                    tok++;
                }

                store[k] = '\0'; /* null terminator*/
                /* if we reached a matrix address bracket*/
                if(*tok == '['){
                    /*goes over until the end of the brackets*/
                    for(k = 0; k < 2; k++){
                        /* goes over until it reaches an ']'*/
                        while(*tok != ']') tok++;
                        tok++;/* moves over from the ']'*/
                        /* if its the first [], moves from the second '['*/
                        if(k== 0) tok++;
                    }
                    /* skips spaces to reaches the possible next operand*/
                    while(isspace(*tok)) tok++;
                }
                /* moves over from the comma*/
                if(*tok == ',') tok++;
                /*search for the label name*/
                ptr = strstr((*symbolChart), store);
                /* if it's a label name */
                if(ptr != NULL){
                    /* goes over the $*/
                    while(*ptr != '$'){
                        ptr++;
                    } 
                    ptr += 2;/*move from the $ and '0'*/
                    /* copies the address*/
                    memcpy(binary, ptr, 3);
                    binary[3] = '\0';/* null teminates the array*/
                    ptr += 4;/*moves to the label type*/
                    /* copies the type to an array*/
                    memcpy(tempArr, ptr, 4);
                    tempArr[4] = '\0';/* null teminates the array*/
                    /* moves over the insIm until it reaches the label (?)*/
                    while((*insIm)[passInst] != '?'){
                        passInst++;
                    }
                    if(strcmp(binary, "$ex") == 0){
                        /* repalce the ?'s to the label address*/
                        memcpy((*insIm) + passInst, "0000000001",10);
                        passInst -= 4;/* gets the address*/
                        tempArr[0] = '$';/* insert $ to the 1st index*/
                        /* copies the address and seperate by $*/
                        memcpy(tempArr + 1, (*insIm) + passInst, 3);
			tempArr[4] = '\0'; /* null terminator*/    
                        /* stores the extern label name in the extern array*/
                        if(storeInArray(&cap, &index, &extArr, store, strlen(store),0 ,1) == 1){
                            boolean = 1;
                            continue;
                        }
                        /* stores the address into the extern array*/
                        if(storeInArray(&cap, &index, &extArr, tempArr, 4,0 ,1) == 1){
                            boolean = 1;
                            continue;
                        }
    			/* stores @ to seperate the code instructions*/
        		if(storeInArray(&cap, &index, &extArr, "@", 1,0 ,1) == 1){
            		    return 1;
        		}   
    
                        passInst += 14;/* moves to get next line*/
                    }
		    /* if it's not an extern label*/
                    else{
                        /* convert the number to  binary*/
                        conBinUn(binary);
                        strcat(binary, "10"); /* adds the A R E*/
                        /* repalce the ?'s to the label address*/
                        memcpy((*insIm) + passInst, binary ,10);
                        passInst += 10; /* pass over the label instruction */
                    }
                }
            }
            /* move to the end of this instruction */
            while((*insIm)[passInst] != '@'){
                passInst++;
            }
            passInst++; /* move to next instruction*/
            continue;
        }
        /* if its data, mat string or extern*/
        continue;
    }
   
    /* if an error was found in the file*/
    if(boolean == 1){
        free(extArr);
        return 1;
    }
    /* if there is an extern in the file*/
    if(index > 0){
        /* creates the name of the extern file*/
        strcpy(storeName, fileName);
        strcat(storeName, ".ext");
        extFile = fopen(storeName, "w"); /* open file for writing */
        if (extFile == NULL) {/* if can't open the extern file,prints error*/
            perror("Error opening file");
            return 1;
        }
        /* prints the extern instructions*/
        for(i = 0; i < index; i += 4){
            /* copies the label name to binary*/
            for(cap = 0; extArr[i] != '$'; cap++, i++){
                binary[cap] = extArr[i];
            }
	    binary[cap] = '\0';/* null terminator*/
            fprintf(extFile,"%s\t" ,binary); /* print the label to the file*/
            i ++; /* increase the index of extArr*/
            memcpy(binary,&extArr[i], 3);/* copy the binary*/
	    binary[3] = '\0';/* null terminator*/
            conDecFour(binary, -1);/* convert the address to base 4*/
            fprintf(extFile,"%s\n" ,binary);/* print to the file*/
        }
        fclose(extFile); /* close the file*/
    }
    /*if there is an entry label in the file */
    if(flagEnt == 1){
        /* creates the name of the entry file*/
        strcpy(storeName, fileName);
        strcat(storeName, ".ent");
        entFile = fopen(storeName, "w"); /* open file for writing */
        if (entFile == NULL) {/* if can't open the entry file,prints error*/
            perror("Error opening file");
            return 1;
        }
        /* prints the extern instructions*/
        for(i = 0; i < sySize; i++){
            /* copies the label name to binary*/
            for(cap = 0; (*symbolChart)[i] != '$'; cap++, i++){
                binary[cap] = (*symbolChart)[i];
            }
	    binary[cap] = '\0';/* null terminator*/
            i += 2; /* increase the index of extArr*/
            memcpy(tempArr,&(*symbolChart)[i], 3);/* copy the address*/
	    tempArr[3] = '\0'; /* null terminator*/
            i += 7;/* moves to the type*/
            /* if it's an entry type label*/
            if((*symbolChart)[i] == '1'){
                fprintf(entFile,"%s\t" ,binary);/* print the label to the file*/
                conDecFour(tempArr, -1);/* convert the address to base 4*/
                fprintf(entFile,"%s\n" ,tempArr);/* print the address to the file*/
            }
            /*moves to the end of the file*/
            while((*symbolChart)[i] != '#'){
                i++;
            }
        }
        fclose(entFile); /* close the file*/
    }
    /* creates the output file */
    strcpy(storeName, fileName);
    strcat(storeName, ".ob"); /* creates the name of the output file*/
    assemFile = fopen(storeName, "w"); /* open file for writing */
    if (assemFile == NULL) {/* if can't open the output file,prints error*/
        perror("Error opening file");
        return 1;
    } 

    /* convert ICF and DCF to base 4 and prints them to the file*/
    conDecFour(binary, IC);
    fprintf(assemFile,"%s\t" ,binary);
    conDecFour(binary, DC);
    fprintf(assemFile,"%s\n" ,binary);

    /* copies the instruction image to the file*/
    for(i = 1; i < insSize; i += 12){   
        memcpy(binary,&(*insIm)[i], 3);/* copy the address*/
	binary[3] = '\0'; /* null terminator*/
        conDecFour(binary, -1); /* convert the address to base 4*/
        fprintf(assemFile,"%s\t" ,binary); /* print to the file*/
        i+= 4; /* increase the index of insIm*/
        memcpy(binary,&(*insIm)[i], 10);/* copy the binary*/
	binary[10] = '\0'; /* null terminator*/
        conBinFour(binary);
        fprintf(assemFile,"%s\n" ,binary);/* print to the file*/
   
    }
    /*copies the data image to the file */
    for(i = 1; i < daSize; i += 12){
        memcpy(binary,&dataIm[i], 3);/* copy the address*/
	binary[3] = '\0'; /* null terminator*/
        conDecFour(binary, -1); /* convert the address to base 4*/
        fprintf(assemFile,"%s\t" ,binary); /* print to the file*/
        i+= 4; /* increase the index of insIm*/
        memcpy(binary,&dataIm[i], 10);/* copy the binary*/
	binary[10] = '\0'; /* null terminator*/
        conBinFour(binary);
        fprintf(assemFile,"%s\n" ,binary);/* print to the file*/
    }
    fclose(assemFile);/* close the file*/
    free(extArr);/* frees the extern array*/
    return 0;
}


