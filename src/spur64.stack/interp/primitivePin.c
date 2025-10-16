/* Extracted from interp.c:69394 (function primitivePin). */

static void
primitivePin(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt boolean;
    sqInt obj;
    char *sp;
    sqInt wasPinned;

	obj = longAt(GIV(stackPointer) + (1 * BytesPerWord));
	if ((((obj & (tagMask())) != 0))
	 || ((!((longAt((void *)(obj))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadReceiver;
		return;
	}
	boolean = longAt(GIV(stackPointer));
	if (!((boolean == GIV(falseObj))
		 || (boolean == GIV(trueObj)))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadArgument;
		return;
	}
	if ((byteAt((void *)(obj + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))) {
		wasPinned = GIV(trueObj);
		if (boolean != wasPinned) {
			setIsPinnedOfto(obj, 0);
		}
	}
	else {
		wasPinned = GIV(falseObj);
		if (boolean == GIV(trueObj)) {
			if ((/* isContext: */
				((!(obj & (tagMask()))))
			 && (((longAt((void *)(obj))) & (classIndexMask())) == ClassMethodContextCompactIndex))
			 && (/* isStillMarriedContext: */
				(((((longAt((void *)((obj + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1))
			 && (!(isWidowedContext(obj))))) {
				/* primitiveFailFor: */
				GIV(primFailCode) = PrimErrBadReceiver;
				return;
			}
			if (!(pinObject(obj))) {
				/* primitiveFailFor: */
				GIV(primFailCode) = PrimErrNoMemory;
				return;
			}
		}
	}

	/* begin pop:thenPush: */
	longAtput((sp = GIV(stackPointer) + (((GIV(argumentCount) + 1) - 1) * BytesPerWord)),wasPinned);
	GIV(stackPointer) = sp;
}