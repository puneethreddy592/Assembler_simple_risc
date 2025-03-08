#include<stdio.h>
#include <stdlib.h>
#include <string.h>
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
    3, 3, 3, 3, 3, 2, 3,3, 2,2,3,3,3,0,2,2,1,1,1,1,0
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

char text[200]; // this will store the message
unsigned short int valid_in = 0;
typedef union {
    char immediate_value_in[20];
    signed short int rs2_in;
} immediate_rs2;
unsigned short valid_token_index = 0; // this is the main variable which tells us the current instruction encoding is done for the correct code written, as given by user.
typedef struct instruction_encoding {
    unsigned short int instruction_in ; // this will store the interger value of the opcode
    unsigned short int r_or_i;
    unsigned short int rd_in;
    unsigned short int rs1_in;
    immediate_rs2 immediate_or_rs2_in;
    unsigned short int modifier;
    unsigned short int instruction_number;
    char label_placer[10]; // for branch instrunctions we will take either immediate or label, since label might not be defined yet, we will store the name here later when the program reached hlt, we will check whether label is defined in other part of the code
    unsigned int instruction_in_used : 1; // this has been intially assigned to 1 meaning it has not been assigned it its assigned we will change it to 0
    unsigned int   r_or_i_used : 1; // u have to know that this means, i am setting the actual value to zero
    unsigned int rd_in_used : 1;
    unsigned int rs1_in_used : 1;
    unsigned int label_placer_used: 1;
    unsigned int immediate_or_rs2_in_used : 1;
    unsigned int valid_set : 1; // this to solve the issue of checking for commas as separator for registers

} instruction_encode;

unsigned short int valid_register(char register_name[], instruction_encode *encode,unsigned int type ){// type takes 0 for Rd, 1 for Rs1 and 2 for Rs2
    unsigned short int register_value = 0;
    unsigned short int valid = 0;
    for(int i = 0;i<16;i++){
        if(strcmp(registers[i], register_name) == 0){
            register_value = i;
            valid = 1;
            break;
        }
    }
    if(type == 0 && valid==1){
        encode->rd_in = register_value;
        encode->rd_in_used = 1;
        return TRUE;
    }
    else if (type == 1 && valid == 1){
        encode->rs1_in = register_value;
        encode->rs1_in_used = 1;
        return TRUE;
    }
    else if(type == 2 && valid==1){
        encode->r_or_i = 0;
        encode->immediate_or_rs2_in.rs2_in = register_value;
        encode->immediate_or_rs2_in_used = 1;
        return TRUE;
    }
    else if(type > 2){
        __message("Error: Internal error happened in valid_instruction function in exp.c \n, sorry for the internal error???????\n");
        return FALSE;
    }
    return FALSE;
};

