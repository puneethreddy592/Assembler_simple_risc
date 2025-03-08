#include<stdio.h>
#include <string.h>
#include "main.c"
char text[200]; // this will store the message
unsigned short int valid_in = 0;
typedef union {
    char immediate_value_in[20];
    unsigned short int rs2_in;
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

    unsigned int instruction_in_used : 1; // this has been intially assigned to 1 meaning it has not been assigned it its assigned we will change it to 0
    unsigned int   r_or_i_used : 1; // u have to know that this means, i am setting the actual value to zero
    unsigned int rd_in_used : 1;
    unsigned int rs1_in_used : 1;
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
void valid_immediate(char token[], instruction_encode *encode, unsigned short int current_row, unsigned short int current_col){
    if(token[0] == '0' && token[1] == 'x'){
        if(strlen(token) < 9){
            encode->r_or_i = 1;
            strcpy(encode->immediate_or_rs2_in.immediate_value_in,token);
        }
        else if(strlen(token) == 9){
            printf("%c", token[3]);
            if(token[2] == '0' || token[2] == '1' || token[2] == '2' || token[2] == '3' || token[2] == '4' || token[2] == '5' || token[2] == '6' || token[2] == '7'){
                encode->r_or_i = 1;
                strcpy(encode->immediate_or_rs2_in.immediate_value_in,token);
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
_Bool valid_token(char token[], instruction_encode *encode,unsigned short int current_row, unsigned short int current_col){
    // valid_token_index++;
    unsigned short int max;
    unsigned short int valid_inst = 0;
    valid_inst = valid_instruction(token,encode);
    printf("%d %d %s\n", valid_inst, encode->instruction_in_used,token );
    if( valid_inst > 2 && encode->instruction_in_used != 1){
        if (strcmp(token, "EOL") !=0){
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
                valid_immediate(token, encode, current_row,current_col);
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
        if(valid_in == 1){
            valid_in = 0;
        }
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
