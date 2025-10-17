/* Extracted from interp.c:54959 (function isBooleanObject). */

	/* StackInterpreter>>#isBooleanObject: */

sqInt
isBooleanObject(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	return (oop == GIV(trueObj))
	 || (oop == GIV(falseObj));
}