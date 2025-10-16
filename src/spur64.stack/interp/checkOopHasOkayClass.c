/* Extracted from interp.c:32897 (function checkOopHasOkayClass). */

sqInt
checkOopHasOkayClass(usqInt obj)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    usqInt numSlots;
    usqInt objClass;
    sqInt objFormat;

	if (!(checkOkayOop(obj))) {
		return 0;
	}
	objClass = ((usqInt) (fetchClassOfNonImm(obj)));
	if (((objClass & (tagMask())) != 0)) {
		fprintf(GIV(transcript),
				"obj %p an immediate is not a valid class or behavior\n",
				((void *)obj));
		return 0;
	}
	if (!(okayOop(objClass))) {
		fprintf(GIV(transcript),
				"obj %p class obj is not ok\n",
				((void *)obj));
		return 0;
	}
	if (!((((byteAt((void *)(objClass + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
		 && (((/* begin numSlotsOf: */
			assert((classIndexOf(objClass)) > (isForwardedObjectClassIndexPun())),
		(((numSlots = byteAt((void *)(objClass + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(objClass - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots))) >= 3))) {
		fprintf(GIV(transcript),
				"obj %p a class (behavior) must be a pointers object of size >= 3\n",
				((void *)obj));
		return 0;
	}
	objFormat = (/* isBytes: */
			((!(obj & (tagMask()))))
		 && (((byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat()))
				? ((((byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask())) | 7) - 7)
				: (byteAt((void *)(obj + (formatFieldByteOffset())))) & (formatMask()));
	if (((((usqInt)((((longAt((void *)((objClass + BaseHeaderSize) + ((((usqInt)(InstanceSpecificationIndex) << (shiftForWord()))))))) >> 3)))) >> (fixedFieldsFieldWidth())) & (formatMask())) != objFormat) {
		fprintf(GIV(transcript),
				"obj %p and its class (behavior) formats differ\n",
				((void *)obj));
		return 0;
	}
	return 1;
}