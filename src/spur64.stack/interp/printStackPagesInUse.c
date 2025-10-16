/* Extracted from interp.c:60954 (function printStackPagesInUse). */

void
printStackPagesInUse(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt i;
    sqInt n;

	n = 0;
	for (i = 0; i < GIV(numStackPages); i += 1) {
		if (!(isFree(stackPageAt(i)))) {
			printStackPageuseCount(stackPageAt(i), (n += 1));
			cr();
		}
	}
}