unsigned short int valid_instruction(char instruction_name[], instruction_encode *encode){

    for(int i = 0;i<21;i++){
        if(strcmp(instruction_names[i], instruction_name) == 0) {
            encode->modifier = 0;
            encode->instruction_in = i;
            encode->instruction_in_used = 1;
            return encode->modifier;
        }
        else if(strcmp(instruction_names_u[i], instruction_name) == 0) {
            encode->r_or_i = 1;
            encode->modifier = 1;
            encode->instruction_in = i;
            encode->instruction_in_used = 1;
            return encode->modifier;
        }
        else if(strcmp(instruction_names_h[i], instruction_name) == 0) {
            encode->r_or_i = 1;
            encode->modifier = 2;
            encode->instruction_in = i;
            encode->instruction_in_used = 1;
            return encode->modifier;
        }
    }
    return 5;
};
_Bool valid_hexaformat(char token[]){
    if(token[2] == '\0'){
        return FALSE;
    }
    for(int i=2;i< strlen(token);i++){

        if (!((token[i] >= '0' && token[i] <= '9') || (token[i] >= 'a' && token[i] <= 'f'))) {
            return FALSE;  // Invalid character found
        }

    }
    return TRUE;
}
void valid_immediate(char token[], instruction_encode *encode, unsigned short int current_row, unsigned short int current_col){
    if(token[0] == '0' && token[1] == 'x'){
        if(strlen(token) < 9){
            if(valid_hexaformat(token)){
                encode->r_or_i = 1;
                strcpy(encode->immediate_or_rs2_in.immediate_value_in,token);
            }
            else {
                memset(text, 0, sizeof(text));
                sprintf(text,"Error:Invalid hexainput %s at position %d:%d", token, current_row, current_col);
                __message(text);
                exit(0);
            }

        }
        else if(strlen(token) == 9){
            if(token[2] == '0' || token[2] == '1' || token[2] == '2' || token[2] == '3' || token[2] == '4' || token[2] == '5' || token[2] == '6' || token[2] == '7'){
                if(valid_hexaformat(token)){
                    encode->r_or_i = 1;
                    strcpy(encode->immediate_or_rs2_in.immediate_value_in,token);
                }
                else {
                    memset(text, 0, sizeof(text));
                    sprintf(text,"Error:Invalid hexainput %s at position %d:%d", token, current_row, current_col);
                    __message(text);
                    exit(0);
                }

            }
            else {
                memset(text, 0, sizeof(text));
                sprintf(text,"Error:, 1 crossed the max width of the immediate value, max range for immediate is 0x7FFFFFF (27 bits relative offset) given is %s at position %d:%d", token, current_row, current_col);
                __message(text);
                exit(0);
            }
        }
        else {
            memset(text, 0, sizeof(text));
            sprintf(text,"Error:, crossed the max width of the immediate value, max range for immediate is 0x7FFFFFF (27 bits relative offset) given is %s at position %d:%d", token, current_row, current_col);
            __message(text);
            exit(0);
        }
    }
    else {
        memset(text, 0, sizeof(text));
        sprintf(text,"Error: Invalid operand for the instruction, its not in the hexadecimal format %s at position %d:%d", token, current_row, current_col);
        __message(text);
        exit(0);
    }
}

