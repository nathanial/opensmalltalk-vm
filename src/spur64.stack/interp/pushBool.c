/* Extracted from interp.c:61182 (function pushBool). */

	/* StackInterpreter>>#pushBool: */

void
pushBool(sqInt trueOrFalse)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt object;
    char *sp;

	object = /* booleanObjectOf: */
			(trueOrFalse
				? GIV(trueObj)
				: GIV(falseObj));

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;
}