#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"


int checkValidName(char *word, int num){
    int nameEnd=30;/*variable indicating the legal length of a macro/label name */
    int i;/* variable declared to be used in the 'for' loop */
    char c;/* variable that will be used to store current char of the macro/label name */
    size_t len = strlen(word);/* counts the length of the actual macro/label name */
    char errName[8]; /* a string that stores if its a macro instruction or label */
    if(num==0) strcpy(errName,"macro");
    else strcpy(errName,"label");
    while(isspace(*word)){
       word++;
       len--;
    }
    /* if the macro/label name was not given */
    if(word == NULL || *word == '\0'){
        printf("%s name was not given",errName);
        return 1;
    }
    /* if the macro/label name is longer than the valid length */
    if(len > nameEnd){
        printf("%s name has too many characters",errName);
        return 1;
    }
    /* if the string name does not start with a letter*/
    if(!isalpha(*word)){
        printf("%s name does not start with a letter",errName);
        return 1;
    }
    /* checks if one of the chars in the macro/label name is invalid */
    for(i = 0; i < len; i++){
        c = word[i];

	/*if the current char is not a valid type of character*/    
        if(!isalpha(c) && !isdigit(c)){
	    /* if it's a macro name and it has a _, will move to check next char*/
	    if(c == '_' && num == 0) continue;
	    /* if we reached a space or tab*/
	    while(isspace(word[i])){
	        i++;
	        /* if it's the end(meaning a valid character)*/
	        if(word[i] == '\0') return 0;
	    }
            printf("%s name contains illegal characters",errName);
            return 1;
        }
    }
    return 0;
}

int checkDupLab(char *array, int size, char *tok){
    int i, j;/* index for the loops*/
    char store[31];/* stores the label name*/
    /* loop for going over the array*/
    for(i = 0; i < size; i++){
        /* stores the name from the array*/
        for(j = 0; array[i] != '$' && j < 31; j++, i++){
            store[j] = array[i];
        }
        store[j] = '\0';/* null terminates the array*/
        /* if the label name is equal to the current label name*/
	if(strcmp(tok, store) == 0){
            return 1;
        }
	/* moves until it reaches end of line((#)*/
	while(array[i] != '#'){
	    i++;
	}
    }
    return 0;
}

int checkDupMac(char *macro, int size, char *tok){
    char arr[31]; /* the max size for a macro name is 30 + one for the end (\0)*/
    int i, j;/* index for the loops*/
    /* loop for going over the array*/
    for(i = 0; i < size; i++){
        /* stores the name from the array*/
        for(j = 0; macro[i] != '#' && j < 31; j++, i++){
            arr[j] = macro[i];
        }
        arr[j] = '\0';/* null terminates the array*/
        /* if the label name is equal to the current label name*/
	if(strcmp(tok, arr) == 0){
	    printf("The macro name appears multiple times in the file(macro/label)");
            return 1;
        }
	/* moves until it reaches end of line((#)*/
	while(macro[i] != '#'){
	    i++;
	}
    }
    return 0;
}


int conBaseTwo(char *tok, int tarNum, char *binary, int linCount, int index){
    int number;
    unsigned int mask; /* used to store an unsigned integer*/
    int i; /* index for the array*/
    if(tarNum == -1){
        number = atoi(tok); /* convert the char to integer*/
    }
    else number = tarNum;
    
    /* if its not in the range for 2's complement signed numbers*/
    if (number > (1 << index) - 1 || number < -(1 << index)) {
        printf("the number is not in the valid range of %d bits, in line %d\n", index + 1, linCount);
        return -1;
    }
    /* convert to unsigned number*/
    mask = (unsigned int) number;
    /*stores the 1 or zero starting from the first index depending on the mask*/
    for(i = index; i >= 0; i--){
        binary[index - i] = (mask & (1U << i)) ? '1' : '0';
    }
    
    binary[index+1] = '\0'; /* index bits + the end */
    return 0;
}



