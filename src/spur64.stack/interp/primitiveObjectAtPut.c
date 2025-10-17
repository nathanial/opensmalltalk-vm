/* Extracted from interp.c:19976 (function primitiveObjectAtPut). */

/*	Store a literal into a CompiledMethod at the given index. Defined for
	CompiledMethods only.
 */

	/* InterpreterPrimitives>>#primitiveObjectAtPut */

static void
primitiveObjectAtPut(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fieldIndex;
    sqInt index;
    sqInt newValue;
    char *sp;
    sqInt thisReceiver;

	newValue = longAt(GIV(stackPointer));
	index = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if (((!(index & (smallIntegerTag()))))
	 || ((index == ConstOne)
	 && ((!(newValue & (smallIntegerTag())))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	index = (index >> 3);
	thisReceiver = longAt(GIV(stackPointer) + (2 * BytesPerWord));
	if (
#  if IMMUTABILITY
		((((usqInt)((byteAt((void *)(thisReceiver + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0
#  else // IMMUTABILITY
		0
#  endif
		) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNoModification;
		return;
	}
	if (!((index > 0)
		 && (index <= ((literalCountOf(thisReceiver)) + LiteralStart)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadIndex;
		return;
	}
	fieldIndex = index - 1;

	/* begin storePointer:ofObject:withValue: */
	assert(validStorePointerArgs(fieldIndex, thisReceiver, newValue));
	assert(isNonImmediate(thisReceiver));
	if (oopisGreaterThanOrEqualTo(thisReceiver, GIV(oldSpaceStart))) {
		if (/* isYoung: */
			((!(newValue & (tagMask()))))
		 && (oopisLessThan(newValue, GIV(oldSpaceStart)))) {
			/* begin possibleRootStoreInto: */
			if (!((byteAt((void *)(thisReceiver + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
				remember(thisReceiver);
			}
		}
	}

	/* most stores into young objects */
	longAtput((void *)((thisReceiver + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))),newValue);

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (2 * BytesPerWord)),newValue);
	GIV(stackPointer) = sp;
}