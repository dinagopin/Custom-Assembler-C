#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"


/* functions*/
int conDecFour(char *store, int count){
    int num;/* used for casting*/
    unsigned int abc;/* used for base 4 character*/
    int index;/* saves the index until which we store*/
    int i;
    int quotient;/* variables that stores the quotient*/
    /* if its a string*/
    if(count == -1){
        num = atoi(store);/* cast to decimal*/
    }
    /* if it's a number*/
    else{
        num = count;
    }
    /* checks the right index range*/
    if(num < 4) i = 0;
    else if(num < 16) i = 1;
    else if(num < 64) i = 2;
    else i = 3;
    /* goes over the array in reverse*/
    for(index = i; index >= 0; index--){
        abc = 97;/* decimal value of the letter 'a'*/
        quotient = num % 4;/* gets the remainder*/
        num = num / 4;/* stores the result of the division*/
        /* increase the abc if the quotient is a bigger char(in ascii chart)*/
        while((unsigned int)quotient > abc-97){
            abc++;
        }
        store[index] = (char)abc;/* stores in reverse the ascii char(a,b,c,d)*/
    }
    store[i+1] = '\0';/* null terminates it*/
    return 0;
}


int conBinFour(char *store){
    char temp[11];/* used for copying the originl string*/
    int index;/* goes over the 10 bits*/
    int num;/* troes the number after casting*/
    int i;/* used for loop*/
    char storeBin[3];/* used for storing chars*/
    strcpy(temp, store);/* copies the string to temp array*/
    /* goes over the array  */
    for(index = 0, i = 0; index < 10; index += 2, i++){
        memcpy(storeBin, &store[index], 2);/* copies 2 chars each time*/
        storeBin[2] = '\0';/* null teminates the storeBin array*/
        num = atoi(storeBin);/* casts to number*/
        /* stores in  the ascii char(a,b,c,d)*/
        if(num == 0) store[i] = 'a';
        if(num == 1) store[i] = 'b';
        if(num == 10) store[i] = 'c';
        if(num == 11) store[i] = 'd';
    }
    store[5] = '\0';/* null terminates it*/
    return 0;
}

int conBinUn(char *store){
    int num;/* used for casting*/
    int index;/* saves the index until which we store*/
    int quotient;/* variables that stores the quotient*/
    unsigned int res;/* saves the result*/
    
    num = atoi(store);/* cast to decimal*/
    /* goes over the array in reverse*/
    for(index = 7; num > 0; index--){
        res = 48;
        quotient = num % 2;/* gets the remainder*/
        num = num / 2;/* stores the result of the division*/
        /* increase the res if the quotient is a bigger char(in ascii chart)*/
        while((unsigned int)quotient > res-48){
            res++;
        }
        store[index] = (char)res;/* stores in reverse the number*/
    }
    /* inserts leading zeros at the beginning of the array*/
    while(index >= 0){
        store[index--] = '0';/* stores in reverse 0*/
    }
    store[8] = '\0';/* null terminates it*/
    return 0;
}