int storeInArray(int *cap, int *size, char **array, char *binary, int maxLen, int counter, int val){
    char *temp;/* used for assignment */
    char cpyCounter[13]; /* copy the counter who was given to a pointer*/
    int x = 0; /* this variable will be used to determine the counter's length*/
  
    /* Checks if the IC/DC is bigger then 256. There is another check in the check_assembler_second*/
    if(counter >= 256){
        printf("Error: the number of instruction lines exceeds the memory size\n");
        return 1;
    }
    /* Checks the total length of the address(counter)*/
    if (val == 0) {
        if(counter<10){
            x=1;
        }
        if(counter<100){
            x=2;
        }
        else x=3;
        x+=2; /* for the $ symbol and to start with '0'*/
    }
    /* when its extern store checks if there is enough place to store for the 0's and $*/
    if(counter == -1){
        x = 2;
    }

    /* checks the sufficient amount to store  */
    if(*cap <= *size+maxLen + x){
        *cap = *cap*2;
        temp = (char*) realloc(*array, *cap*sizeof(char));/*allocates additional dynamic space for the fitting  array */
        /* if the dynamiic allocation failed */
        if(!temp){
            puts("realloc failed");
            return 1;
        }
        *array = temp;/* assigns the dynamic fitting array to the dynamic temp array */
    }
    /* if we need to copy the address*/
    if(val == 0){
       cpyCounter[0] = '0';/* starts the counter with 0*/
       /* Converts the counter into a string after the 0 if its not extern*/
       if(counter != -1){
          sprintf(cpyCounter + 1, "%d", counter);
       }
       memcpy(*array + *size, cpyCounter, x - 1); /* copies the counter into the fitting array */
       *size += x - 1;
       /*seperates the the counter from the number through $ symbol*/
       (*array)[(*size)++] = '$';
    }
    memcpy(*array + *size, binary, maxLen); /* copies to the matrix*/
    *size += maxLen;/* increases the size by the maxLen*/
    return 0;
}

int checkEntExt(char *tok, char *row, int linCount){
    char *ptr;/* pointer for the right character */
    /* points to the first char of the label name*/
    ptr= strstr(row,tok);
    /* skips until the end of the label name */
    while(*ptr != ':'){
        ptr++;  
    }
    ptr++;/*moves to the char following the label name */
    while(isspace(*ptr)){/* skips over tabs and spaces */
        ptr++;
    }
    
    /* if the next string in the line is ".entry" */
    if(strncmp(ptr,".entry",6) == 0){
        printf("Warning: the '.entry' instruction is preceded by a label, in line %d\n",linCount);
        return 1;
    }
    /* if the next string in the line is ".extern" */
    if(strncmp(ptr,".extern",7) == 0){
        printf("Warning: the '.extern' instruction is preceded by a label, in line %d\n",linCount);
        return 1;
    }
    return 0;
} 

int checkNumbers(char *tok, int number, int *DC, int matsize, char **dataIm, int *daSize, int *daCap, int boolean, int linCount){
    char array[6]; /*used to store the right string */
    char binary[11]; /* the number in binary*/
    int count = 0; /*counts the number of values given to the matrix*/
    char *endNum; /* point to the char after the number*/
    double val; /* used for storing the number of strtod*/
    int countNum = 0; /*counts the number times we entered the loop */
    int isShtrudel = 1;/* flag for checking if to store # or not*/
    if(number == 0){
        strcpy(array,".data"); /*if the error was at the .data part */
    }
    else{
        strcpy(array,".mat");/* if the error was at the .mat part */
    }
    while (*tok == ' ' || *tok == '\t'){/* skips tabs and spaces */
        tok++;
    }         
    /* if no values were given after a .data*/    
    if((*tok == '\0') && number == 0){
        printf(" No values were given for the .data instruction, in line %d\n", linCount);
        return 1;
    }
    /*goes over the remaining chars in the input line */
    while(1){
        /* if we reached the end of the line succesfully */
        if(*tok == '\0') break;
        /* if a comma is missing between arguments */
        if(countNum != 0 && *tok != ','){
            printf("The %s misses a comma between numbers, in line %d\n", array,linCount);
            return 1;
        }
        /* if we reached a ',' symbol */
        if(*tok == ','){
            /* if the ',' is before the first argument */
            if(countNum == 0){
                printf("The %s contains a comma before the first number, in line %d\n", array,linCount);
                return 1;
            }
            tok++;
            /* skips tabs and spaces */
            while (*tok == ' ' || *tok == '\t'){
                tok++;
            }
            /* if there is a ',' after the last argument */
            if((*tok == '\0')){ 
                printf("The %s contains a comma after the last number, in line %d\n", array,linCount);
                return 1;
            }
        }
        /* parses the assumed number into val */
        val = strtod(tok, &endNum);
        /* checks if the number is integer*/
        if(parseInt(tok, endNum, linCount, array) == 1){
            return 1;
        }
        tok = endNum;/* moves to the char follwing the end of this argument */
        while(isspace(*tok)){/*skips over tabs and spaces*/
            tok++;
        }
        
        /* if the number is out of range*/
        if(conBaseTwo("", (int)val,  binary, linCount, 9) == -1){
            return 1;
        }
        /* if it's a matrix (number == 1)*/
        if(number == 1 && count >= matsize){
            printf("the matrix contains too many values, in line %d\n", linCount);
            return 1;
        }
        
        if(boolean != 1){
            /* stores # before the beginning of the line*/
            noErrStore(&(*dataIm), daCap, daSize, DC, &isShtrudel);
            /* stores the address and number into the data image, seperated by a $ symbol*/
            if(storeInArray(daCap, daSize, &(*dataIm), binary, 10,*DC, 0) == 1){
                return 1;
            }
        }
        /* if we inserted the value of a matrix */
        if(number == 1){
            count++;
        }
        countNum++;/* counts the argument number */
        /*tok = endNum; moves to the char follwing the end of this argument */
        (*DC)++;/* increases the data counter by 1 */
    }   
    /* if some of the indices in the matrix were unintialised, inserts 0's*/
    while(number == 1 && count < matsize && boolean != 1){
        /* stores # before the beginning of the line*/
        noErrStore(&(*dataIm), daCap, daSize, DC, &isShtrudel); 
        /*stores 10 zeros in the data image  */
        if(storeInArray(daCap, daSize, &(*dataIm), "0000000000", 10, *DC, 0) == 1){
            return 1;
        }
        (*DC)++;/* increases the data counter by 1 */
        count++;
    }
    /* if there was no error in the file, seperates each data instruction by a @ symbol */
    if(boolean == 0){
        if(storeInArray(daCap, daSize, &(*dataIm), "@", 1,*DC, 1) == 1){
            return 1;
        }
    }
    return 0; 
}

