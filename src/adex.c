#include "adex.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <vector/vector.h>

const char *adex_error_name[] = {
    "success", "out of memory", "syntax error", "arithmetic error", "address unavailable",
};

#define ID_MAX 0x20

enum tok_type {
    TOK_CONST,
    TOK_ID,
    TOK_OP,
    TOK_BRACK_L,
    TOK_BRACK_R,
};

enum op {
    OP_B,
    OP_BZ,
    OP_H,
    OP_HZ,
    OP_W,
    OP_MUL,
    OP_DIV,
    OP_REM,
    OP_ADD,
    OP_SUB,
    OP_MAX,
};

enum brack {
    BRACK_PAREN,
    BRACK_SQUARE,
};

struct tok {
    enum tok_type type;
    union {
        u32 value;
        char id[ID_MAX];
        enum op op;
        enum brack brack;
    };
};

static const char *op_s[] = {"b.", "bz.", "h.", "hz.", "w.", "*", "/", "%", "+", "-"};

static s32 op_prec[] = {
    /* modes */
    0,
    0,
    0,
    0,
    0,
    /* mul, div, rem */
    1,
    1,
    1,
    /* add, sub */
    2,
    2,
};

static s32 op_ar[] = {
    /* modes */
    1,
    1,
    1,
    1,
    1,
    /* mul, div, rem */
    2,
    2,
    2,
    /* add, sub */
    2,
    2,
};

struct sym {
    const char *id;
    void *addr;
};

static bool make_tok(struct tok *tok, enum tok_type type, ...) {
    tok->type = type;
    va_list arg;
    va_start(arg, type);
    if (type == TOK_CONST) {
        tok->value = va_arg(arg, u32);
    } else if (type == TOK_ID) {
        strcpy(tok->id, va_arg(arg, char *));
    } else if (type == TOK_OP) {
        tok->op = va_arg(arg, enum op);
    } else if (type == TOK_BRACK_L || type == TOK_BRACK_R) {
        tok->brack = va_arg(arg, enum brack);
    }
    va_end(arg);
    return tok;
}

static bool is_func_tok(struct tok *tok) {
    return tok->type == TOK_OP && op_ar[tok->op] == 1;
}

static bool parse_op(const char **p, s32 *op) {
    for (s32 i = 0; i < OP_MAX; ++i) {
        s32 l = strlen(op_s[i]);
        if (strncmp(*p, op_s[i], l) == 0) {
            *p += l;
            *op = i;
            return 1;
        }
    }
    return 0;
}

static bool is_word_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

static char *parse_word(const char **p) {
    const char *e = *p;
    while (is_word_char(*e)) {
        ++e;
    }
    s32 l = e - *p;
    char *s = malloc(l + 1);
    if (!s) {
        return NULL;
    }
    memcpy(s, *p, l);
    s[l] = 0;
    *p = e;
    return s;
}

static void stack_init(struct vector *stack) {
    vector_init(stack, sizeof(struct tok));
}

static struct tok *stack_peek(struct vector *stack) {
    if (stack->size == 0) {
        return NULL;
    } else {
        return vector_at(stack, stack->size - 1);
    }
}

static bool stack_push(struct vector *stack, struct tok *tok) {
    return vector_push_back(stack, 1, tok);
}

static void stack_pop(struct vector *stack, struct tok *tok) {
    *tok = *stack_peek(stack);
    vector_erase(stack, stack->size - 1, 1);
}

