#ifndef _H_TRUSTED_HOTCALL_WHILE_
#define _H_TRUSTED_HOTCALL_WHILE_

#include "for.h"
#include <string.h>
#include "hotcall_while.h"
#include "hotcall_config.h"
#include <hotcall-bundler-trusted.h>

static inline void
hotcall_handle_while_begin(struct ecall_queue_item *qi, const struct hotcall_config *hotcall_config, struct queue_context *queue_ctx, struct batch_status * batch_status) {

    struct hotcall_while_start *while_s = qi->call.while_s;
    struct loop_stack_item *loop_stack = queue_ctx->loop_stack;
    unsigned int loop_stack_pos = queue_ctx->loop_stack_pos, loop_offset = (loop_stack_pos > 0 ? loop_stack[loop_stack_pos - 1].offset : 0);

    if(!evaluate_predicate(while_s->config->postfix, while_s->config->postfix_length, hotcall_config, loop_offset)) {
        qi->next = while_s->config->loop_end->next = loop_stack[loop_stack_pos - 1].loop_end;
        loop_stack[queue_ctx->loop_stack_pos - 1].index = 0;
        while_s->config->loop_in_process = false;
        queue_ctx->loop_stack_pos--;
        return;
    }

    if(!while_s->config->loop_in_process) {
        while_s->config->loop_in_process = true;
        qi->next = while_s->config->loop_start;
        loop_stack[loop_stack_pos].loop_start = qi;
        loop_stack[loop_stack_pos].offset = loop_offset;
        queue_ctx->loop_stack_pos++;
    } else {
        if(while_s->config->iter_vectors) {
            loop_stack[loop_stack_pos - 1].offset++;
        }
    }
}

static int iii = 0;
static inline void
hotcall_handle_while_end(struct ecall_queue_item *qi, const struct hotcall_config *hotcall_config, struct queue_context *queue_ctx, struct batch_status * batch_status) {
    struct loop_stack_item *loop_stack = queue_ctx->loop_stack;
    unsigned int loop_stack_pos = queue_ctx->loop_stack_pos;

    if(!(loop_stack[loop_stack_pos - 1].index++)) {
        loop_stack[loop_stack_pos - 1].loop_end = qi->next;
    }
    qi->next = loop_stack[loop_stack_pos - 1].loop_start;
}

#endif