int checkLabelVal(char *tok, int linCount, char *tempArr, int *DC, char **dataIm, int *daSize, int *daCap, int boolean){
    /* num1 num2 used for the size of the matrix*/
    int num1 = -1; /* num1 is -1 if nothing was entered yet*/
    int num2 = -1; /* num2 is -1 if nothing was entered yet*/
    char binary[11]; /* the number in binary*/
    double val; /* stores the number of the matrix size*/
    char *endNum; /* pointer to the next char after the number */
    int matSize; /*stores the size of the matrix(row*col) */
    int isShtrudel= 1;/*checks if to add a #*/
    int k = 0; /* increase the tok without pointer*/
    
    /* if the instruction is type of .data*/
    if(strcmp(tempArr, ".data") == 0){
        /* checks if the values are valid and if so inserts them to dataIm array*/
        if(checkNumbers(tok, 0, DC, -1, &(*dataIm), daSize, daCap, boolean,linCount) == 1){
            return 1;
        }
    }
   
    /* if the instruction is type of .mat*/
    else if(strcmp(tempArr, ".mat") == 0){
        /*check the size of the matrix */
        while(*tok != '\0' && num2 == -1){
            /* skip spaces or tabs if its before declaring matrix size*/
            if(num1 == -1){
                while(*tok == '\t' || *tok == ' '){
                    tok++;
                }
            }
            if(*tok != '['){
                printf("The matrix was not declared properly, in line %d\n", linCount);
                return 1;
            }
            tok++;/* move from the '['*/
            /* if we have space between the parentheses to the number*/
            while(isspace(*tok)){
                tok++;
            }
            
            val = strtod(tok, &endNum);
            /*checks if its not a negative number */
            if(*tok == '-'){
                printf("The matrix contains illegal matrix index, in line %d\n", linCount);
                return 1;
            }
            /* if its not a valid integer */
            if(parseInt(tok, endNum, linCount, ".mat") == 1){
                return 1;
            }
            /* checks if we didn't initialise the row variable*/
            if(num1 == -1){
                num1 = (int)val;
            }
            /* checks if we didn't initialise the column variable*/
            else{
                num2 = (int)val;
            }
            tok = endNum;/* tok points after the number*/
            /* if we have space between the number and the parentheses */
            while(isspace(*tok)){
                tok++;
            }
            if(*tok != ']'){
                printf("The matrix was not declared properly, in line %d\n", linCount);
                return 1;
            }
            tok++;/* moves from the '[*/
        }
        /* if some or non of the indexes was given*/
        if(num1 == -1 || num2 == -1){
            printf("No value for column or row index was given, in line %d\n", linCount);
            return 1;
        }
        matSize = num1*num2; /* the size of the matrix*/
        /* checks if the values are valid and if so inserts them to dataIm array*/
        if(checkNumbers(tok, 1, DC, matSize, &(*dataIm), daSize, daCap, boolean,linCount) == 1){
            return 1;
        }
    }
    /* check .string */
    else{ 
        if(isspace(*tok)){/* skips tabs and spaces*/
            tok++;
        }
        /* if the string did not start with " symbol*/
        if(*tok != '"'){
            printf("The string does not contain double quotes before the first character, in line %d\n", linCount);
            return 1;     
        }
	tok++;
        /* goes over the input */
        while(*tok !='\0'){
            /* if there is " symbol and it's not the first char*/
            if(*tok == '"'){
	         k = 1;
		 while(isspace(tok[k])){
		      k++;
		 }
		 if(tok[k] == '\0'){
		      break;
		 }
	    }
	    
            /* if the char is not ASCII*/
            if ((unsigned char)*tok > 127){
                printf("The string contains non-ASCII values, in line %d\n", linCount);
                return 1;  
            }
            /* converts the ASCII char into binary*/
            if(conBaseTwo("", (int)(unsigned char)*tok,  binary, linCount,9) == -1){
                return 1;
            }
            /* if there was no error in the file*/
            if(boolean == 0){
                /* stores # before the beginning of the line*/
                noErrStore(&(*dataIm), daCap, daSize, DC, &isShtrudel);
                /* store the line + the char representation(binary)*/
                if(storeInArray(daCap, daSize, &(*dataIm), binary, 10, *DC, 0) == 1){
                    return 1;                }   
            }
	    (*DC)++; 
	    tok++;
        }
        
        /* if the string ended without a '"' symbol right before \0*/
        if(*tok == '\0'){
            printf("The string does not contain double quotes marking the end of the string, in line %d\n", linCount);
                return 1;
        }
        /* if there was no error in the file*/
        if(boolean != 1){
	    /* stores # before the beginning of the line*/
            noErrStore(&(*dataIm), daCap, daSize, DC, &isShtrudel);
            /* store the line + the char representation(binary)*/
            if(storeInArray(daCap, daSize, &(*dataIm), "0000000000", 10, *DC, 0) == 1){
                return 1;
            } 
		(*DC)++; 
            /* seperates each data instruction by a @ symbol */
            if(storeInArray(daCap, daSize, &(*dataIm), "@", 1, *DC, 1) == 1){
                return 1;
            }
        }
    }
    return 0;
}