typedef struct labels_in{
    char label_name[10];
    unsigned short int label_address_pointing_to;

    unsigned short int label_used : 1;
}label;
label labels[100];
signed short int label_index = 0;
signed short int valid_label_in(char token[]){
    for(int i = 0; i <= label_index;i++){
        if(strcmp(labels[i].label_name, token) == 0){
            return labels[i].label_address_pointing_to;
        }
    }
    return -1;
}
_Bool valid_token(char token[], instruction_encode *encode,unsigned short int current_row, unsigned short int current_col){
    // valid_token_index++;
    unsigned short int max;
    unsigned short int valid_inst = 0;
    valid_inst = valid_instruction(token,encode);
    if(label_index != -1 && labels[label_index].label_used == 1){
        strncpy(labels[label_index].label_name,token,strlen(token));
        labels[label_index].label_address_pointing_to = encode->instruction_number;
        label_index++;
        return TRUE;
    }
    if( (valid_inst > 2 && encode->instruction_in_used != 1)){
        if(strcmp(token, ":") == 0){
            labels[label_index].label_used = 1;
        }
        else if (strcmp(token, "EOL") !=0){
            memset(text, 0, sizeof(text));
            sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
            __message(text);
            exit(0);
        }
    }


    if (encode->instruction_in_used == 1 && valid_inst > 2){
        max = max_operands[encode->instruction_in];

        if(max ==0 ){
            if (strcmp(token, "EOL") !=0){
                char text[100];
                sprintf(text,"Error: Invalid operand for the  instruction %s at position %d:%d", token, current_row, current_col);
                __message(text);
                exit(0);
            }
            else {
                valid_token_index++;
                printf("%d token \n", valid_token_index);
                valid_in = 1;
            }
        }
        else if(max == 1){
            if(encode->immediate_or_rs2_in_used != 1) {

                encode->immediate_or_rs2_in_used = 1 ;
                short int label = valid_label_in(token);
                if(label == -1){
                    if(token[0] == '0' && token[1] == 'x'){
                        valid_immediate(token, encode,current_row,current_col);
                    }
                    else {
                        encode->label_placer_used = 1;
                        strcpy(encode->label_placer, token);
                    }
                }
                else {
                    encode->r_or_i = 0;
                    label = encode->instruction_number - label;
                    encode->immediate_or_rs2_in.rs2_in = (-1)*label;
                    printf("%d \n", label);
                }
            }
            else {
            if(strcmp(token,"EOL") != 0) {
                memset(text, 0, sizeof(text));
                sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                __message(text);
                exit(0);
            }
            else {
                valid_token_index++;
                valid_in = 1;
                printf("%d token \n", valid_token_index);

            }
         }
        }
        else if(max == 3){ // this code has to check for commas after register update this, register read then we have to check for comma and then check for register
            if(encode->rd_in_used != 1) {
                valid_register(token, encode, 0);
            }
            else if(encode->rs1_in_used != 1 && encode->rd_in_used == 1 &&  encode->valid_set == 0){
                if(strcmp( token,",") != 0 ){
                    memset(text, 0, sizeof(text));
                    sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                    __message(text);
                    exit(0);
                }
                else {
                    encode->valid_set = 1;
                }
            }
            else if(encode->rd_in_used == 1 && encode->valid_set == 1 && encode->rs1_in_used != 1){

                if(encode->rs1_in_used != 1){
                    if(valid_register(token, encode,1) == 0){
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        encode->valid_set = 0;
                    }

                }
            }
            else if(encode->rs1_in_used == 1 && encode->valid_set == 0 && encode->immediate_or_rs2_in_used != 1){

                if(strcmp( token,",") != 0 ){
                    memset(text, 0, sizeof(text));
                    sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                    __message(text);
                    exit(0);
                }
                else {
                    encode->valid_set = 1;
                }
            }
            else if(encode->rs1_in_used == 1 && encode->valid_set == 1 && encode->immediate_or_rs2_in_used != 1){
                if(valid_register(token,encode, 2) == 0){
                    valid_immediate(token, encode,current_row, current_col);
                    encode->immediate_or_rs2_in_used = 1;
                    encode->valid_set = 0;
                }
                else {
                    encode->valid_set = 0;
                }
            }
            else if(encode->immediate_or_rs2_in_used == 1 && encode->valid_set == 0){
                if(strcmp(token,"EOL") != 0) {
                    memset(text, 0, sizeof(text));
                    sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                    __message(text);
                    exit(0);
                }
                else {
                    valid_token_index++;
                    valid_in = 1;
                    printf("%d token \n", valid_token_index);

                }
            }
        }
        else if(max == 2){ // this will be ld - 14, st - 15, mov - 9, cmp - 5, not - 8
            if( encode->instruction_in == 14 || encode->instruction_in == 15 ) {
                if(encode->rd_in_used != 1 ){
                    if(valid_register(token, encode, 0) == 1){
                        encode->valid_set = 1;
                    }
                    else {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                }
                else if(encode->rd_in_used == 1 && encode->valid_set == 1 && encode->immediate_or_rs2_in_used != 1 && encode->rs1_in_used != 1){
                    if(strcmp( token,",") != 0 ){
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        encode->valid_set = 0;
                    }
                }
                else if(encode->immediate_or_rs2_in_used != 1 && encode->valid_set == 0 && encode->rd_in_used == 1 && encode->rs1_in_used != 1){
                    if(valid_register(token,encode, 2) == 0){
                        valid_immediate(token, encode,current_row, current_col);
                        encode->immediate_or_rs2_in_used = 1;
                        encode->valid_set = 1;
                    }
                    else {
                        encode->valid_set = 1;
                    }
                }
                else if(encode->immediate_or_rs2_in_used == 1 && encode->valid_set == 1 && encode->rs1_in_used != 1){
                    if(strcmp(token,"[") != 0) {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d instead of [", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        encode->valid_set = 0;
                    }
                }
                else if(encode->immediate_or_rs2_in_used == 1 && encode->valid_set == 0 && encode->rs1_in_used != 1){
                    if(valid_register(token, encode, 1) == 1){
                        encode->valid_set = 1;
                    }
                    else {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                }
                else if(encode->immediate_or_rs2_in_used == 1 && encode->valid_set == 1 && encode->rs1_in_used == 1){
                    if(strcmp(token,"]") != 0) {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand,the max operands is 2, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        encode->valid_set = 0;
                    }
                }
                else if(encode->immediate_or_rs2_in_used == 1 && encode->valid_set == 0 && encode->rs1_in_used == 1 && encode->rd_in_used == 1){
                    if(strcmp(token,"EOL") != 0) {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand,the max operands is 2, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        valid_token_index++;
                        valid_in = 1;
                        printf("%d token \n", valid_token_index);

                    }
                }
            }
            else if(encode->instruction_in == 9 || encode->instruction_in == 8){
                if(encode->rd_in_used != 1 ){
                    if(valid_register(token, encode, 0) == 1){
                        encode->valid_set = 1;
                    }
                    else {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                }
                else if(encode->rd_in_used == 1 && encode->valid_set == 1 && encode->immediate_or_rs2_in_used != 1){
                    if(strcmp( token,",") != 0 ){
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        encode->valid_set = 0;
                    }
                }
                else if(encode->immediate_or_rs2_in_used != 1 && encode->valid_set == 0 && encode->rd_in_used == 1){
                    if(valid_register(token,encode, 2) == 0){
                        valid_immediate(token, encode,current_row, current_col);
                        encode->immediate_or_rs2_in_used = 1;
                        encode->valid_set = 1;
                    }
                    else {
                        encode->valid_set = 1;
                    }
                }
                else if(encode->immediate_or_rs2_in_used == 1 && encode->valid_set == 1){
                    if(strcmp(token,"EOL") != 0) {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand,the max operands is 2, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        valid_token_index++;
                        valid_in = 1;
                        printf("%d token \n", valid_token_index);

                    }
                }
            }
            else if(encode->instruction_in == 5){
                if(encode->rs1_in_used != 1 && encode->valid_set == 0){
                    if(valid_register(token, encode, 1) == 1){
                        encode->valid_set = 1;
                    }
                    else {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                }
                else if(encode->rs1_in_used == 1 && encode->valid_set == 1 && encode->immediate_or_rs2_in_used != 1){

                    if(strcmp( token,",") != 0 ){
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        encode->valid_set = 0;
                    }
                }

                else if(encode->rs1_in_used == 1 && encode->valid_set == 0 && encode->immediate_or_rs2_in_used != 1){
                    if(valid_register(token,encode, 2) == 0){
                        valid_immediate(token, encode,current_row, current_col);
                        encode->immediate_or_rs2_in_used = 1;
                        encode->valid_set = 1;
                    }
                    else {
                        encode->valid_set = 1;
                    }
                }
                else if(encode->immediate_or_rs2_in_used == 1 && encode->valid_set == 1){
                    if(strcmp(token,"EOL") != 0) {
                        memset(text, 0, sizeof(text));
                        sprintf(text,"Error: Invalid operand,the max operands is 2, found %s at position %d:%d", token, current_row, current_col);
                        __message(text);
                        exit(0);
                    }
                    else {
                        valid_token_index++;
                        valid_in = 1;
                        printf("%d token \n", valid_token_index);

                    }
                }
                }
            }
        }

    return FALSE;
}
_Bool pass_token(char token[], instruction_encode *encode, unsigned short int current_row, unsigned short int current_col){ // whenever a token is generated here the nessecary validation happens
    if( token[0] !='\0' ){
        static int i=0;
        i++;
       printf("%d %s\n", i, token);
       if(strcmp(token,"hlt") == 0){
           __message("Program Terminating, as hlt function has been reached\n");
           return FALSE;
       }
       else {
           valid_token(token, encode,current_row + 1, current_col+1);
       }
    }


    if(strcmp(token,"hlt") == 0){
        __message("Program Terminating, as hlt function has been reached\n");
    }
    return TRUE;
}

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
