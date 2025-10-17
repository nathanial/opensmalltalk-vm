/* Extracted from interp.c:14372 (function primitiveConstantFill). */

/*	Fill the receiver, which must be an indexable non-pointer
	object, with the given integer value. */

	/* InterpreterPrimitives>>#primitiveConstantFill */

static void
primitiveConstantFill(void)
{
    usqInt end;
    usqLong fillValue;
    sqInt format;
    usqInt i;
    sqInt oddBytes;
    sqInt rcvr;

	/* begin primitiveConstantFillSpur */
	if (argumentCount != 1) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadNumArgs;
		goto l1;
	}
	rcvr = longAt(stackPointer + (1 * BytesPerWord));
	if (!(((!(rcvr & (tagMask()))))
		 && (((format = (byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask()))) >= (sixtyFourBitIndexableFormat())))) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadReceiver;
		goto l1;
	}
	if (
#  if IMMUTABILITY
		((((usqInt)((byteAt((void *)(rcvr + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1) != 0
#  else
		0
#  endif
		) {
		/* primitiveFailFor: */
		primFailCode = PrimErrNoModification;
		goto l1;
	}
	fillValue = positive64BitValueOf(longAt(stackPointer));
	if (primFailCode) {
		/* primitiveFailFor: */
		primFailCode = PrimErrBadArgument;
		goto l1;
	}
	if (format >= (firstShortFormat())) {
		if (format >= (firstByteFormat())) {
			if ((fillValue > 0xFF)
			 || (format >= (firstCompiledMethodFormat()))) {
				/* primitiveFailFor: */
				primFailCode = (fillValue > 0xFF
							? PrimErrBadArgument
							: PrimErrBadReceiver);
				goto l1;
			}
			fillValue = ((fillValue + ((fillValue << 8))) + ((fillValue << 16))) + ((fillValue << 24));
			oddBytes = format & 7;
		}
		else {
			if (fillValue > 0xFFFF) {
				/* primitiveFailFor: */
				primFailCode = PrimErrBadArgument;
				goto l1;
			}
			fillValue += (fillValue << 16);
			oddBytes = ((format & 3) << 1);
		}
		fillValue += (fillValue << 32);
	}
	else {
		if (format == (sixtyFourBitIndexableFormat())) {
			oddBytes = 0;
		}
		else {
			if (fillValue > 0xFFFFFFFFU) {
				/* primitiveFailFor: */
				primFailCode = PrimErrBadArgument;
				goto l1;
			}
			fillValue += (fillValue << 32);
			oddBytes = ((format & 1) << 2);
		}
	}
	end = addressAfter(rcvr);
	i = rcvr + BaseHeaderSize;
	while (i < end) {
		long64Atput((void *)(i),fillValue);
		i += 8;
	}

	/* now ensure trailing bytes are zero */
	if (oddBytes > 0) {
		fillValue = (fillValue) >> (8 * oddBytes);
		long64Atput((void *)(i - 8),fillValue);
	}

	/* begin pop: */
	stackPointer += 1 * BytesPerWord;
	/* end primitiveConstantFillSpur */
l1:;
}