int checkReg(char *word, int type){
    char ptr[3];/* array to store the string */
    int i = 0; /* index for array*/

    /* if the string comes from matrix access method */
    if(type == 0){
        /* inserts the string inside the matrix brackets+ 1 to check if it's valid */
        while(i < 3 && *word != ']' && !isspace(*word)){
            ptr[i++] = *word; 
            word++;
        }
        /* inserts \0 at the end to prevent overflow  */
        ptr[i] = '\0';
    }
    else{
        /* if it was sent to check for register method but the length of it is longer than 2(r+ number)*/
        if(strlen(word) > 2){
            return 1;
        }
        strcpy(ptr,word);/* copies the assumed register into ptr*/
    }
    
    /*checks if it's a register or not */
    if(!strcmp(ptr, "r0") || !strcmp(ptr, "r1") || !strcmp(ptr, "r2") || !strcmp(ptr, "r3") || !strcmp(ptr, "r4") || !strcmp(ptr, "r5") || !strcmp(ptr, "r6") || !strcmp(ptr, "r7")){
        return 0;
    }

    else{
        return 1;
    } 
}

int noErrStore(char **image, int *cap, int *size, int *counter, int *isShtrudel){
    /* if it's not the first index in the data image*/
    if(*size > 0){
        /* if there was a @ symbol before this*/
        if(*isShtrudel != 1){
            /* will store# before storing the address*/
            if(storeInArray(cap, size, &(*image), "#", 1, *counter, 1) == 1){
                return 1;
            }
        }
        *isShtrudel = 0;/* clear the isShtrudel flag*/
    }
    /* if it's the first line*/
    if(*size == 0){
        *isShtrudel = 0;/* clear the isShtrudel flag*/ 
    }
    return 0;
}

int parseInt(char *tok, char *endNum, int linCount, char *array){
    /* if there is no number it will point to the same place*/
    if (tok == endNum){
        printf("The %s conains argument that is not a number, in line %d\n", array,linCount);
        return 1;
    }
    /* as long as the string didnt reach the end of the number or string */
    while(*tok != '\0' && tok != endNum){
        /* if the number is not an integer, finds if the string has a '.' in it */
        if(*tok == '.'){
            printf("The %s contains argument that is not an integer, in line %d\n", array,linCount);
            return 1;
        }
        tok++;
    }
    return 0;
} 


