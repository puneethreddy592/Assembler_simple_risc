#include "tokens.h" // this is to import tokens and registers global arrays;
#include "instruction_encoder.c"
#include <string.h>
/*
main function starting
*/


int main(int argc, char *argv[]){ // here i am taking the assembly file input thorough this argument
    __init(argc, argv);
    remove("output.bin");
    remove("output.int");
    remove("output.hex");
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
    fclose(fptr);
    unsigned int source_len_copy = source_len;

    unsigned short int current_row=0;
    unsigned short int current_col=0;
    unsigned short int current_cur=0; // this guy basically points to the present character that is being tokenized
    instruction_encode encodes[1000]; // this stores an instruction line encode in struct
    char token[10] ="";
    unsigned short int token_index=0;


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


char file_name[20];
strcpy(file_name, "output");
char file_bin_name[20];
strcpy(file_bin_name, file_name);
strcat(file_bin_name,".bin");
char file_int_name[20];
strcpy(file_int_name, file_name);
strcat(file_int_name,".int");

for(int i=0; i<valid_token_index;i++){
    instruction_encoder_formater_int(&encodes[i],file_int_name);
}
for(int i=0; i<valid_token_index;i++){
    instruction_encoder_formater_bin(&encodes[i], file_bin_name);
}



return 0;
}



/*
    functioin declaratoins

*/
