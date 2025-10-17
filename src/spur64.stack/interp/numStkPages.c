/* Extracted from interp.c:58155 (function numStkPages). */

	/* StackInterpreter>>#numStkPages */

static sqInt
numStkPages(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(numStackPages);
}