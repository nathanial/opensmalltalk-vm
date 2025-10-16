/* Extracted from interp.c:35664 (function followForwarded). */

sqInt
followForwarded(sqInt objOop)
{
    sqInt referent;

	assert(isUnambiguouslyForwarder(objOop));
	referent = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
	while (/* isOopForwarded: */
		((!(referent & (tagMask()))))
	 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
	}
	return referent;
}