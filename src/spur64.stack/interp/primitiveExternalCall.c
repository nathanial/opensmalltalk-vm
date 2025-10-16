/* Extracted from interp.c:68214 (function primitiveExternalCall). */

static void
primitiveExternalCall(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void (*addr)(void);
    sqInt index;
    sqInt lit;
    usqInt numSlots;
    sqInt reasonCode;

	if (!((/* isOopCompiledMethod: */
			((!(GIV(newMethod) & (tagMask()))))
		 && (((byteAt((void *)(GIV(newMethod) + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))
		 && (((literalCountOf(GIV(newMethod))) > 0)
		 && (((lit = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + (1U << (shiftForWord()))))),
		(/* isArray: */
				((!(lit & (tagMask()))))
			 && (((byteAt((void *)(lit + (formatFieldByteOffset())))) & (formatMask())) == (arrayFormat())))
			 && ((((/* begin numSlotsOf: */
				assert((classIndexOf(lit)) > (isForwardedObjectClassIndexPun())),
			(((numSlots = byteAt((void *)(lit + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
						? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(lit - BaseHeaderSize)))) << 8)))))) >> 8
						: numSlots))) == 4)
			 && (((index = longAt((void *)((lit + BaseHeaderSize) + ((((usqInt)(ExternalCallLiteralTargetFunctionIndex) << (shiftForWord()))))))),
			(((index) & 7) == 1))))))))) {
		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrBadMethod;
		return;
	}
	index = (index >> 3);

	/* Check if we have already looked up the function and failed. */
	if (index < 0) {
		rewriteMethodCacheEntryForExternalPrimitiveToFunction(0);

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNotFound;
		return;
	}

	/* Function address was not found in this session,
	   Void the primitive function.
	   Try to call the function directly */
	if ((index > 0)
	 && (index <= MaxExternalPrimitiveTableSize)) {
		addr = externalPrimitiveTable[index - 1];
		if (addr) {
			rewriteMethodCacheEntryForExternalPrimitiveToFunction(addr);

			/* begin callExternalPrimitive: */
			primitiveFunctionPointer = addr;
			dispatchFunctionPointer(addr);

			/* begin maybeRetryPrimitiveOnFailure */
			if (GIV(primFailCode)) {
				retryPrimitiveOnFailure();
			}
			return;
		}

		/* if we get here, then an index to the external prim was
		   kept on the ST side although the underlying prim
		   table was already flushed */

		/* primitiveFailFor: */
		GIV(primFailCode) = PrimErrNamedInternal;
		return;
	}

	/* Clean up session id/metadata and external primitive index */

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(lit))
	 && (!(isForwarded(lit))));
	assert(validStorePointerUncheckedArgs(ExternalCallLiteralFlagsIndex, lit, ConstZero));
	longAtput((void *)((lit + BaseHeaderSize) + ((((usqInt)(ExternalCallLiteralFlagsIndex) << (shiftForWord()))))),ConstZero);

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(lit))
	 && (!(isForwarded(lit))));
	assert(validStorePointerUncheckedArgs(ExternalCallLiteralTargetFunctionIndex, lit, ConstZero));
	longAtput((void *)((lit + BaseHeaderSize) + ((((usqInt)(ExternalCallLiteralTargetFunctionIndex) << (shiftForWord()))))),ConstZero);

	/* The function has not been loaded yet. Attempt to link it, cache it, and call it. */
	addr = linkExternalCallerrInto(lit, (&GIV(primFailCode)));
	if (!addr) {
		assert((fetchPointerofObject(ExternalCallLiteralFlagsIndex, lit)) == ConstZero);
		reasonCode = (GIV(primFailCode)
					? GIV(primFailCode)
					: PrimErrNotFound);
		GIV(primFailCode) = reasonCode;
		return;
	}

	/* begin callExternalPrimitive: */
	primitiveFunctionPointer = addr;
	dispatchFunctionPointer(addr);

	/* begin maybeRetryPrimitiveOnFailure */
	if (GIV(primFailCode)) {
		retryPrimitiveOnFailure();
	}
}