/* Extracted from interp.c:58050 (function nameOfClass). */

/*	Brain-damaged nameOfClass: for C VM. Does *not* answer Foo class for
	metaclasses. Use e.g. classIsMeta: to avoid being fooled. */

	/* StackInterpreter>>#nameOfClass: */

static NoDbgRegParms char *
nameOfClass(sqInt classOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt maybeNameOop;
    sqInt maybeThisClassOop;
    usqInt numSlots;
    usqInt numSlotsUsqInt;

	/* begin numSlotsOf: */
	assert((classIndexOf(classOop)) > (isForwardedObjectClassIndexPun()));
	numSlots = (((numSlotsUsqInt = byteAt((void *)(classOop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
				? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(classOop - BaseHeaderSize)))) << 8)))))) >> 8
				: numSlotsUsqInt);
	if (numSlots == GIV(metaclassNumSlots)) {
		maybeThisClassOop = longAt((void *)((classOop + BaseHeaderSize) + ((((usqInt)(GIV(thisClassIndex)) << (shiftForWord()))))));
		if (addressCouldBeClassObj(maybeThisClassOop)) {
			return nameOfClass(longAt((void *)((classOop + BaseHeaderSize) + ((((usqInt)(GIV(thisClassIndex)) << (shiftForWord())))))));
		}
		return "bad class";
	}
	if (!((numSlots > GIV(classNameIndex))
		 && (((maybeNameOop = longAt((void *)((classOop + BaseHeaderSize) + ((((usqInt)(GIV(classNameIndex)) << (shiftForWord()))))))),
		/* isBytes: */
			((!(maybeNameOop & (tagMask()))))
			 && (((byteAt((void *)(maybeNameOop + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())))))) {
		return "bad class";
	}
	return firstIndexableField(maybeNameOop);
}