/* Extracted from interp.c:35514 (function followFieldofObject). */

static NoDbgRegParms sqInt
followFieldofObject(sqInt fieldIndex, sqInt anObject)
{
    sqInt objOop;

	objOop = longAt((void *)((anObject + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(objOop & (tagMask()))))
	 && ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		objOop = fixFollowedFieldofObjectwithInitialValue(fieldIndex, anObject, objOop);
	}
	return objOop;
}