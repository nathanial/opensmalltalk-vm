/* Extracted from interp.c:26228 (function startOfAlienData). */

/*	Answer the start of the Alien's data or fail if oop is not an Alien. */

	/* InterpreterPrimitives>>#startOfAlienData: */

void *
startOfAlienData(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objOop;
    sqInt oopClass;
    sqInt tagBits;

	/* begin is:KindOfClass: */
	oopClass = /* fetchClassOf: */
			((tagBits = oop & (tagMask()))
				? longAt((void *)((GIV(classTableFirstPage) + BaseHeaderSize) + ((((usqInt)(tagBits) << (shiftForWord()))))))
				: fetchClassOfNonImm(oop));
	while ((oopClass != GIV(nilObj))
	 && ((/* isPointers: */
		((!(oopClass & (tagMask()))))
	 && (((byteAt((void *)(oopClass + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */))
	 && ((numSlotsOfAny(oopClass)) > InstanceSpecificationIndex))) {
		if (oopClass == (longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassAlien) << (shiftForWord())))))))) {
			goto l1;
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

	/* primitiveFailFor: */
	GIV(primFailCode) = PrimErrBadArgument;
	return 0;
l1:
	return ((void *) (((longAt((void *)(oop + BaseHeaderSize))) > 0
		? (oop + BaseHeaderSize) + BytesPerOop
		: longAt((void *)((oop + BaseHeaderSize) + BytesPerOop)))));
}