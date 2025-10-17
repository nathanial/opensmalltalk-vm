/* Extracted from interp.c:54458 (function handleForwardedSendFaultForTag). */

/*	Handle a send fault that may be due to a send to a forwarded object.
	Unforward the receiver on the stack and answer its actual class. */

	/* StackInterpreter>>#handleForwardedSendFaultForTag: */

static NoDbgRegParms sqInt
handleForwardedSendFaultForTag(sqInt classTag)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt rcvr;
    sqInt referent;
    sqInt tagBits;

	assert(isForwardedClassTag(classTag));
	rcvr = longAt(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord));

	/* should *not* be a super send, so the receiver should be forwarded. */
	assert(isOopForwarded(rcvr));

	/* begin followForwarded: */
	assert(isUnambiguouslyForwarder(rcvr));
	referent = longAt((void *)((rcvr + BaseHeaderSize) + (0U << (shiftForWord()))));
	while (/* isOopForwarded: */
		((!(referent & (tagMask()))))
	 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
	}
	rcvr = referent;

	/* stackValue:put: */
	longAtput(GIV(stackPointer) + (GIV(argumentCount) * BytesPerWord),rcvr);
	followForwardedFrameContentsstackPointer(GIV(framePointer), GIV(stackPointer) + ((GIV(argumentCount) + 1) * BytesPerWord));
	if (/* isPointers: */
		((!((longAt(GIV(framePointer) + FoxReceiver)) & (tagMask()))))
	 && (((byteAt((void *)((longAt(GIV(framePointer) + FoxReceiver)) + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)) {
		followForwardedObjectFieldstoDepth(longAt(GIV(framePointer) + FoxReceiver), 0);
	}
	return /* fetchClassTagOf: */
		((tagBits = rcvr & (tagMask()))
			? tagBits
			: (longAt((void *)(rcvr))) & (classIndexMask()));
}