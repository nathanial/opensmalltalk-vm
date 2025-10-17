/* Extracted from interp.c:61211 (function pushFloat). */

	/* StackInterpreter>>#pushFloat: */

void
pushFloat(double f)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt object;
    char *sp;

	object = floatObjectOf(f);

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;
}