/* Extracted from interp.c:40956 (function noFixupFollowFieldofObject). */

static NoDbgRegParms sqInt
noFixupFollowFieldofObject(sqInt fieldIndex, sqInt anObject)
{
    sqInt objOop;
    sqInt referent;

	objOop = longAt((void *)((anObject + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
	if (/* isOopForwarded: */
		((!(objOop & (tagMask()))))
	 && ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		/* begin followForwarded: */
		assert(isUnambiguouslyForwarder(objOop));
		referent = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
		while (/* isOopForwarded: */
			((!(referent & (tagMask()))))
		 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
		}
		objOop = referent;
	}
	return objOop;
}