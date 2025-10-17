/* Extracted from interp.c:55629 (function longPrintOop). */

/*	useful for VM debugging */

	/* StackInterpreter>>#longPrintOop: */

void
longPrintOop(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aClass;
    int byte;
    char *className;
    sqInt classPointer;
    sqInt cls;
    sqInt column;
    sqInt contextSize;
    sqInt fieldOop;
    sqInt fmt;
    sqInt fmtSqInt;
    sqInt header;
    sqInt i;
    usqInt index;
    sqInt lastIndex;
    usqInt lastPointer;
    sqInt length;
    sqInt n;
    usqInt numLiterals;
    usqInt numSlots;
    usqInt numSlotsUsqInt;
    sqInt objOop;
    sqInt oopClass;
    sqInt sp;
    sqInt tagBits;

	length = 0;
	if (((oop & (tagMask())) != 0)) {
		printImmediateObjecton(oop, GIV(transcript));
		return;
	}
	if (!(addressCouldBeObj(oop))) {
		printCantBeObjecton(oop, GIV(transcript));
		return;
	}
	if (((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
		printFreeObjecton(oop, GIV(transcript));
		return;
	}
	if ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		printForwarderon(oop, GIV(transcript));
		return;
	}
	if ((cls = fetchClassOfNonImm(oop))) {
		className = nameOfClasslengthInto(cls, (&length));
		fprintf(GIV(transcript),
				"%p: a(n) %.*s",
				((void *)oop),
				((int) length),
				className);
		fprintf(GIV(transcript),
				"(%" PRIxSQINT "=>%p)",
				(longAt((void *)(oop))) & (classIndexMask()),
				((void *)cls));
	}
	else {
		fprintf(GIV(transcript),
				"%p has a nil class!!\n",
				((void *)oop));
	}
	fmt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	fprintf(GIV(transcript),
			" format %" PRIxSQINT "",
			fmt);
	if (fmt > 5 /* lastPointerFormat */) {
		fprintf(GIV(transcript),
				" nbytes %" PRIdSQINT "",
				numBytesOf(oop));
	}
	else {
		if (/* isIndexableFormat: */
			(fmt >= (arrayFormat()))
		 && ((fmt <= (weakArrayFormat()))
		 || (fmt >= (sixtyFourBitIndexableFormat())))) {
			/* begin lengthOf: */
			fmtSqInt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
			numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset())));
			numSlots = (numSlotsUsqInt == (numSlotsMask())
						? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
						: numSlotsUsqInt);
			if (fmtSqInt <= (ephemeronFormat())) {
				length = numSlots;
				goto l1;
			}
			if (fmtSqInt >= (firstByteFormat())) {
				length = ((numSlots << (shiftForWord()))) - (fmtSqInt & 7);
				goto l1;
			}

			/* bytes, including CompiledMethod */
			if (fmtSqInt >= (firstShortFormat())) {
				length = ((numSlots << ((shiftForWord()) - 1))) - (fmtSqInt & 3);
				goto l1;
			}
			if (fmtSqInt >= (firstLongFormat())) {
				length = ((numSlots << ((shiftForWord()) - 2))) - (fmtSqInt & 1);
				goto l1;
			}
			if (fmtSqInt == (sixtyFourBitIndexableFormat())) {
				length = numSlots;
				goto l1;
			}

			/* fmt = self forwardedFormat */
			length = 0;
			/* end lengthOf: */
l1:
			fprintf(GIV(transcript),
					" size %" PRIdSQINT "",
					length - (fixedFieldsOfformatlength(oop, fmt, length)));
		}
	}

	/* begin printHeaderTypeOf:on: */
	fprintf(GIV(transcript),
			" hdr%d %c%c%c%c%c",
			((byteAt((void *)(oop + (numSlotsFieldByteOffset())))) == (numSlotsMask())
			? 16
			: 8),
			((((usqInt)((byteAt((void *)(oop + (immutableExtraBitsByteOffset())))))) >> (immutableBitByteShift())) & 1
			? 'i'
			: '.'),
			((byteAt((void *)(oop + (formatFieldByteOffset())))) & (1U << (rememberedBitByteShift()))
			? 'r'
			: '.'),
			((byteAt((void *)(oop + (formatFieldByteOffset())))) & (1U << (pinnedBitByteShift()))
			? 'p'
			: '.'),
			((byteAt((void *)(oop + (markBitsByteOffset())))) & (1U << (markedBitByteShift()))
			? 'm'
			: '.'),
			((byteAt((void *)(oop + (markBitsByteOffset())))) & (1U << (greyBitByteShift()))
			? 'g'
			: '.'));
	fprintf(GIV(transcript),
			" hash %" PRIxSQINT "\n",
			(long32At((void *)(oop + 4))) & (identityHashHalfWordMask()));
	if (/* isPureBitsFormat: */
		(fmt >= (sixtyFourBitIndexableFormat()))
	 && (fmt < (firstCompiledMethodFormat()))) {
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
				fprintf(GIV(transcript),
						" datasize %" PRIdSQINT " %s @ %p\n",
						longAt((void *)(oop + BaseHeaderSize)),
						((longAt((void *)(oop + BaseHeaderSize))) < 0
						? "indirect"
						: (longAt((void *)(oop + BaseHeaderSize))
								? "direct"
								: "pointer")),
						startOfAlienData(oop));
				return;
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
		classPointer = longAt((void *)((GIV(specialObjectsOop) + BaseHeaderSize) + ((((usqInt)(ClassByteString) << (shiftForWord()))))));

		/* begin superclassOf: */
		/* begin followObjField:ofObject: */
		objOop = longAt((void *)((classPointer + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
		assert(isNonImmediate(objOop));
		if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, classPointer, objOop);
		}
		aClass = objOop;

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
			if (oopClass == aClass) {
				printStringDataOfon(oop, GIV(transcript));
				return;
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
		printNonPointerDataOfon(oop, GIV(transcript));
		return;
	}

	/* This will answer false if splObj: ClassAlien is nilObject */

	/* begin numPointerSlotsOf: */
	fmtSqInt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
	if (fmtSqInt <= 5 /* lastPointerFormat */) {
		if ((fmtSqInt == (indexablePointersFormat()))
		 && (((longAt((void *)(oop))) & (classIndexMask())) == ClassMethodContextCompactIndex)) {
			/* begin fetchStackPointerOf: */
			sp = longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(StackPointerIndex) << (shiftForWord()))))));
			if (!((((sp) & 7) == 1))) {
				contextSize = 0;
				goto l2;
			}
			assert((ReceiverIndex + ((sp >> 3))) < (lengthOf(oop)));
			contextSize = (sp >> 3);
			/* end fetchStackPointerOf: */
