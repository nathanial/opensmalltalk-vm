/* Extracted from interp.c:35680 (function followMaybeForwarded). */

	/* SpurMemoryManager>>#followMaybeForwarded: */

static NoDbgRegParms sqInt
followMaybeForwarded(sqInt objOop)
{
	return (/* isOopForwarded: */
		((!(objOop & (tagMask()))))
	 && ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))
			? followForwarded(objOop)
			: objOop);
}