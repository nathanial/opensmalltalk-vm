/* Extracted from interp.c:19434 (function primitiveNew). */

	/* InterpreterPrimitives>>#primitiveNew */

static void
primitiveNew(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt obj;
    sqInt reasonCode;
    char *sp;


	/* For the mirror prims check that the class obj is actually a valid class. */
	if ((obj = instantiateClass(longAt(GIV(stackPointer))))) {
		/* begin pop:thenPush: */
		longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),obj);
		GIV(stackPointer) = sp;
	}
	else {
		reasonCode = (isFixedSizePointerFormat((((usqInt)((((longAt((void *)(((longAt(GIV(stackPointer))) + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)))) >> (fixedFieldsFieldWidth())) & (formatMask()))
					? PrimErrNoMemory
					: PrimErrBadReceiver);

		/* begin primitiveFailFor: */
		GIV(primFailCode) = reasonCode;
	}
}