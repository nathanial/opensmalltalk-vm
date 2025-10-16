/* Extracted from interp.c:62284 (function retryPrimitiveOnFailure). */

static sqInt
retryPrimitiveOnFailure(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt accessorDepth;
    sqInt canRetry;
    usqInt firstBytecode;
    sqInt followDone;
    sqInt gcDone;
    sqInt index;
    sqInt methodHeader;
    sqInt oop;
    sqInt primIndex;
    sqInt primitiveIndex;
    sqInt referent;
    sqInt retried;
    sqInt retry;
    sqInt scannedStackFrame;

	/* begin primitiveIndexOf: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(GIV(newMethod)));
	methodHeader = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
		firstBytecode = (GIV(newMethod) + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize;
		primitiveIndex = (byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
	}
	else {
		primitiveIndex = 0;
	}
	assert(saneFunctionPointerForFailureOfPrimIndex(primitiveIndex));
	gcDone = 0;
	followDone = (canRetry = (retried = 0));
	while (1) {
		retry = 0;
		if (GIV(primFailCode) == PrimErrNoMemory) {
			if (((gcDone += 1)) == 1) {
				canRetry = primitiveIndex == PrimNumberExternalCall;
			}
			if (canRetry) {
				if (gcDone == 1) {
					/* begin scavengingGC */
					scavengingGCTenuringIf(TenureByAge);
				}
				if (gcDone == 2) {
					fullGC();
				}
				retry = gcDone <= 2;
			}
		}
		else {
			if (!followDone) {
				followDone = 1;

				/* begin checkForAndFollowForwardedPrimitiveState */
				assert(failed());
				retry = 0;

				/* begin primitiveIndexOf: */
				/* begin methodHeaderOf: */
				assert(isCompiledMethod(GIV(newMethod)));
				methodHeader = longAt((void *)((GIV(newMethod) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
				if (((methodHeader & AlternateHeaderHasPrimFlag) != 0)) {
					firstBytecode = (GIV(newMethod) + ((LiteralStart + (((methodHeader >> 3)) & AlternateHeaderNumLiteralsMask)) * BytesPerOop)) + BaseHeaderSize;
					primIndex = (byteAt((void *)(firstBytecode + 1))) + ((((usqInt)((byteAt((void *)(firstBytecode + 2)))) << 8)));
				}
				else {
					primIndex = 0;
				}
				assert((GIV(argumentCount) == (argumentCountOf(GIV(newMethod))))
				 || (isMetaPrimitiveIndex(primIndex)));

				/* First things first; make sure the metadata has been followed before it is accessed to derive accessorDepth... */
				if (((/* isCalloutPrimitiveIndex: */
					(primIndex == PrimNumberExternalCall)
				 || (primIndex == PrimNumberFFICall))
				 || (primIndex == PrimNumberDoExternalCall))
				 && (unfollowFirstLiteralOfMaybeCalloutMethodprimitiveIndex(GIV(newMethod), primIndex))) {
					retry = 1;
				}

				/* If the primitive is one of the meta primitives PrimNumberDoPrimitive or PrimNumberDoExternalCall, then
				   metaAccessorDepth will have been set to nil at the start of the primitive, and to the accessor depth of the
				   called primitive (or external call) immediately before dispatch.  Hence if primIndex is that of a meta primitive
				   then if metaAccessorDepth is -2, the accessor depth is that of the meta primitive, and if > -2, then
				   metaAccessorDepth is the accessor depth of the primitive (or external call).  Similarly, if the primitive is
				   primitiveExternalCall then the accessor depth is that of primitiveExternalCall until primitiveFunctionPointer
				   is assigned, at which point the accessor depth is taken from the slot in newMethod's first literal. */
				accessorDepth = ((/* isMetaPrimitiveIndex: */
						(primIndex == PrimNumberDoPrimitive)
					 || (primIndex == PrimNumberDoExternalCall))
					 && (GIV(metaAccessorDepth) > -2)
							? GIV(metaAccessorDepth)
							: accessorDepthForPrimitiveMethod(GIV(newMethod)));
				assert(((accessorDepth >= -1) && (accessorDepth <= 5)));
				if (accessorDepth >= 0) {
					scannedStackFrame = 0;
					for (index = 0; index <= GIV(argumentCount); index += 1) {
						oop = longAt((void *)(GIV(stackPointer) + (index * BytesPerWord)));
						if ((!(oop & (tagMask())))) {
							if ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
								assert(index < GIV(argumentCount));
								retry = 1;

								/* begin followForwarded: */
								assert(isUnambiguouslyForwarder(oop));
								referent = longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
								while (/* isOopForwarded: */
									((!(referent & (tagMask()))))
								 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
									referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
								}
								oop = referent;

								/* stackValue:put: */
								longAtput((void *)(GIV(stackPointer) + (index * BytesPerWord)),oop);
								if (!scannedStackFrame) {
									scannedStackFrame = 1;

									/* Avoid repeated primitive failures by following all state in the current stack frame. */
									followForwardedFrameContentsstackPointer(GIV(framePointer), GIV(stackPointer) + ((GIV(argumentCount) + 1) * BytesPerWord));
								}
							}
							if ((accessorDepth > 0)
							 && ((/* hasPointerFields: */
								((!(oop & (tagMask()))))
							 && (hasPointerFieldsNonImm(oop)))
							 && (followForwardedObjectFieldstoDepth(oop, accessorDepth)))) {
								retry = 1;
							}
						}
					}
				}
			}
		}
		if (!(retry)) break;
		assert(GIV(primFailCode) != 0);
		retried = 1;

		/* begin initPrimCall */
		GIV(primFailCode) = 0;
		dispatchFunctionPointer(primitiveFunctionPointer);
	}
	return retried;
}