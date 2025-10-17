/* Extracted from interp.c:69394 (function primitivePin). */

/*	Pin or unpin the receiver, i.e. make it immobile or mobile, based on the
	argument. Answer whether the object was already pinned. N.B. pinning does
	*not* prevent
	an object from being garbage collected. */

	/* StackInterpreterPrimitives>>#primitivePin */

static void
primitivePin(void)
{
    sqInt boolean;
    sqInt obj;
    char *sp;
    sqInt wasPinned;

	obj = longAt(stackPointer + (1 * BytesPerWord));
	if ((((obj & (tagMask())) != 0))
	 || ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadReceiver;
		return;
	}
	boolean = longAt(stackPointer);
	if (!((boolean == falseObj)
		 || (boolean == trueObj))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		return;
	}
	if ((byteAt((void *)(obj + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) {
		wasPinned = trueObj;
		if (boolean != wasPinned) {
			setIsPinnedOfto(obj, 0);
		}
	}
	else {
		wasPinned = falseObj;
		if (boolean == trueObj) {
			if ((/* isContext: */
				((!(obj & (tagMask()))))
			 && (((longAt((void *)(obj))) & (classIndexMask())) == ClassMethodContextCompactIndex))
			 && (/* isStillMarriedContext: */
				(((((longAt((void *)((obj + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
			 && (!(isWidowedContext(obj))))) {
				/* primitiveFailFor: */
				primFailCode = PrimErrBadReceiver;
				return;
			}
			if (!(pinObject(obj))) {
				/* primitiveFailFor: */
				primFailCode = PrimErrNoMemory;
				return;
			}
		}
	}

	/* begin pop:thenPush: */
	longAtput((sp = stackPointer + (((argumentCount + 1) - 1) * BytesPerWord)),wasPinned);
	stackPointer = sp;
}