enum adex_error adex_parse(struct adex *adex, const char *str) {
    enum adex_error e;
    /* initialize adex */
    stack_init(&adex->expr);
    /* initialize stack */
    struct vector stack;
    stack_init(&stack);
    /* parse string */
    bool mode_op = 0;
    const char *p = str;
    while (*p) {
        char c = *p;
        struct tok tok;
        s32 op;
        /* parse whitespace */
        if (c == ' ' || c == '\t') {
            ++p;
            continue;
        }
        /* parse comment and end-of-line */
        else if (c == '#' || c == '\r' || c == '\n') {
            break;
            /* parse operator */
        } else if (parse_op(&p, &op)) {
            make_tok(&tok, TOK_OP, op);
            /* parse constant */
        } else if (c >= '0' && c <= '9') {
            char *word = parse_word(&p);
            if (!word) {
                goto mem_err;
            }
            char *word_s = word;
            char *word_e = word_s + strlen(word_s);
            s32 base;
            if (strncmp(word, "0x", 2) == 0 || strncmp(word, "0X", 2) == 0) {
                base = 16;
                word_s += 2;
            } else if (strncmp(word, "0b", 2) == 0 || strncmp(word, "0B", 2) == 0) {
                base = 2;
                word_s += 2;
            } else if (strncmp(word, "0", 1) == 0 && word_e - word_s > 1) {
                base = 8;
                word_s += 1;
            } else {
                base = 10;
            }
            if (word_s == word_e) {
                free(word);
                goto syntax_err;
            }
            u32 v = 0;
            u32 m = 1;
            for (char *word_p = word_e - 1; word_p >= word_s; --word_p) {
                char c = *word_p;
                s32 d = -1;
                if (c >= '0' && c <= '9') {
                    d = c - '0';
                } else if (c >= 'A' && c <= 'F') {
                    d = 10 + c - 'A';
                } else if (c >= 'a' && c <= 'f') {
                    d = 10 + c - 'a';
                }
                if (d < 0 || d >= base) {
                    free(word);
                    goto syntax_err;
                }
                v += d * m;
                m *= base;
            }
            free(word);
            make_tok(&tok, TOK_CONST, v);
        }
        /* parse identifier */
        else if (is_word_char(c)) {
            char *word = parse_word(&p);
            if (!word) {
                goto mem_err;
            }
            if (strlen(word) + 1 > ID_MAX) {
                free(word);
                goto syntax_err;
            }
            make_tok(&tok, TOK_ID, word);
            free(word);
        }
        /* parse bracket */
        else if (c == '[') {
            if (mode_op) {
                mode_op = FALSE;
                make_tok(&tok, TOK_BRACK_L, BRACK_SQUARE);
                ++p;
            } else {
                make_tok(&tok, TOK_OP, OP_W);
            }
        } else {
            if (c == '(') {
                make_tok(&tok, TOK_BRACK_L, BRACK_PAREN);
            } else if (c == ']') {
                make_tok(&tok, TOK_BRACK_R, BRACK_SQUARE);
            } else if (c == ')') {
                make_tok(&tok, TOK_BRACK_R, BRACK_PAREN);
            } else {
                goto syntax_err;
            }
            ++p;
        }
        /* process token */
        if (mode_op) {
            goto syntax_err;
        }
        if (tok.type == TOK_ID) {
            // TODO: figure out what this is even doing
            //      bool found = FALSE;
            //      for (s32 i = 0; i < sizeof(syms) / sizeof(*syms); ++i)
            //        if (strcmp(tok.id, syms[i].id) == 0) {
            //          make_tok(&tok, TOK_CONST, (u32)syms[i].addr);
            //          found = TRUE;
            //          break;
            //        }
            //      if (!found)
            //        goto syntax_err;
        }
        if (tok.type == TOK_CONST) {
            if (!stack_push(&adex->expr, &tok)) {
                goto mem_err;
            }
        } else if (tok.type == TOK_BRACK_L || is_func_tok(&tok)) {
            if (!stack_push(&stack, &tok)) {
                goto mem_err;
            }
            if (tok.type == TOK_OP && tok.op >= OP_B && tok.op <= OP_W) {
                mode_op = TRUE;
            }
        } else if (tok.type == TOK_BRACK_R) {
            while (1) {
                if (!stack_peek(&stack)) {
                    goto syntax_err;
                }
                struct tok pop;
                stack_pop(&stack, &pop);
                if (pop.type == TOK_BRACK_L && pop.brack == tok.brack) {
                    break;
                }
                if (!stack_push(&adex->expr, &pop)) {
                    goto mem_err;
                }
            }
            struct tok *top = stack_peek(&stack);
            if (top && is_func_tok(top)) {
                struct tok pop;
                stack_pop(&stack, &pop);
                if (!stack_push(&adex->expr, &pop)) {
                    goto mem_err;
                }
            }
        } else if (tok.type == TOK_OP) {
            while (1) {
                struct tok *top = stack_peek(&stack);
                if (!top || top->type != TOK_OP || op_prec[top->op] < op_prec[tok.op]) {
                    break;
                }
                struct tok pop;
                stack_pop(&stack, &pop);
                if (!stack_push(&adex->expr, &pop)) {
                    goto mem_err;
                }
            }
            if (!stack_push(&stack, &tok)) {
                goto mem_err;
            }
        }
    }
    /* push remaining ops */
    while (stack_peek(&stack)) {
        struct tok pop;
        stack_pop(&stack, &pop);
        if (pop.type != TOK_OP) {
            goto syntax_err;
        }
        if (!stack_push(&adex->expr, &pop)) {
            goto mem_err;
        }
    }
    /* check syntax */
    s32 n_ops = 0;
    for (s32 i = 0; i < adex->expr.size; ++i) {
        struct tok *tok = vector_at(&adex->expr, i);
        if (tok->type == TOK_CONST || tok->type == TOK_ID) {
            ++n_ops;
        } else if (tok->type == TOK_OP) {
            s32 ar = op_ar[tok->op];
            if (n_ops < ar) {
                goto syntax_err;
            }
            n_ops -= ar;
            n_ops += 1;
        }
    }
    if (n_ops != 1) {
        goto syntax_err;
    }
    e = ADEX_ERROR_SUCCESS;
    goto exit;
mem_err:
    e = ADEX_ERROR_MEMORY;
    goto err;
syntax_err:
    e = ADEX_ERROR_SYNTAX;
    goto err;
err:
    adex_destroy(adex);
    goto exit;
exit:
    vector_destroy(&stack);
    return e;
}

