/* Extracted from interp.c:50983 (function checkOkayFields). */

static NoDbgRegParms sqInt
checkOkayFields(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt fieldOop;
    sqInt hasYoung;
    sqInt i;
    sqInt iSqInt;

	if ((oop == null)
	 || (oop == 0)) {
		return 1;
	}
	if ((((oop) & 7) == 1)) {
		return 1;
	}
	if (!(checkOkayOop(oop))) {
		return 0;
	}
	if (!(checkOopHasOkayClass(oop))) {
		return 0;
	}
	if (!((((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) <= 5 /* lastPointerFormat */)
		 || (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		return 1;
	}
	hasYoung = 0 /* (hasSpurMemoryManagerAPI not) */;
	i = (numPointerSlotsOf(oop)) - 1;
	while (i >= 0) {
		fieldOop = longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(i) << (shiftForWord()))))));
		if (!((((fieldOop) & 7) == 1))) {
			hasYoung = hasYoung
				 || (/* isYoung: */
					((!(fieldOop & (tagMask()))))
				 && (oopisLessThan(fieldOop, GIV(oldSpaceStart))));
			if (!(checkOkayOop(fieldOop))) {
				return 0;
			}
			if (!(checkOopHasOkayClass(fieldOop))) {
				return 0;
			}
		}
		i -= 1;
	}
	if (hasYoung) {
		/* begin checkOkayYoungReferrer: */
		if (oopisLessThan(oop, GIV(oldSpaceStart))) {
			return 1;
		}
		if (!((byteAt((void *)(oop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift())))) {
			fprintf(GIV(transcript),
					"remembered bit is not set in %p\n",
					((void *)oop));
			return 0;
		}

		/* begin isInRememberedSet: */
		for (iSqInt = 0; iSqInt < GIV(rememberedSetSize); iSqInt += 1) {
			if ((GIV(rememberedSet)[iSqInt]) == oop) {
				return 1;
			}
		}
		fprintf(GIV(transcript),
				"%p has remembered bit set but is not in remembered set\n",
				((void *)oop));
		return 0;
	}
	return 1;
}