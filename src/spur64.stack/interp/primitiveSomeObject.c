/* Extracted from interp.c:24075 (function primitiveSomeObject). */

/*	Return the first object in the heap. */

	/* InterpreterPrimitives>>#primitiveSomeObject */

static void
primitiveSomeObject(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt object;
    char *sp;

	/* begin pop: */
	GIV(stackPointer) += (GIV(argumentCount) + 1) * BytesPerWord;
	object = firstAccessibleObject();

	/* begin push: */
	longAtput((sp = GIV(stackPointer) - BytesPerWord),object);
	GIV(stackPointer) = sp;
}