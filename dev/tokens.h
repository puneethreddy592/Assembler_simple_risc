//declaring my registers
enum token_registers {
    TOKEN_R1 = 0,
    TOKEN_R2,
    TOKEN_R3,
    TOKEN_R4,
    TOKEN_R5,
    TOKEN_R6,
    TOKEN_R7,
    TOKEN_R8,
    TOKEN_R9,
    TOKEN_R10,
    TOKEN_R11,
    TOKEN_R12,
    TOKEN_R13,
    TOKEN_R14,
    TOKEN_R15
};

enum token_instructions {
    TOKEN_ADD = 0,
    TOKEN_SUB = 1,
    TOKEN_MUL = 2,
    TOKEN_DIV = 3,
    TOKEN_MOD = 4,
    TOKEN_CMP = 5,
    TOKEN_AND = 6,
    TOKEN_OR = 7,
    TOKEN_NOT = 8,
    TOKEN_MOV = 9,
    TOKEN_LSL = 10,
    TOKEN_LSR = 11,
    TOKEN_ASR = 12,
    TOKEN_NOP = 13,
    TOKEN_LD = 14,
    TOKEN_ST = 15,
    TOKEN_BEQ = 16,
    TOKEN_BGT = 17,
    TOKEN_B = 18,
    TOKEN_CALL = 19,
    TOKEN_RET = 20
};

enum token_modifier {
    TOKEN_DEFAULT=0,
    TOKEN_U = 1,
    TOKEN_H=2,
};
