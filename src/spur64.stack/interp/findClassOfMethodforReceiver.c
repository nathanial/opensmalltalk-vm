/* Extracted from interp.c:53207 (function findClassOfMethodforReceiver). */

sqInt
findClassOfMethodforReceiver(sqInt meth, sqInt rcvr)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
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
					? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
					: fetchClassOfNonImm(rcvr)));
		if (rclass != GIV(nilObj)) {
			return rclass;
		}
	}
	if (!((addressCouldBeObj(meth))
		 && (((byteAt((void *)(meth + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		return GIV(nilObj);
	}
	return findClassContainingMethodstartingAt(meth, safeMethodClassOf(meth));
}