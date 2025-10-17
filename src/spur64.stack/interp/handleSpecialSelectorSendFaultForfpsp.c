/* Extracted from interp.c:54501 (function handleSpecialSelectorSendFaultForfpsp). */

/*	Handle a special send fault that may be due to a special selector
	send accessing a forwarded object. obj is forwarded.
	Unforward stack contents and and inst vars and answer obj's target. */

	/* StackInterpreter>>#handleSpecialSelectorSendFaultFor:fp:sp: */

static NoDbgRegParms sqInt
handleSpecialSelectorSendFaultForfpsp(sqInt obj, char *theFP, char *theSP)
{
    sqInt rcvr;
    sqInt referent;

	assert(isOopForwarded(obj));
	followForwardedFrameContentsstackPointer(theFP, theSP);
	rcvr = longAt(theFP + FoxReceiver);
	if (/* isPointers: */
		((!(rcvr & (tagMask()))))
	 && (((byteAt((void *)(rcvr + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)) {
		followForwardedObjectFieldstoDepth(rcvr, 0);
	}

	/* begin followForwarded: */
	assert(isUnambiguouslyForwarder(obj));
	referent = longAt((void *)((obj + BaseHeaderSize) + (0U << (shiftForWord()))));
	while (/* isOopForwarded: */
		((!(referent & (tagMask()))))
	 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
	}
	return referent;
}