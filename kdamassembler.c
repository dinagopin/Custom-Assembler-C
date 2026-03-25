#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"


int check_mcro(FILE* readFile, FILE* writeFile, char *fileName,  char **macro, char **name, int *cap, int *size){
    char *temp;/*unintialized pointers for malloc*/
    char *word;/*unintialized pointer for strtok*/
    char *delim = " \t";/* special pointer to skip over characters*/
    char row[MAX_CHAR];/*array to store an input line */
    char tempRow[MAX_CHAR]; /*temporary array to store an input line to use strtok for pointer word */
    char savedWord[35];/* used to combine a macro name and : into one string*/
    int insEnd=31;/* constant varibale as a counter for the name array */
    int i; /*use for loop */
    size_t len;/*size of line from input */
        
        /* as long as the file does not reach the end*/
    while(fgets(row,MAX_CHAR,readFile) != NULL){
        row[strcspn(row, "\n")] = '\0';/*remove the newline character*/
        len=strlen(row);
        if(len>80){ /* if therare more than 80 chars in a given row */
            puts("The line exceeds 80 characters");
	    fclose(writeFile);
	    remove(fileName);
            return 1;
        }
        strcpy(tempRow, row);/* copies the input into another array */
        word = strtok(tempRow, delim);/*skips tabs and white spaces*/
        
        /*checks if it is an empty line*/
        if(word == NULL){
            continue;
        }
        
        /*checks mcro */
        if(strcmp(word, "mcro") == 0){/* if the line has the word 'mcro' in it */
            word = strtok(NULL, delim);/*skips tabs and white spaces*/
            if(word == NULL){/* if the 'mcro' line containes no macro name */
                puts("no mcro name was typed");
		fclose(writeFile);
	        remove(fileName);
                return 1;
            }
            for(i=0;i<insEnd;i++){/* checks if the macro name is of the 31 invalid words */
                if(strcmp(word, name[i]) == 0){
                    puts("invalid macro name");
		    fclose(writeFile);
	            remove(fileName);
                    return 1;
                }
            }
            strcpy(savedWord, word);/* copies the inputs into a 'savedWord' array */
            /* if the macro name isn't valid */
            if(checkValidName(word, 0) == 1){
                printf("\n");
		fclose(writeFile);
	        remove(fileName);
                return 1;
            }
	    /* if there is an existing macro by the same name */
            if(checkDupMac(*macro, *size, word) == 1){
	        printf("\n"); 
		fclose(writeFile);
	        remove(fileName);
                return 1;
	    }
            word = strtok(NULL,delim);/*skips over white tabs */
            /* if there are invalid chars following a valid macro name */
            if(word!=NULL){
                puts("extra words after the mcro name");
		fclose(writeFile);
	        remove(fileName);
                return 1;
            }
            /* checks if there is sufficient space in the macro array to store 33 characters of macro name + the # symbol, representing the end */
            if(*cap <= *size+33){
                *cap = *cap*2;
                temp = (char*) realloc(*macro, *cap*sizeof(char));/*allocates additional dynamic 
		space for the macro chart */
                if(!temp){/* if the dynamiic allocation failed */
                    puts("realloc failed");
                    free(macro);
		    fclose(writeFile);
	            remove(fileName);
                    return 1;
                }
            *macro = temp;/* assigns the dynamic macro array to the dynamic temp array */
            }
            /*copies the macro name into the macro chart*/
            i=0;
            while(isalpha(savedWord[i]) || isdigit(savedWord[i]) || ((savedWord[i]) == '_')){
                (*macro)[(*size)++] = savedWord[i];
                i++;
            }
            (*macro)[(*size)++]='#';/*seperates a mcro name from the instruction */
            /*copy all the instructions in the mcro */
            while(fgets(row,MAX_CHAR,readFile) != NULL){
                row[strcspn(row, "\n")] = '\0';/*remove the newline character*/
                len=strlen(row);
                if(len>80){/* if the line has more than 80 chars */
                    puts("The line exceeds 80 characters");
		    fclose(writeFile);
	            remove(fileName);	
                    return 1;
                }
                word = strtok(row, delim);/*skips tabs and white spaces*/
                /*checks if it is an empty line*/
                if(word == NULL){
                    continue;
                }
                
                /* if the string 'mcroend' exists in the input line */
                if(strcmp(word, "mcroend") == 0){
                    word=strtok(NULL,delim);/*skips over white tabs */
                    if(word!=NULL){/* extra chars following macroend */
                        puts("extra chars after 'mcroend'");
			fclose(writeFile);
	                remove(fileName);
                        return 1;
                    }
                    (*macro)[(*size)++]='@'; /*seperates one mcro from another */
                    break;
                }
                /*32 chars are a sufficient amount to store one line of an instruction + an end symbol */
                if(*cap <= *size+len+2){
                    *cap = *cap*2;
                    temp = (char*) realloc(*macro, *cap*sizeof(char));/*allocates additional dynamic space for the macro chart */
                    if(!temp){/* if the dynamiic allocation failed */
                        puts("realloc failed");
                        free(macro);
			fclose(writeFile);
	                remove(fileName);
                        return 1;
                        
                    }
                    *macro = temp;/* assigns the dynamic macro array to the dynamic temp array */
                }
                /*seperates by a tab if its not label at begining of the line*/
                if(!isupper(*word)){
                        (*macro)[(*size)++] = '\t';
                }
                /* goes over an instruction line*/
                while(word != NULL){
                    (*macro)[(*size)++] = '\t'; /*seperates an instruction name from its operands by a tab */
                    while(*word != '\0'){/* as long as didn't reach end of current operand */
                        (*macro)[(*size)++]=*word; /* copies current char of the instruction operand */
                        word++;
                    }
                    /*get the next operand*/
                    word = strtok(NULL, delim);
                }
                (*macro)[(*size)++] = '$';/*seperates one instruction from another */
                /*enters the internal 'while' loop again to get the next line inside the mcro designation */
                continue;
            }
            /* enters the outer 'while' loop to get the next line of the input */
            continue;
        }
        
        /*if it is a comment line */
        if(*word == ';'){
            continue;
        }
        
        /*combines mcro name and # */
        strcpy(savedWord, word);
        strcat(savedWord,"#");
        temp=strstr(*macro, savedWord);/* saves the revamped macro name back in the macro chart */
        /*if it is a macro command that appears in the macro chart*/
        if(temp != NULL){
            /* goes over the macro name until it reaches the saved symbol '#' indicating end of macro name */
            while(*temp != '#'){
                temp++;
            }
            temp++;/* skips over the '#' symbol */
            /* if it's not a mcroend, copies the instructions */
            while(*temp != '@'){
                if(*temp == '$'){
                    fputs("\n",writeFile);
                    temp++;
                } 
                else {
                    fprintf(writeFile,"%c",*temp);/* prints current macro instruction to the .am file */
                    temp++;
                }
            }
        }
        /*if it is a regular command */
        else{
            fprintf(writeFile,"%s\n",row);/* prints the command to the .am file */
        }
    }
    
    /*the macro array will be freed later on in the main file */
    return 0; 
}




