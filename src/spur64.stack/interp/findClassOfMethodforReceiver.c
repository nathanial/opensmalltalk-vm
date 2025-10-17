/* Extracted from interp.c:53207 (function findClassOfMethodforReceiver). */

	/* StackInterpreter>>#findClassOfMethod:forReceiver: */

sqInt
findClassOfMethodforReceiver(sqInt meth, sqInt rcvr)
{
    sqInt rclass;
    sqInt tagBits;

	if ((/* addressCouldBeOop: */
		(((rcvr & (tagMask())) != 0))
	 || (addressCouldBeObj(rcvr)))
	 && (!(/* isOopForwarded: */
		((!(rcvr & (tagMask()))))
	 && ((!((longAt((void *)(rcvr))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))))) {
		rclass = findClassContainingMethodstartingAt(meth, /* fetchClassOf: */
				((tagBits = rcvr & (tagMask()))
					? longAt((void *)((classTableFirstPage + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
					: fetchClassOfNonImm(rcvr)));
		if (rclass != nilObj) {
			return rclass;
		}
	}
	if (!((addressCouldBeObj(meth))
		 && (((byteAt((void *)(meth + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		return nilObj;
	}
	return findClassContainingMethodstartingAt(meth, safeMethodClassOf(meth));
}