/* Extracted from interp.c:65409 (function unfollowFirstLiteralOfMaybeCalloutMethodprimitiveIndex). */

static NoDbgRegParms sqInt
unfollowFirstLiteralOfMaybeCalloutMethodprimitiveIndex(sqInt methodObj, sqInt primIndex)
{
    sqInt firstLiteral;
    sqInt found;

	found = 0;

	/* inlined self literal: 0 ofMethod: methodObj for clarity... */
	firstLiteral = longAt((void *)((methodObj + BaseHeaderSize) + ((((usqInt)(LiteralStart) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(firstLiteral & (tagMask()))))
	 && ((!((longAt((void *)(firstLiteral))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		found = 1;
		firstLiteral = fixFollowedFieldofObjectwithInitialValue(LiteralStart, methodObj, firstLiteral);
	}
	if (followForwardedObjectFieldstoDepth(firstLiteral, (primIndex == PrimNumberFFICall
			? (primitiveAccessorDepthTable[PrimNumberFFICall]) - 1
			: 0))) {
		found = 1;
	}
	return found;
}