/* Extracted from interp.c:54501 (function handleSpecialSelectorSendFaultForfpsp). */

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