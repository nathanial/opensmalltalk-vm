/* Extracted from interp.c:19626 (function primitiveNewWithArg). */

/*	Allocate a new indexable instance. Fail if the allocation would leave less
	than lowSpaceThreshold bytes free. May cause a GC.
 */

	/* InterpreterPrimitives>>#primitiveNewWithArg */

static void
primitiveNewWithArg(void)
{
    sqInt instSpec;
    sqInt obj;
    sqInt reasonCode;
    usqIntptr_t size;
    char *sp;
    sqInt value;


	/* For the mirror prims check that the class obj is actually a valid class. */

	/* begin positiveMachineIntegerValueOf: */
	if (((((longAt(stackPointer))) & 7) == 1)) {
		value = ((longAt(stackPointer)) >> 3);
		if (value < 0) {
			/* begin primitiveFail */
			if (!primFailCode) {
				primFailCode = 1;
			}
			size = null;
			goto l1;
		}
		size = value;
		goto l1;
	}

	/* don't inline the rare case */
	size = positiveMachineIntegerValueOfObj(longAt(stackPointer));
	/* end positiveMachineIntegerValueOf: */
l1:
	if (primFailCode) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}

	/* positiveMachineIntegerValueOf: succeeds only for non-negative integers. */
	if ((obj = instantiateClassindexableSize(longAt(stackPointer + (1 * BytesPerWord)), size))) {
		/* begin pop:thenPush: */
		longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),obj);
		stackPointer = sp;
	}
	else {
		instSpec = (((usqInt)((((longAt((void *)(((longAt(stackPointer + (1 * BytesPerWord))) + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)))) >> (fixedFieldsFieldWidth())) & (formatMask());
		reasonCode = ((/* isIndexableFormat: */
				(instSpec >= (arrayFormat()))
			 && ((instSpec <= (weakArrayFormat()))
			 || (instSpec >= (sixtyFourBitIndexableFormat()))))
			 && (!(instSpec >= (firstCompiledMethodFormat())))
					? PrimErrNoMemory
					: PrimErrBadReceiver);

		/* begin primitiveFailFor: */
		primFailCode = reasonCode;
	}
}