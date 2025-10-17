/* Extracted from interp.c:55472 (function isKindOf). */

/*	Support for external primitives. */

	/* StackInterpreter>>#is:KindOf: */

sqInt
isKindOf(sqInt oop, char *className)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objOop;
    sqInt oopClass;
    sqInt tagBits;

	oopClass = /* fetchClassOf: */
			((tagBits = oop & (tagMask()))
				? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
				: fetchClassOfNonImm(oop));
	while (!(oopClass == GIV(nilObj))) {
		if (classNameOfIs(oopClass, className)) {
			return 1;
		}

		/* begin superclassOf: */
		/* begin followObjField:ofObject: */
		objOop = longAt((void *)((oopClass + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
		assert(isNonImmediate(objOop));
		if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, oopClass, objOop);
		}
		oopClass = objOop;
	}
	return 0;
}