#include "my_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char* cr_str() {
    return (char*)malloc(100);
}

static p_type* allocatePT() {
    p_type* b = (p_type*)malloc(sizeof(p_type));
    if (b == NULL) return NULL;
    b->att_cont = b->use_ac = 0;
    b->str = NULL;

    return b;
}

p_type* createPT(int att_cont, int use_att, char* str) {
    p_type* b = allocatePT();
    if (b == NULL) return NULL;
    b->att_cont = att_cont;
    b->use_ac = use_att;
    char* str_new = cr_str();

    if (str != NULL)
        strcpy(str_new, str);
    else str_new[0] = '\0';

    b->str = str_new;
    return b;
}

p_type* createPTfromNum(int att_cont, int use_att, int num) {
    p_type* b = allocatePT();
    if (b == NULL) return NULL;
    b->att_cont = att_cont;
    b->use_ac = use_att;
    char* str_new = cr_str();
    sprintf(str_new, "%d", num);
    b->str = str_new;
    return b;
}

p_type* createMatt(p_type* table, p_type* attr) {
    // new PT
    p_type* npt = createPT(0, 0, NULL);
    sprintf(npt->str, "%s.%s", table->str, attr->str);

    return npt;
}

// reduntant --> $$ = $2 is same
// p_type* createOnClause(p_type* ocl) {
//     p_type* b = createPT(0, 0, NULL);
//     if (b == NULL) return NULL;
//     sprintf(b->str, "%s", ocl->str);

//     return b;
// }

p_type* createJoin(p_type* t1, p_type* t2, p_type* ocl) {
    p_type* b = createPT(0, 0, NULL);
    if (b == NULL) return NULL;

    // checking if on clause exist
    if (strlen(ocl->str) > 0) {
        sprintf(b->str, "%s%s%s_{%s}", t1->str, JOIN_SYM, t2->str, ocl->str);
    }
    else {
        sprintf(b->str, "%s%s%s", t1->str, JOIN_SYM, t2->str);
    }

    return b;
}

p_type* createJoinClause(p_type* jc, p_type* t, p_type* ocl) {
    p_type* b = createPT(0, 0, NULL);
    if (b == NULL) return NULL;

    // checking if on clause exist
    if (strlen(ocl->str) > 0) {
        sprintf(b->str, "(%s)%s%s_{%s}", jc->str, JOIN_SYM, t->str, ocl->str);
    }
    else {
        sprintf(b->str, "(%s)%s%s", jc->str, JOIN_SYM, t->str);
    }
    return b;
}

p_type* createJoinComma(p_type* t1, p_type* t2) {
    p_type* b = createPT(0, 0, NULL);
    if (b == NULL) return NULL;

    sprintf(b->str, "%s%s%s", t1->str, JOIN_SYM, t2->str);

    return b;
}

p_type* createFromClause(p_type* r, p_type* wc) {
    p_type* b = createPT(0, 0, NULL);
    if (b == NULL) return NULL;

    // checking if selection needed
    if (strlen(wc->str) > 0)
        sprintf(b->str, "%s_{%s}(%s)", SIGMA_SYM, wc->str, r->str);
    else
        sprintf(b->str, "%s", r->str);

    return b;
}

// p_type* createWhereClause(p_type*); // redundant :- $$ = $2 is same

p_type* createSelectClause(p_type* att) {
    p_type* b = createPT(att->att_cont, 1, NULL);
    if (b == NULL) return NULL;
    if (att->att_cont == 0) return b;
    else {
        sprintf(b->str, "%s_{%s}", PI_SYM, att->str);
        return b;
    }
    return NULL; // should not reach here
}

p_type* createQueryClause(p_type* sc, p_type* fwc) {
    p_type* b = createPT(sc->att_cont, 1, NULL);
    if (b == NULL) return NULL;

    // checking if projection needed
    if (strlen(sc->str) > 0)
        sprintf(b->str, "%s(%s)", sc->str, fwc->str);
    else
        sprintf(b->str, "%s", fwc->str);

    return b;
}


p_type* concat_attr(p_type* att1, p_type* att2) {
    p_type* b = createPT(0, 1, NULL);
    if (b == NULL) return NULL;

    // wrongly used 
    if (att1->att_cont == 0 || att2->att_cont == 0
        || att1->use_ac == 0 || att2->use_ac == 0) {
        fprintf(stderr, "ERROR : invalid syntax (wrong attribute)\n");
        exit(EXIT_FAILURE);
    }

    // normal case
    b->att_cont = att1->att_cont + att2->att_cont;
    sprintf(b->str, "%s,%s", att1->str, att2->str);

    return b;
}

p_type* setop(p_type* q1, setopr s, p_type* q2) {
    // error case
    if (q1->att_cont != q2->att_cont) {
        fprintf(stderr, "ERROR: attributes' count should be same while performing set operations\n");
        exit(EXIT_FAILURE);
    }

    p_type* b = createPT(q1->att_cont, 1, NULL);
    if (b == NULL) return NULL;

    sprintf(b->str, "%s %s %s", q1->str, SETOP_STR[s], q2->str);
    return b;
}

p_type* relop(p_type* p1, relopr r, p_type* p2) {
    p_type* b = createPT(0, 0, NULL);
    if (b == NULL) return NULL;

    sprintf(b->str, "%s%s%s", p1->str, RELOP_STR[r], p2->str);
    return b;
}

p_type* logop(p_type* p1, logopr l, p_type* p2) {
    p_type* b = createPT(0, 0, NULL);
    if (b == NULL) return NULL;

    sprintf(b->str, "%s%s%s", p1->str, LOGOP_STR[l], p2->str);
    return b;
}

const char* SETOP_STR[3] = {
    [UNION] = UNION_SYM,
    [INTER] = INTER_SYM,
    [MINUS] = MINUS_SYM
};

const char* LOGOP_STR[2] = {
    [AND] = AND_SYM,
    [OR] = OR_SYM
};

const char* RELOP_STR[5] = {
    [EQ] = "=",
    [LE] = "<=",
    [GE] = ">=",
    [LT] = "<",
    [GT] = ">"
};
