/* Extracted from interp.c:18680 (function primitiveIsBigEnder). */

EXPORT(sqInt)
primitiveIsBigEnder(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	/* begin pop:thenPushBool: */
	longAtput((sp = GIV(stackPointer)),GIV(falseObj));
	GIV(stackPointer) = sp;
	return 0;
}