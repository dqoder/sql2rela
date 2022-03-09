#ifndef __MY_UTIL_H__
#define __MY_UTIL_H__

#define UNION_SYM "∪"
#define INTER_SYM "∩"
#define JOIN_SYM "⋈ "
#define MINUS_SYM "-"
#define PI_SYM "Π"
#define SIGMA_SYM "σ"
#define AND_SYM "∧"
#define OR_SYM "∨"

typedef enum logical_opration {
    AND, OR
} logopr;

typedef enum set_operation {
    UNION, INTER, MINUS
} setopr;

typedef enum rel_operation {
    EQ, LE, GE, LT, GT
} relopr;


typedef struct struct_type {
    int att_cont;
    int use_ac;
    char* str;
} p_type;

/* ---- array ----- */

extern const char* SETOP_STR[];
extern const char* LOGOP_STR[];
extern const char* RELOP_STR[];
// int Ar00[] = { 1,3 };
// char* SETOP_STR[3] = {
//     [UNION] = UNION_SYM,
//     [INTER] = INTER_SYM,
//     [MINUS] = MINUS_SYM
// };

// char* LOGOP_STR[2] = {
//     [AND] = AND_SYM,
//     [OR] = OR_SYM
// };

// char* RELOP_STR[5] = {
//     [EQ] = "=",
//     [LE] = "<=",
//     [GE] = ">=",
//     [LT] = "<",
//     [GT] = ">"
// };

// if above didn't work
// char RELOP_STR[5][3] = { "=", "<=", ">=", "<", ">" };

/* --- functions --- */
// char* cr_str(); // create string

p_type* createPT(int att_cnt, int use_att, char* str);
p_type* createPTfromNum(int att_cnt, int use_att, int num);

p_type* createMatt(p_type*, p_type*);

// p_type* createOnClause(p_type*); // redundant :- $$ = $2 is same
p_type* createJoin(p_type*, p_type*, p_type*);
p_type* createJoinClause(p_type*, p_type*, p_type*);
p_type* createJoinComma(p_type*, p_type*);

p_type* createFromClause(p_type*, p_type*);
// p_type* createWhereClause(p_type*); // redundant :- $$ = $2 is same
p_type* createSelectClause(p_type*);
p_type* createQueryClause(p_type*, p_type*);

p_type* concat_attr(p_type*, p_type*);

p_type* setop(p_type*, setopr, p_type*);
p_type* relop(p_type*, relopr, p_type*);
p_type* logop(p_type*, logopr, p_type*);


#endif 