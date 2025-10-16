/* Extracted from interp.c:54431 (function handleForwardedSelectorFaultFor). */

static NoDbgRegParms sqInt
handleForwardedSelectorFaultFor(sqInt selectorOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt referent;

	assert(isOopForwarded(selectorOop));

	/* begin followForwardedFieldsInCurrentMethod */
	followForwardedObjectFieldstoDepth(GIV(method), 0);
	followForwardedObjectFieldstoDepth(longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(SpecialSelectors) << (shiftForWord())))))), 0);

	/* begin followForwarded: */
	assert(isUnambiguouslyForwarder(selectorOop));
	referent = longAt((void *)((selectorOop + BaseHeaderSize) + (0U << (shiftForWord()))));
	while (/* isOopForwarded: */
		((!(referent & (tagMask()))))
	 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
		referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
	}
	return referent;
}