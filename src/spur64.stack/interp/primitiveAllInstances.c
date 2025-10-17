/* Extracted from interp.c:11757 (function primitiveAllInstances). */

/*	Answer an array of all instances of the receiver that exist
	when the primitive is called, excluding any that may be
	garbage collected as a side effect of allocating the result array. */

	/* InterpreterPrimitives>>#primitiveAllInstances */

EXPORT(void)
primitiveAllInstances(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt delta;
    sqInt result;
    char *sp;


	/* For the mirror prims check that the class obj is actually a valid class. */
	result = allInstancesOf(longAt(GIV(stackPointer)));
	if ((((result) & 7) == 1)) {
		/* begin growToAccomodateContainerWithNumSlots: */
		delta = (BaseHeaderSize * 2) + (((result >> 3)) * BytesPerOop);
		growOldSpaceByAtLeast(((GIV(growHeadroom) < delta) ? delta : GIV(growHeadroom)));
		result = allInstancesOf(longAt(GIV(stackPointer)));
		if ((((result) & 7) == 1)) {
			/* primitiveFailFor: */
			GIV(primFailCode) = PrimErrNoMemory;
			return;
		}
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),result);
	GIV(stackPointer) = sp;
}