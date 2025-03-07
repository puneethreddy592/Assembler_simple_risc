#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokens.h" // this is to import tokens and registers global arrays;
#define TRUE 1
#define FALSE 0
typedef enum token_instruction {
    TOKEN_INSTRUNCION,
    TOKEN_REGISTER,
    TOKEN_R_OR_I,
    TOKEN_MODIFIER,
} token_instruct;
// declaring my registers and instruction_names;
const char *registers[16] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15"};
const char *instruction_names[21] = {
    "add", "sub", "mul", "div", "mod", "cmp", "and", "or", "not", "mov",
    "lsl", "lsr", "asr", "nop", "ld", "st", "beq", "bgt", "b", "call", "ret"
};
const char *instruction_names_u[21] = {
    "addu", "subu", "mulu", "divu", "modu", "cmpu", "andu", "oru", "notu", "movu",
    "lslu", "lsru", "asru", "nop", "ldu", "stu", "beq", "bgt", "b", "call", "ret"
};
const char *instruction_names_h[21] = {
    "addh", "subh", "mulh", "divh", "modh", "cmph", "andh", "orh", "noth", "movh",
    "lslh", "lsrh", "asrh", "nop", "ldh", "sth", "beq", "bgt", "b", "call", "ret"
};
const unsigned int max_operands[21] = {
    3, 3, 3, 3, 3, 2, 3,3, 2,2,3,3,3,0,3,3,1,1,1,1,0
};
//Declaring funcitons
void __message(char message[]);
_Bool file_verify(int argc, char *argv[]);
_Bool __init(int argc, char *argv[]){ // this is the initialsing funcion, that does file verification and sorts
    __message("Program has initialised....");
    _Bool _initstatus0 = file_verify(argc, argv);
    if(!_initstatus0) {
        __message("The program has been terminated, due to fail in file validation...");
        exit(0);
    }
    return TRUE;
}

/*
main function starting
*/


/*
int main(int argc, char *argv[]){ // here i am taking the assembly file input thorough this argument
    __init(argc, argv);
    FILE *fptr = fopen(argv[1], "r");

    return 0;
}
*/



/*
    functioin declaratoins

*/

void __message(char message[]){
    printf("%s \n", message);
}
_Bool file_verify(int argc, char *argv[]){
    if (argc == 1){
        __message("Error File Has not been passed, terminating the program");
        return FALSE;
    }
    if (argc > 2){
        __message("Error: More than one argumnets passed, exiting program execution.");
        return FALSE;
    }
    unsigned int length_file_name =strlen(argv[1]);
    if (length_file_name > 20){
        __message("File name too big, reduce the file name to less than 20 characters");
        return FALSE;
    }
    FILE *file = fopen(argv[1],"r");
    if(file == NULL){
        char text[100];
        sprintf(text,"Error: File doesn't exist, please check the file name,  \n Given File Name: %s", argv[1]);
        __message(text);
        fclose(file);
        return FALSE;
    }
    else {
        fclose(file);
        char text[100];
        sprintf(text,"File exist, please check the file name \n Given File Name: %s", argv[1]);
        __message(text);
    }
    char* extension_file_accepting[4]; // this is to check whether the extension of the file is proper or not
    // currently accepting only .txt
    extension_file_accepting[0] = ".txt";
    unsigned short int index = 0;
    for (int i=0;i < length_file_name;i++){
        char c = argv[1][i];
        if (c == '.'){
            index = i;
            break;
        }
    }
    char str[20];
    memset(str, 0, sizeof(str));  // Ensure the string is null-terminated
    strncpy(str,argv[1]+index,length_file_name - index);
    if (strcmp(str,extension_file_accepting[0])!=0){
        __message("Error: Wrong file extension, only .txt files are accepted");
        return FALSE;
    }
    return TRUE;
}
