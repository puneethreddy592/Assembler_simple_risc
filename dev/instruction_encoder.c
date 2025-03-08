#include "token_parse.c"
#include<stdio.h>
#include<stdlib.h>
char *instruction_encode_storage[1000];

long int hexa_to_deci(char hex[]){
    int decimal = 0, base = 1, length, i, value;
    length = strlen(hex);
    for(i = length - 1; i >= 2; i--) {
        if(hex[i] >= '0' && hex[i] <= '9') {
            value = hex[i] - '0';
        } else if(hex[i] >= 'A' && hex[i] <= 'F') {
            value = hex[i] - 'A' + 10;
        }
        decimal += value * base;
        base = base * 16;
    }
    return decimal;
}


void decimal_to_binary(signed short int num,unsigned int resolution, FILE* file_ptr) {
    if (file_ptr == NULL) {
        printf("Error: File pointer is NULL.\n");
        return;
    }

    // Handle two's complement for negative numbers
    if (num < 0) {
        // Convert negative number to two's complement
        num = (1 << resolution) + num;  // Add 2^resolution to convert to two's complement
    }

    // Iterate over each bit of the number
    for (int i = resolution - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;  // Extract each bit using bitwise operations
        fprintf(file_ptr, "%d", bit);  // Write the bit to the file
    }

}
void instruction_encoder_formater_int(instruction_encode *encode, char file_int[]){
    int instruction_opcode_formater;
    int instruction_r_or_i_formater;
    short int instruction_offset_for__branch_formater_label;
    char instruction_offset_for_branch_formater_immediate[20];
    FILE *file_pointer_to_int;
    file_pointer_to_int = fopen(file_int, "a");
    if (file_pointer_to_int == NULL) {
            printf("Error creating file.\n");
            fclose(file_pointer_to_int);
            exit(0);
    }
    if(encode->instruction_in_used == 1){

        instruction_opcode_formater  = encode->instruction_in;
        fprintf(file_pointer_to_int,"%d|",instruction_opcode_formater);
        if(max_operands[encode->instruction_in] == 1){
            if(encode->label_placer_used == 1){
                short int label_pointing_address = valid_label_in(encode->label_placer);
                if( label_pointing_address == -1) {
                    memset(text, 0, sizeof(text));
                    sprintf(text,"Error: Undefined label has been used \"%s\"  ", encode->label_placer);
                    __message(text);
                    fclose(file_pointer_to_int);
                    remove(file_int);
                    exit(0);
                }
                else {
                   // printf("I have entered her \n");
                    instruction_offset_for__branch_formater_label =  label_pointing_address - encode->instruction_number  ;
                    fprintf(file_pointer_to_int,"%d|",instruction_offset_for__branch_formater_label);
                }
            }
            else {
                    if(encode->r_or_i == 0){
                        instruction_offset_for__branch_formater_label = encode->immediate_or_rs2_in.rs2_in;
                        fprintf(file_pointer_to_int, "%d|", instruction_offset_for__branch_formater_label);
                    }
                    else {
                        strcpy(instruction_offset_for_branch_formater_immediate,encode->immediate_or_rs2_in.immediate_value_in );
                        long int deci = hexa_to_deci(instruction_offset_for_branch_formater_immediate);
                        fprintf(file_pointer_to_int, "%ld|", deci);
                    }

            }

        }
    }

    if(max_operands[encode->instruction_in] != 1){

         instruction_r_or_i_formater = encode->r_or_i;
         fprintf(file_pointer_to_int, "%d|", instruction_r_or_i_formater);
        if(encode->rd_in_used == 1){
            fprintf(file_pointer_to_int, "%d|", encode->rd_in);
        }
        else {
            fprintf(file_pointer_to_int,"0|");
        }
        if(encode->rs1_in_used == 1){
            fprintf(file_pointer_to_int, "%d|", encode->rs1_in);
        }
        else {
            fprintf(file_pointer_to_int,"0");
        }
        if(encode->immediate_or_rs2_in_used == 1){
            if(encode->r_or_i == 0){
                fprintf(file_pointer_to_int, "%d", encode->immediate_or_rs2_in.rs2_in);
            }
            else {
                long int deci = hexa_to_deci(encode->immediate_or_rs2_in.immediate_value_in);
                fprintf(file_pointer_to_int, "%d|%ld",encode->modifier,deci);
            }
        }
        else {
            fprintf(file_pointer_to_int, "0");
        }
    }
    fprintf(file_pointer_to_int,"\n");
    fclose(file_pointer_to_int);
}
void instruction_encoder_formater_bin(instruction_encode *encode, char file_bin[]){
    int instruction_opcode_formater;
    int instruction_r_or_i_formater;
    short int instruction_offset_for__branch_formater_label;
    char instruction_offset_for_branch_formater_immediate[20];
    FILE *file_pointer_to_int;
    file_pointer_to_int = fopen(file_bin, "a");
    if (file_pointer_to_int == NULL) {
            printf("Error creating file.\n");
            fclose(file_pointer_to_int);
            exit(0);
    }
    if(encode->instruction_in_used == 1){

        instruction_opcode_formater  = encode->instruction_in;
        decimal_to_binary(instruction_opcode_formater,5,file_pointer_to_int);
        if(max_operands[encode->instruction_in] == 1){
            if(encode->label_placer_used == 1){
                short int label_pointing_address = valid_label_in(encode->label_placer);
                if( label_pointing_address == -1) {
                    memset(text, 0, sizeof(text));
                    sprintf(text,"Error: Undefined label has been used \"%s\"  ", encode->label_placer);
                    __message(text);
                    fclose(file_pointer_to_int);
                    remove(file_bin);
                    exit(0);
                }
                else {
                   // printf("I have entered her \n");
                    instruction_offset_for__branch_formater_label =  label_pointing_address - encode->instruction_number  ;

                    decimal_to_binary(instruction_offset_for__branch_formater_label,27, file_pointer_to_int);
                }
            }
            else {
                    if(encode->r_or_i == 0){
                        instruction_offset_for__branch_formater_label = encode->immediate_or_rs2_in.rs2_in;
                        decimal_to_binary(instruction_offset_for__branch_formater_label,27, file_pointer_to_int);

                    }
                    else {
                        strcpy(instruction_offset_for_branch_formater_immediate,encode->immediate_or_rs2_in.immediate_value_in );
                        long int deci = hexa_to_deci(instruction_offset_for_branch_formater_immediate);
                        decimal_to_binary(deci,27, file_pointer_to_int);

                    }

            }

        }
    }

    if(max_operands[encode->instruction_in] != 1){

         instruction_r_or_i_formater = encode->r_or_i;
         fprintf(file_pointer_to_int, "%d", instruction_r_or_i_formater);
        if(encode->rd_in_used == 1){
            decimal_to_binary(encode->rd_in,4,file_pointer_to_int) ;
        }
        else {
            fprintf(file_pointer_to_int,"0000");
        }
        if(encode->rs1_in_used == 1){
            decimal_to_binary(encode->rs1_in,4,file_pointer_to_int);
        }
        else {
            fprintf(file_pointer_to_int,"0000");
        }
        if(encode->immediate_or_rs2_in_used == 1){
            if(encode->r_or_i == 0){
                decimal_to_binary(encode->immediate_or_rs2_in.rs2_in,4,file_pointer_to_int);
                decimal_to_binary(0, 14, file_pointer_to_int);
            }
            else {
                long int deci = hexa_to_deci(encode->immediate_or_rs2_in.immediate_value_in);
                decimal_to_binary(encode->modifier,2,file_pointer_to_int);
                decimal_to_binary(deci,16,file_pointer_to_int);
            }
        }
        else {
            decimal_to_binary(0, 18,file_pointer_to_int);
        }
    }
    fprintf(file_pointer_to_int,"\n");
    fclose(file_pointer_to_int);
}
