/* Extracted from interp.c:54109 (function getCodeCompactionMSecs). */

/*	This is nil in the StackVM */

	/* StackInterpreter>>#getCodeCompactionMSecs */

static sqInt
getCodeCompactionMSecs(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(nilObj);
}