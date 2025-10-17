/* Extracted from interp.c:61225 (function pushInteger). */

	/* StackInterpreter>>#pushInteger: */

sqInt
pushInteger(sqInt integerValue)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *sp;

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),(((usqInt)integerValue << 3) | 1));
	GIV(stackPointer) = sp;
	return null;
}