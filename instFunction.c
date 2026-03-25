#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "data.h"



/*checks instruction if there is any error and store it to the insIm*/
int checkInst(char *tok, int linCount, int *IC, char **insIm, int *insCap, int *insSize, int boolean, char **name){
    int maxIns = 16; /* max instruction*/
    char binary[11]; /* stores the converted number to bits*/
    int i; /* index for the loop*/
    int j = 0;/* index for loop checking register names and for index*/
    char *delim= " \t,"; /* delimeter for strtok*/
    char *word; /* stores the instruction from strtok*/
    double val;/* stores number */
    char * endNum; /*pointer used in strtod */
    int count; /* count the times for the for loop*/
    int prevSize, curSize, index = 0; /* integers to the available space in the instruction image*/ 
    char storeInst[64];/* used to stores the instruction temporarily*/
    char tempRow[80];/* will be used to store the line for the register*/
    char labeName[32]; /* will be used to store the assumed label name*/
    int isShtrudel = 1; /* flag for checking if to store # symbol*/
    int firRg = 0; /* 0 if no register was entered */
    char tempTok[80]; /* stores copy the row */
    /* This is the instructions chart which will be used in helping to decipher instructions and find possible errors in the code when going over the code file during the first pass of the assembler */
  instOpcode instChart[] = { {"mov", "0000", "0123", "-123"},
  {"cmp", "0001", "0123", "0123"},
  {"add", "0010", "0123", "-123"},
  {"sub", "0011", "0123", "-123"}, 
  {"lea", "0100", "-12-", "-123"},
  {"clr", "0101", "----", "-123"},
  {"not", "0110", "----", "-123"},
  {"inc", "0111", "----", "-123"}, 
  {"dec", "1000", "----", "-123"},
  {"jmp", "1001", "----", "-123"}, 
  {"bne", "1010", "----", "-123"},
  {"jsr", "1011", "----", "-123"},
  {"red", "1100", "----", "-123"},
  {"prn", "1101", "----", "0123"},
  {"rts", "1110", "----", "----"},
  {"stop", "1111", "----", "----"} };
    strcpy(tempTok, tok); /* copies the line to tempTok*/
    word = strtok(tempTok, delim); /*gets the instruction name*/
    /* if it's one of the data declarations that was not seperated by a space*/
    if(strstr(word,".mat") != NULL || strstr(word,".string") != NULL || strstr(word,".data") != NULL){
        printf(" The data instruction does not contain a space following the data declaration, in line %d\n",linCount);
	return 1;
    }
    for(i = 0; i < maxIns+1; i++){
        /* checks if the instruction name is valid */
        if(i < maxIns){
            /* if its one of the instruction names*/
            if(strcmp(instChart[i].ins, word) == 0){
                break;
            }
        }
        /* if the name of the instruction is invalid */
        else{
            printf("The instruction name is invalid, in line %d\n",linCount);
            return 1;
        }
    }
    /* if there was no error in the file*/
    if(boolean == 0){
        /* stores the bits in the temporary array*/
        memcpy(&storeInst[index],instChart[i].op, 4);
        index += 4;/* increases to move over the op code*/
    }
    /* if tok has spaces it skips them*/
    while(*tok != '\0' && isspace(*tok)){
        tok++;
    }
    tok += strlen(word); /* gets the rest of the row */

    /*goves over the line to parse the operands */
    for(count = 0; count < 2; count++){
        /* skips over tabs and spaces*/
        while(*tok != '\0' && isspace(*tok)){
            tok++;
        }
        if(*tok == '\0'){
            break;
        }
        /* if its before the sec operand*/
        if(count == 1){
            /* checks if there is comma between the 2 operands*/
            if(*tok != ','){
                printf("The operands are not seperated by comma, in line %d\n", linCount);
                return 1;
            }
            tok++;/* moves over the comma */
        }
	/* if there is a comma after the instruction name*/
	if(count == 0 && *tok == ','){
	    printf("The line contains a comma after the instruction name, in line %d\n",linCount);
	    return 1;
	}
        /* skips over tabs and spaces after the comma if it has*/
        while(isspace(*tok)){
            tok++;
        }
        /* for checking if it is register in the else if*/
        strcpy(tempRow, tok); /* copy the row before the use of strtok*/
        /* stop when there is space, tab or comma*/
        word =strtok(tempRow,delim);
        /* if its a direct addressing method*/
        if(*tok == '#'){
            tok++;/* moves over the # */
            /*if the # symbol is not followed by an integer*/
            if(*tok != '-' && *tok != '+' && !isdigit(*tok)){
                printf("The immediate address '#' isn't followed by a number, in line %d\n", linCount);
                return 1;
            }
            /* parses the assumed number into val */
            val = strtod(tok, &endNum);
            /* checks if the number is integer*/
            if(parseInt(tok, endNum, linCount, "instruction") == 1){
                return 1;
            }
            tok = endNum;/*tok point to the rest of the instruction*/
            if(count == 0 && instChart[i].frstOp[0] != '0' ){
                /* if the reason for incompatibleness is from mismatch with the first operand's address types*/
            /* if the address type is incompatible with the instruction's designated address type */
                if(strcmp(instChart[i].frstOp, "----") != 0){
                    printf("The immediate address type is incompatible with the source operand, in line %d\n",linCount);
                    return 1;
                }
                count++; /* checks in the sec operand*/
            }
            /* if the reason for incompatibleness is from lack if an existing 1st operand + mismatch with the destination operand's address types*/
            if(count == 1 && instChart[i].scndOp[0] != '0'){
                /* if it doesn't support this specific address method*/
                if(strcmp(instChart[i].scndOp, "----") != 0){
                    printf("The immediate address type is incompatible with the destination operand, in line %d\n",linCount);
                    return 1;
                }
                /* if it's an instruction that is supposed to have no operands but has extra chars in the line */
                else{
                    printf("The instruction is not allowed to contain any operands, in line %d\n",linCount);
                    return 1;
                }
            }
            
            /* insert to the first operand if there was no error in the file*/
            if(count == 0 && boolean == 0){
                /* stores the bits of 1st operand method in the temporary array*/
                memcpy(&storeInst[index],"00", 2);
                index += 2;/* moves to the 2nd operand address method*/
                /* prevsize is the index to store the next 4 bits */
                prevSize= index;
                index += 4;/* moves to the next memory line*/
                /* convert the number to bits*/
                if(conBaseTwo("", (int)val, binary, linCount, 7) == -1){
                    return 1;
                }
                strcat(binary,"00"); /* add the A R E bits*/
                /* stores the memory line in the temporary array*/
                memcpy(&storeInst[index],binary, 10);
                index += 10;/* moves to the next memory line*/
                curSize = index;/* curSize stores the current index */
            }
            /* insert to the second operand*/
            if(boolean == 0 && count == 1){
                /* if the instruction does not contains first operand,then it stores the address method of 2 operands+ A R E*/
                if(strcmp(instChart[i].frstOp,"----") == 0){
                    memcpy(&storeInst[index],"000000", 6);
                    index += 6;/* moves to the next memory line */
                }
                /*if its the sec operand*/
                else{
                    index = prevSize;/*  index stores the last 4 bits of 1st mem word*/
                    /* stores the sec operand + A R E*/
                    memcpy(&storeInst[index],"0000", 4);
                    index += 4;/* moves to the next memory line*/
                    index = curSize;/* index for the next memory word*/
                }
                /* convert the number to binary*/
                if(conBaseTwo("", (int)val, binary, linCount, 7) == -1){
                    return 1;
                }
                strcat(binary,"00"); /* add the A R E bits*/
                memcpy(&storeInst[index],binary, 10);
                index += 10;/* moves to the next memory line*/
            }
        }
        /* if its a legal register*/
        else if(checkReg(word, 1) == 0){
            /* checks if the address method of the source operand can be a register */
            word++; /* word contains now only the number of the register*/
            /* if it's the first operand and it doesn't support the register address method*/
            if(count == 0 && instChart[i].frstOp[3] != '3'){
                /*if it doesn't support the register address method*/
                if(strcmp(instChart[i].frstOp,"----") != 0){
                    printf("The register address type is incompatible with the source operand, in line %d\n",linCount);
                    return 1;
                }
                count++; /* checks in the sec operand*/
            }
            /*if we inserted the first operand*/
            if(count == 0 && boolean == 0){
                firRg = 1;
                /* store the address method of the register*/
                memcpy(&storeInst[index],"11", 2);
                index += 2;/* moves to the next memory line*/
                /* save 4 bits to store the adress type of the sec opernad and the bits of A R E*/
                prevSize = index;
                index += 4;
                /* convert the number to bits*/
                if(conBaseTwo(word, -1, binary, linCount, 3) == -1){
                    return 1;
                }
                /* stores the number + 6 0's in the instruction image indicating only one register(so far) */
                strcat(binary,"000000");
                memcpy(&storeInst[index],binary, 10);
                index += 10;/* moves to the next memory line*/
                /* curSize keeps the index */
                curSize = index;
            }
            /* the insert of the second operand*/
            if(boolean == 0 && count == 1){
                /* checks if the address method of the dest operand can be a register*/
                if(instChart[i].scndOp[3] != '3'){
                    /* if it's an instruction that is supposed to have no operands but has extra chars in the line */
                    printf("The instruction is not allowed to contain any operands, in line %d\n",linCount);
                        return 1;
                }
                /* if both operand are register*/
                if(firRg == 1){
                    /*insSize stroes the index of prevSize */
                    index = prevSize;
                    /* stores the address method + A R E*/
                    memcpy(&storeInst[index],"1100", 4);
                    /* idex stores the index of the sec register number */
                    index = curSize-6;
                    /* convert the number to bits*/
                    if(conBaseTwo(word, -1, binary, linCount, 3) == -1){
                        return 1;
                    }
                    /* stores the register number in the instruction image*/
                    memcpy(&storeInst[index],binary, 4);
                    index += 6;/* moves from the next register and skip the A R E bc it was stored before */
                }
                else{
		    /* if it's an instruction that has no source operand*/
		    if(strcmp(instChart[i].frstOp,"----") == 0){
		       /* stores the address method + A R E*/
                       memcpy(&storeInst[index],"001100", 6);
                       index += 6;/* moves to the next memory line*/
		    }
		    else{
                       /*index stores the index of prevSize */
                       index = prevSize;
                       /* stores the address method + A R E*/
                       memcpy(&storeInst[index],"1100", 4);
                       index += 4;/* moves to the next memory line*/
                       /* index stores the index of the sec register number */
                       index = curSize;
		    }
                    /* stores 4 0's indicating that  only the second operand is a register*/
                    memcpy(&storeInst[index],"0000", 4);
                    index += 4;/* moves to the A R E of register number*/
                    /* stores 4 0's indicating that  only the second operand is a register*/
                    /* convert the  register number to bits*/
                    if(conBaseTwo(word, -1, binary, linCount, 3) == -1){
                       return 1;
                    }
                    strcat(binary, "00"); /* adds the A R E to the register number*/
                    /* stores the number + A R E in the instruction image */
                    memcpy(&storeInst[index],binary, 6);
                    index += 6;/* moves to the next memory line*/
                }
            }
            tok += 2; /* the pointer increases until the end of the register number */
        }
        /* if its a valid label name, we can check address method used with labels */
        else if(isalpha(*tok)){
            /*copies next 30 chars and stops if they are followed by ',' or '[' or tabs and spaces, or if we reached the end, to check for label */
            j = 0;
            while((j < 31 && *tok != ',' && !isspace(*tok) && *tok != '[') && *tok != '\0'){
                labeName[j++] = *tok;/* stores the current char*/
                tok++;
            }
            labeName[j] = '\0';
            /* if its not a valid label name, argument is not a defined argument and will lead to an error */
            if(checkValidName(labeName,1) == 1){
                printf(", in line %d\n",linCount);
                return 1;
            }
            /* goes over the 29 invalid label names*/
            for(j = 0; j < 29; j++){
                /* checks if the label name is one of the invalid saved names of the language*/
                if(strcmp(labeName, name[j]) == 0){
                    printf("The label name is an illegal saved word of the language, in line %d\n", linCount);
                    return 1;
                }
            }
            /* if it is assumed to be a matrix access address method */
            if(*tok == '['){
                /* if the address method is incompatible with the operand type then it could be the sec operand only or method without operands */
                if(count == 0 && instChart[i].frstOp[2] != '2'){
                    count++; /* checks in the sec operand*/
                    /* stores zero's for the address method of the first operand */
                    if(boolean == 0){
                        /* store the method address of the 1st operand */
                        memcpy(&storeInst[index],"00", 2);
                        index += 2;/* moves to the next operand */
                        /* prevSize stores the index */
                        prevSize = index;
                        curSize = index+4;/*curSize increases to safecheck for overflow*/
                    }
                }
                /* stores the address method for the matrix access type */
                if(count == 0 && boolean == 0){
                    memcpy(&storeInst[index], "10", 2);
                    /* prevSize stores the index of next operand */
                    prevSize = index +2;
                    index += 6;/* moves to the next memory line*/
                }
                /* if it is sec operand*/
                if(count == 1){
                    /* all the instructions could contains matrix except for the instructions thats have no destination of operand*/
                    if(strcmp(instChart[i].scndOp,"----") == 0){
                        printf("The instruction is not supposed to contain any operand, in line %d\n", linCount);
                        return 1;
                    }
                    if(boolean == 0){
                        /* stores the address method for the matrix access type */
                        index = prevSize;
                        memcpy(&storeInst[index],"1000", 4);
                        /* index stores the index of the next memory word*/
                        index = curSize;
                    }
                }
                /* moves over the matrix twice */
                for(j = 0; j < 2 ; j++){
                    /* if in the second time there is no opening bracket */
                    if( j == 1 && *tok != '['){
                        printf("The matrix was not declared properly, in line %d\n", linCount);
                        return 1;
                    }
                    tok++; /* move from the [ */
                    /* if we have space between the parentheses to the number*/
                    while(isspace(*tok)){
                        tok++;
                    }
                    /*checks if its register */
                    if(checkReg(tok, 0) == 1){
                        printf("The matrix contains illegal values, in line %d\n", linCount);
                        return 1;
                    }
                    tok++; /* moves to the register number*/
                    /* if it's the first brackets of the matrix */
                    if(j == 0 && boolean == 0){
                        /*  stores the address type of the label name and the bits of A R E*/
                        memcpy(&storeInst[index],"??????????", 10);
                        index += 10;/* moves to the next memory line*/
                        /* convert the number to bits*/
                        if(conBaseTwo(tok, -1, binary, linCount, 3) == -1){
                            return 1;
                        }
                        /* stores the row register number in 9-6 bits*/
                        memcpy(&storeInst[index],binary, 4);
                        index += 4;/* moves to the col register number bits*/
                    }
                     /* if it's the second brackets of the matrix */
                    if(j == 1 && boolean == 0){
                        /* convert the number to bits*/
                        if(conBaseTwo(tok, -1, binary, linCount, 3) == -1){
                            return 1;
                        }
                        strcat(binary,"00");/* adds the A R E to the binary*/
                        /* stores the column register number in 5-2 bits*/
                        memcpy(&storeInst[index],binary, 6);
                        index += 6 ;/* moves to the next memory line*/
                        curSize= index; /*cursize stores the index of the next memory word */
                    }
                    tok++; /* moves from the register number */
                    /* if we have space between the number and the parentheses */
                    while(isspace(*tok)){
                        tok++;
                    }
                    /*if there is no closing bracket */
                    if(*tok != ']'){
                        printf("The matrix was not declared properly, in line %d\n", linCount);
                        return 1;
                    }
                    tok++; /* moves from the ] */
                }
            }
            /* if it's a direct address(label) method */
            else{
                /* if the address method is incompatible with the operand type then it could be the sec operand*/
                if(count == 0 && instChart[i].frstOp[1] != '1'){
                    count++;/* checks in the sec operand*/
                    /* if there was no error in the file*/
                    if(boolean == 0){
                        /* stores zero's for the address method of the first operand */
                        memcpy(&storeInst[index],"00", 2);
                        index += 2;/* moves to the next operand method*/
                        /* prevSize stores the index */
                        prevSize = index;
                        curSize = index + 4; /*curSize increases to safecheck for overflow*/
                    }
                }
                /* stores the address method of the label */
                if(count == 0 && boolean == 0){
                    memcpy(&storeInst[index],"01", 2);
                    /* saves spaces for the last 4 bits of the first word */
                    prevSize = index + 2;
                    index += 6;
                }
                /* if it is sec operand*/
                if(count == 1){
                    /* all the instructions could contain label except for the instructions thats have no destination of operand*/
                    if(strcmp(instChart[i].scndOp, "----") == 0){
                        printf("The instruction contains extra operands, in line %d\n", linCount);
                        return 1;
                    }
                    if(boolean == 0){
                        index = prevSize;
                        /* stores the address method of the sec operand + A R E */
                        memcpy(&storeInst[index],"0100", 4);
                        index += 4;/* moves to the next memory line*/
                        /* insSize stores the index of the next memory word*/
                        index = curSize;
                        memcpy(&storeInst[index],"??????????", 10);
                        index += 10;/* moves to the next memory line*/
                    }
                }
            }
        }
        /* if there are invalid characters in the line where the operands should be */
        else if (*tok != '\0'){ 
            printf("The instruction contains undefined operands, in line %d\n", linCount);
            return 1;
        }
    } /*end of the loop for */
    
    /* skips over tabs and spaces*/
    while(*tok != '\0' && isspace(*tok)){
        tok++;
    }
    /* if there are illegal characters after the end of the operand */
    if(*tok != '\0'){
        printf("The instruction contains extra characters after valid operands, in line %d\n", linCount);
        return 1;
    }
    /* if we reached the end of the line prematurely */
    if(count < 2){
        /* if it's any of the instructions that have in their syntax at least 1 operand */
        if(i < 14){
            /* if it's an instruction that should have in its syntax 2 operands */
            if(i<5){
                /* if the instruction did not have any operands in the line*/
                if(count == 0){
                    printf("The instruction does not have any operands, in line %d\n", linCount);
                    return 1;  
                }
                /* if the instruction did not have dest operand in the line*/
                else{
                    printf("The instruction does not have destination operand and a comma, in line %d\n", linCount);
                    return 1;  
                }
            }
            /* if it's an instruction that should have a destination operand */
            else{
                /* if the instruction did not have dest operand in the line*/
                if(count == 0){
                    printf("The instruction does not have destination operand, in line %d\n", linCount);
                    return 1;
                }
            }
        }
    }
    /* if it's an instructions that's not supposed to have any operands*/
    if(i > 13 && boolean == 0){
        memcpy(&storeInst[index],"000000", 6);
        index += 6;/* moves to the next memory line*/
    }
    storeInst[index] = '\0';/* null terminate the temporary array*/
    /* goes over the temprary array with the stored inst lines if the boolean is 1 it wont write to the temprary storeInst and it will skip this for*/
    for(curSize = 0 ; curSize < index ; curSize += 10){
        memcpy(binary, &storeInst[curSize], 10);
        /* stores # before the beginning of the line*/
        noErrStore(&(*insIm), insCap, insSize, IC, &isShtrudel);
        /* stores the memory word*/
        if(storeInArray(insCap, insSize, &(*insIm), binary, 10,*IC+100
        ,0) == 1){
            return 1;
        }
        (*IC)++;/* increases to the next word*/
    }
    /* if there was no error in the file*/
    if(boolean == 0){
        /* stores @ to seperate the code instructions*/
        if(storeInArray(insCap, insSize, &(*insIm), "@", 1,*IC ,1) == 1){
            return 1;
        }   
    }
    return 0;
 }
 
 
 
