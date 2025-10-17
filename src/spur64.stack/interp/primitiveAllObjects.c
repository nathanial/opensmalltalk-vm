/* Extracted from interp.c:11790 (function primitiveAllObjects). */

/*	Answer an array of all objects that exist when the primitive
	is called, excluding those that may be garbage collected as
	a side effect of allocating the result array. */

	/* InterpreterPrimitives>>#primitiveAllObjects */

EXPORT(void)
primitiveAllObjects(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt delta;
    sqInt result;
    char *sp;

	result = allObjects();
	if ((((result) & 7) == 1)) {
		/* begin growToAccomodateContainerWithNumSlots: */
		delta = (BaseHeaderSize * 2) + (((result >> 3)) * BytesPerOop);
		growOldSpaceByAtLeast(((growHeadroom < delta) ? delta : growHeadroom));
		result = allObjects();
		if ((((result) & 7) == 1)) {
			/* primitiveFailFor: */
			primFailCode = PrimErrNoMemory;
			return;
		}
	}

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),result);
	stackPointer = sp;
}