l2:
			lastPointer = CtxtTempFrameStart + contextSize;
			goto l3;
		}

		/* contexts end at the stack pointer */

		/* begin numSlotsOf: */
		assert((classIndexOf(oop)) > (isForwardedObjectClassIndexPun()));
		lastPointer = (((numSlots = byteAt((void *)(oop + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots);
		goto l3;
	}
	if (fmtSqInt == (forwardedFormat())) {
		lastPointer = 1;
		goto l3;
	}
	if (fmtSqInt < (firstCompiledMethodFormat())) {
		lastPointer = 0;
		goto l3;
	}

	/* begin methodHeaderOf: */
	assert(isCompiledMethod(oop));
	header = longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));

	/* begin literalCountOfMethodHeader: */
	assert((((header) & 7) == 1));
	numLiterals = ((header >> 3)) & AlternateHeaderNumLiteralsMask;
	lastPointer = numLiterals + LiteralStart;
	/* end numPointerSlotsOf: */
l3:
	lastIndex = ((0x100 < lastPointer) ? 0x100 : lastPointer);
	if (lastIndex > 0) {
		for (i = 1; i <= lastIndex; i += 1) {
			fieldOop = longAt((void *)((oop + BaseHeaderSize) + ((((usqInt)((i - 1)) << (shiftForWord()))))));

			/* begin space */
			printChar(' ');
			fprintf(GIV(transcript),
					"%" PRIdSQINT "",
					((sqInt)(i - 1)));
			printChar(' ');
			printHex(fieldOop);

			/* begin space */
			printChar(' ');
			if ((i == 1)
			 && (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat()))) {
				/* begin printDecodeMethodHeaderOop: */
				/* begin printOopShort: */
				printOopShortInner(fieldOop);
				fflush(GIV(transcript));
				if (((fieldOop & AlternateHeaderHasPrimFlag) != 0)) {
					/* begin print: */
					fprintf(GIV(transcript),
							"%s",
							" hasPrim");
				}
				if (fieldOop & LargeContextBit) {
					/* begin print: */
					fprintf(GIV(transcript),
							"%s",
							" largeFrame");
				}
				if (((fieldOop >> 3)) < 0) {
					/* begin print: */
					fprintf(GIV(transcript),
							"%s",
							" altSet");
				}

				/* begin print: */
				fprintf(GIV(transcript),
						"%s",
						" nLits ");
				/* begin literalCountOfMethodHeader: */
				assert((((fieldOop) & 7) == 1));
				n = ((fieldOop >> 3)) & AlternateHeaderNumLiteralsMask;

				/* begin printNum: */
				fprintf(GIV(transcript),
						"%" PRIdSQINT "",
						((sqInt)n));
				fprintf(GIV(transcript),
						"%s",
						" nArgs ");
				fprintf(GIV(transcript),
						"%" PRIdSQINT "",
						((sqInt)((((usqInt)(fieldOop)) >> MethodHeaderArgCountShift) & 15)));
				fprintf(GIV(transcript),
						"%s",
						" nTemps ");
				fprintf(GIV(transcript),
						"%" PRIdSQINT "",
						((sqInt)((((usqInt)(fieldOop)) >> MethodHeaderTempCountShift) & 0x3F)));
			}
			else {
				printOopShortInner(fieldOop);
			}
			cr();
		}
	}
	if (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())) {
		lastPointer = (((literalCountOf(oop)) + LiteralStart) * BytesPerOop) + 1;

		/* begin lengthOf: */
		fmtSqInt = (byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask());
		numSlotsUsqInt = byteAt((void *)(oop + (numSlotsFieldByteOffset())));
		numSlots = (numSlotsUsqInt == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(oop - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlotsUsqInt);
		if (fmtSqInt <= (ephemeronFormat())) {
			lastIndex = numSlots;
			goto l4;
		}
		if (fmtSqInt >= (firstByteFormat())) {
			lastIndex = ((numSlots << (shiftForWord()))) - (fmtSqInt & 7);
			goto l4;
		}

		/* bytes, including CompiledMethod */
		if (fmtSqInt >= (firstShortFormat())) {
			lastIndex = ((numSlots << ((shiftForWord()) - 1))) - (fmtSqInt & 3);
			goto l4;
		}
		if (fmtSqInt >= (firstLongFormat())) {
			lastIndex = ((numSlots << ((shiftForWord()) - 2))) - (fmtSqInt & 1);
			goto l4;
		}
		if (fmtSqInt == (sixtyFourBitIndexableFormat())) {
			lastIndex = numSlots;
			goto l4;
		}

		/* fmt = self forwardedFormat */
		lastIndex = 0;
		/* end lengthOf: */
l4:
		if ((lastIndex - lastPointer) > 0x100) {
			lastIndex = lastPointer + 0x100;
		}
		column = 1;
		for (index = lastPointer; index <= lastIndex; index += 1) {
			if (column == 1) {
				fprintf(GIV(transcript),
						"%10p ",
						((void *)(((oop + BaseHeaderSize) + index) - 1)));
			}
			byte = ((int) (byteAt((void *)((oop + BaseHeaderSize) + (index - 1)))));
			fprintf(GIV(transcript),
					"%02x/%-+3d%c",
					byte,
					byte,
					(column == 8
					? '\n'
					: ' '));
			if (((column += 1)) > 8) {
				column = 1;
			}
		}
		if ((lengthOf(oop)) > lastIndex) {
			/* begin print: */
			fprintf(GIV(transcript),
					"%s",
					"...");
		}
		if (((column >= 2) && (column <= 7))) {
			cr();
		}
	}
	else {
		if (lastPointer > lastIndex) {
			/* begin print: */
			fprintf(GIV(transcript),
					"%s",
					"...");
			cr();
		}
	}
}