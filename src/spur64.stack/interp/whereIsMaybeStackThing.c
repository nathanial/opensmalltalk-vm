/* Extracted from interp.c:26411 (function whereIsMaybeStackThing). */

static NoDbgRegParms char *
whereIsMaybeStackThing(sqInt anOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	if (oopisGreaterThanOrEqualToandLessThan(anOop, GIV(stackMemory), GIV(pages))) {
		return " is in the stack zone";
	}
	return null;
}