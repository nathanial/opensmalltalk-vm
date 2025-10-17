/* Extracted from interp.c:54360 (function getMaxLiteralCountForCompile). */

/*	This is nil in the StackVM */

	/* StackInterpreter>>#getMaxLiteralCountForCompile */

static sqInt
getMaxLiteralCountForCompile(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return GIV(nilObj);
}