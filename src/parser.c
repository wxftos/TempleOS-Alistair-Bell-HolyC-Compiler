#include "parser.h"
#include "parser_errors.h"
#include "defs.h"
#include "util.h"

struct state_machine {
	struct error error;
	unsigned int active_modifiers;
	unsigned int doubled_modifiers;
	unsigned int scope_param;
	unsigned int scope_global;
	unsigned int scope_index;
};
static const char *src_chars;

#define BIT(x) \
	(1 << ((unsigned int)x))

static int
validate_new_modifiers(struct token *target, struct state_machine *mach)
{
	/* 
	 * Some assigment modifiers may be applied twice, or only once.
	 * However modifiers such as `static` and `extern` conflict and will be invalid if apply in conjunction.
	 */
	register enum token_modifier mod = (enum token_modifier)target->type;
	if (mod == MODIFIER_const) {
		/* Means that the const modifier is already been applied. */
		if ((mach->active_modifiers & BIT(mod)) != 0) {
			if ((mach->doubled_modifiers & BIT(mod)) != 0) {
				/* Throw an error that there are too many constant modifiers. */
				mach->error.offender = target;
				mach->error.value = ERROR_REPEATED_MODIFIER;
				return -1;
			} else {
				mach->doubled_modifiers |= BIT(mod);
				return 0;
			}
		} 
	} else {
		static const enum token_modifier mapping[] = {
			[MODIFIER_static] = MODIFIER_extern,
			[MODIFIER_extern] = MODIFIER_static,
		};
		if ((mach->active_modifiers & BIT(mapping[mod])) != 0) {
			mach->error.offender = target;
			mach->error.value = ERROR_CONFLICTING_MODIFIER;
			return -1;
		}
	}
	mach->active_modifiers |= BIT(mod);
	return 0;
}
static int
throw_error(struct state_machine *mach)
{
	char cpy[256];
	register struct token const *t = mach->error.offender;
	strncpy(cpy, src_chars + t->offset, t->diff);
	cpy[t->diff] = (char)0;
	fprintf(stderr, "error: parser violation, %s, offender \'%s\', line %u!\n", error_str[(int)mach->error.value], cpy, t->line);
	return -1;
}
static int
parse_expression(struct token *tail, struct token *head, struct state_machine *mach)
{
	struct token *inc = tail;
	do {
		switch (inc->category) {
			case TOKEN_MODIFIER: {
				if (validate_new_modifiers(inc, mach) < 0) {
					return throw_error(mach);
				}
			}
		}
		++inc;
	} while (inc != head);

	return -1;	
}
int
parse_tokens(struct token *tokens, unsigned int count, char *chars)
{
	src_chars = chars;


	struct state_machine *mach = (struct state_machine *)calloc(1, sizeof(*mach));
	if (!mach) {
		register int snap = errno;
		fprintf(stderr, "error: failed to allocate parser state machine. %s\n", strerror(snap));
		return -1;
	}
	struct token *head = &(*tokens);
	struct token *tail = head;
	for (; head < tokens + count; ++head) {
		if (head->category == TOKEN_TERMINATOR) {
			/* This signifies the end of an expression, we need to parse this segment.  */
			if (parse_expression(tail, head, mach) < 0) {
				free((void *)mach);
				return -1;
			}
			tail = head + 1;
		}
	}
	free((void*)mach);
	return 0;
}


