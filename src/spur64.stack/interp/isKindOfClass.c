/* Extracted from interp.c:55436 (function isKindOfClass). */

/*	Support for external primitives. */

	/* StackInterpreter>>#is:KindOfClass: */

sqInt
isKindOfClass(sqInt oop, sqInt aClass)
{
    sqInt objOop;
    sqInt oopClass;
    sqInt tagBits;

	oopClass = /* fetchClassOf: */
			((tagBits = oop & (tagMask()))
				? longAt((void *)((classTableFirstPage + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
				: fetchClassOfNonImm(oop));
	while ((oopClass != nilObj)
	 && ((/* isPointers: */
		((!(oopClass & (tagMask()))))
	 && (((byteAt((void *)(oopClass + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
	 && ((numSlotsOfAny(oopClass)) > InstanceSpecificationIndex))) {
		if (oopClass == aClass) {
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