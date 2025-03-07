#include<stdio.h>
#include <string.h>
#include "main.c"
char text[200];
typedef union {
    char immediate_value_in[20];
    unsigned short int rs2_in;
} immediate_rs2;
unsigned short valid_token_index = 0; // this is the main variable which tells us the current instruction encoding is done for the correct code written, as given by user.
typedef struct instruction_encoding {
    unsigned short int instruction_in; // this will store the interger value of the opcode
    unsigned short int r_or_i;
    unsigned short int rd_in;
    unsigned short int rs1_in;
    immediate_rs2 immediate_or_rs2_in;
    unsigned short int operands_in; // no of operands
    unsigned short int modifier;
    unsigned short int instruction_number;
} instruction_encode;

unsigned short int valid_register(char register_name[], instruction_encode *encode,unsigned int type ){// type takes 0 for Rd, 1 for Rs1 and 2 for Rs2
    unsigned short int register_value = 0;
    for(int i = 0;i<16;i++){
        if(strcmp(registers[i], register_name) == 0){
            register_value = i;
            break;
        }
    }
    if(type == 0){
        encode->rd_in = register_value;
        return TRUE;
    }
    else if (type == 1){
        encode->rs1_in = register_value;
        return TRUE;
    }
    else if(type == 2){
        encode->immediate_or_rs2_in.rs2_in = register_value;
        return TRUE;
    }
    else {
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
            return encode->modifier;
        }
        else if(strcmp(instruction_names_u[i], instruction_name) == 0) {
            encode->r_or_i = 1;
            encode->modifier = 1;
            encode->instruction_in = i;
            return encode->modifier;

        }
        else if(strcmp(instruction_names_h[i], instruction_name) == 0) {
            encode->r_or_i = 1;
            encode->modifier = 2;
            encode->instruction_in = i;
            return encode->modifier;
        }
    }
    return FALSE;
};
void valid_immediate(char token[], instruction_encode *encode, unsigned short int current_row, unsigned short int current_col){
    if(token[0] == '0' && token[1] == 'x'){
        if(strlen(token) < 9){
            encode->r_or_i = 1;
            strcpy(encode->immediate_or_rs2_in.immediate_value_in,token);
            encode->operands_in = max_operands[encode->instruction_in];
        }
        else if(strlen(token) == 9){
            printf("%c", token[3]);
            if(token[2] == '0' || token[2] == '1' || token[2] == '2' || token[2] == '3' || token[2] == '4' || token[2] == '5' || token[2] == '6' || token[2] == '7'){
                encode->r_or_i = 1;
                strcpy(encode->immediate_or_rs2_in.immediate_value_in,token);
                encode->operands_in = max_operands[encode->instruction_in];
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
    }
}
_Bool valid_token(char token[], instruction_encode *encode,unsigned short int current_row, unsigned short int current_col){
    // valid_token_index++;
    unsigned short int max;
    if ( encode->instruction_in){
        max = max_operands[encode->instruction_in];
        if(max ==0 ){
            if (strcmp(token, "EOL") !=0){
                char text[100];
                sprintf(text,"Error: Invalid operand for the  instruction %s at position %d:%d", token, current_row, current_col);
                __message(text);
                exit(0);
            }
            else {
                encode->operands_in = 0;
                valid_token_index++;
            }
        }
        else if(max == 1){

            if(encode->operands_in != 1) {
                valid_immediate(token, encode, current_row,current_col);
            }
            else {
            if(strcmp(token,"EOL") != 0) {
                memset(text, 0, sizeof(text));
                sprintf(text,"Error: Invalid operand for a 1 one operand instruction %s at position %d:%d", token, current_row, current_col);
                __message(text);
                exit(0);
            }
            else {
                valid_token_index++;
            }
         }
        }

    }
    valid_instruction(token,encode);
    return FALSE;
}
_Bool pass_token(char token[], instruction_encode *encode, unsigned short int current_row, unsigned short int current_col){ // whenever a token is generated here the nessecary validation happens
    if( token[0] !='\0' ){
        static int i=0;
        i++;
       printf("%d %s\n", i, token);
       valid_token(token, encode,current_row + 1, current_col+1);
    }


    if(strcmp(token,"hlt") == 0){
        __message("Program Terminating, as hlt function has been reached\n");
    }
    return TRUE;
}
int main(int argc, char *argv[]){ // here i am taking the assembly file input thorough this argument
    __init(argc, argv);
    FILE *fptr = fopen(argv[1], "r");
    char *file_path = argv[1];
    char source[16000]; // i am taking an instuction lines of 100, max instructions per line is 16 characters excluding spaces, hence 30*1000 = 30000
    unsigned int source_len = 0;
    while (1) {
        char c = fgetc(fptr);
        if (c == EOF) break;

        if (source_len < sizeof(source) - 1) {
            source[source_len] = c;
            source_len++;
            source[source_len] = '\0';
        }
        else{
            __message("Error: Source buffer overflow, too many tokens in give file, the maximum characters are 16000!!!!\n");
            break;
        }
    }

    unsigned int source_len_copy = source_len;

    unsigned short int current_row=0;
    unsigned short int current_col=0;
    unsigned short int current_cur=0; // this guy basically points to the present character that is being tokenized
    instruction_encode encodes[1000]; // this stores an instruction line encode in struct
    char token[10] ="";
    unsigned short int token_index=0;
    printf("The name of the file is %s \n", file_path);
    printf("Total No of Characters in the given file is %d \n", source_len);



    while(source_len_copy--){ // parse and token while loop
       encodes[valid_token_index].instruction_number = valid_token_index+1;
        char current_c =  source[current_cur];
        if ( current_c == ' '){
            current_cur++;
            pass_token(token, &encodes[valid_token_index], current_row, current_col);
            token_index=0;
            memset(token,0,sizeof(token));
        }
        else if(current_c == '\n'){
            pass_token(token, &encodes[valid_token_index], current_row, current_col);
            token_index=0;
            memset(token,0,sizeof(token));
            pass_token("EOL",&encodes[valid_token_index], current_row, current_col);
            current_row++;
            current_cur++;
            current_col=1;
        }
        else if(current_c == ','){
            current_cur++;
            pass_token(token, &encodes[valid_token_index], current_row, current_col);
            token_index=0;
            memset(token,0,sizeof(token));
            pass_token(",",&encodes[valid_token_index], current_row, current_col);
        }
        else if(current_c == '/') {
            char current_c = source[++current_cur];
            if (current_c == '/') {
                while(1){
                    current_cur++;
                    current_c = source[current_cur];
                    if( current_c == '\n'){
                        pass_token(token, &encodes[valid_token_index], current_row, current_col);
                        token_index=0;
                        memset(token,0,sizeof(token));
                        pass_token("EOL",&encodes[valid_token_index], current_row, current_col);
                        current_row++;
                        current_cur++;
                        current_col = 1;
                        break;
                    }
                }
            }
            else {
                memset(text, 0, sizeof(text));
                sprintf(text,"Error: Undefined token \"%c\"  at %s:%d:%d ",current_c ,file_path,current_row+1, current_col+1);
                __message(text);
                exit(0);
            }

        }
        else if(current_c == '['){
            pass_token(token, &encodes[valid_token_index], current_row, current_col);
            token_index=0;
            memset(token,0,sizeof(token));
            pass_token("[",&encodes[valid_token_index], current_row, current_col);
            current_cur++;
        }
        else if(current_c == ']'){
            pass_token(token, &encodes[valid_token_index], current_row, current_col);
            token_index=0;
            memset(token,0,sizeof(token));
            pass_token("]",&encodes[valid_token_index], current_row, current_col);
            current_cur++;
        }
        else if (current_c == ':'){ // have to write for labels matching
            pass_token(":",&encodes[valid_token_index], current_row, current_col);
            current_cur++;
        }
        else if( ' '< current_c && current_c < '0'|| '9' < current_c && current_c < '`' || current_c > 'z' || current_c < ' '){
            memset(text, 0, sizeof(text));
            if(strcmp(token,"hlt")) break;
            sprintf(text,"Error: Undefined token \"%c\" at %s:%d:%d ", current_c,file_path,current_row+1, current_col+1);
            __message(text);
            exit(0);
        }
        else {
            token[token_index] = current_c;
            token_index++;
            current_cur++;
            if(token_index > 9){
                __message("Error: buffer Overflow at token in parse while loop\n");
            }
        }
    }

    printf("\nTotal No of instruction encoding done is %d \n", valid_token_index);
    printf("%s",encodes[1].immediate_or_rs2_in.immediate_value_in);
    return 0;
}