static bool validate_addr(u32 addr, s32 size) {
    return addr >= 0x80000000 && addr < 0x80800000 && addr % size == 0;
}

enum adex_error adex_eval(struct adex *adex, u32 *result) {
    enum adex_error e;
    /* initialize stack */
    struct vector stack;
    vector_init(&stack, sizeof(u32));
    /* evaluate sub-expressions */
    for (s32 i = 0; i < adex->expr.size; ++i) {
        struct tok *tok = vector_at(&adex->expr, i);
        u32 value;
        if (tok->type == TOK_CONST) {
            /* push operand */
            value = tok->value;
        } else if (tok->type == TOK_OP) {
            /* compute operation */
            s32 op = tok->op;
            s32 ar = op_ar[op];
            u32 *ops = vector_at(&stack, stack.size - ar);
            bool sign = TRUE;
            switch (op) {
                /* dereference operators */
                case OP_BZ: sign = FALSE;
                case OP_B:
                    if (!validate_addr(ops[0], 1)) {
                        goto addr_err;
                    }
                    value = (sign ? *(s8 *)ops[0] : *(u8 *)ops[0]);
                    break;
                case OP_HZ: sign = FALSE;
                case OP_H:
                    if (!validate_addr(ops[0], 2)) {
                        goto addr_err;
                    }
                    value = (sign ? *(s16 *)ops[0] : *(u16 *)ops[0]);
                    break;
                case OP_W:
                    if (!validate_addr(ops[0], 4)) {
                        goto addr_err;
                    }
                    value = *(u32 *)ops[0];
                    break;
                /* arithmetic operators */
                case OP_ADD: value = ops[0] + ops[1]; break;
                case OP_SUB: value = ops[0] - ops[1]; break;
                case OP_MUL: value = ops[0] * ops[1]; break;
                case OP_DIV:
                    if (ops[1] == 0) {
                        goto arith_err;
                    }
                    value = ops[0] / ops[1];
                    break;
                case OP_REM:
                    if (ops[1] == 0) {
                        goto arith_err;
                    }
                    value = ops[0] % ops[1];
                    break;
            }
            /* pop operands */
            vector_erase(&stack, stack.size - ar, ar);
        }
        /* push result */
        if (!vector_push_back(&stack, 1, &value)) {
            goto mem_err;
        }
    }
    u32 *top = vector_at(&stack, stack.size - 1);
    *result = *top;
    e = ADEX_ERROR_SUCCESS;
    goto exit;
mem_err:
    e = ADEX_ERROR_MEMORY;
    goto exit;
arith_err:
    e = ADEX_ERROR_ARITHMETIC;
    goto exit;
addr_err:
    e = ADEX_ERROR_ADDRESS;
    goto exit;
exit:
    vector_destroy(&stack);
    return e;
}

void adex_destroy(struct adex *adex) {
    vector_destroy(&adex->expr);
}
