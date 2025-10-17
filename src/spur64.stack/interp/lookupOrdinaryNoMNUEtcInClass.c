/* Extracted from interp.c:56281 (function lookupOrdinaryNoMNUEtcInClass). */

/*	Lookup messageSelector in class. Answer 0 on success. Answer the splObj:
	index for the error selector to use on failure rather than performing MNU
	processing etc. */

	/* StackInterpreter>>#lookupOrdinaryNoMNUEtcInClass: */

static NoDbgRegParms sqInt
lookupOrdinaryNoMNUEtcInClass(sqInt class)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt currentClass;
    sqInt dictionary;
    sqInt found;
    sqInt index;
    usqInt length;
    sqInt mask;
    sqInt methodArray;
    sqInt nextSelector;
    usqInt numSlots;
    sqInt objOop;
    sqInt objOopSqInt;
    sqInt wrapAround;

	/* begin lookupBreakFor: */
	if ((breakSelectorLength <= 0)
	 && (/* shouldBreakForLookupIn:given: */
		(breakLookupClassTag)
	 && ((class == breakLookupClassTag)
	 || (((long32At((void *)(class + 4))) & (identityHashHalfWordMask())) == breakLookupClassTag)))) {
		warning("lookup class send break (heartbeat suppressed)");
	}
	currentClass = class;
	while (currentClass != GIV(nilObj)) {
		/* begin followObjField:ofObject: */
		dictionary = longAt((void *)((currentClass + BaseHeaderSize) + ((((usqInt)(MethodDictionaryIndex) << (shiftForWord()))))));
		assert(isNonImmediate(dictionary));
		if ((!((longAt((void *)(dictionary))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			dictionary = fixFollowedFieldofObjectwithInitialValue(MethodDictionaryIndex, currentClass, dictionary);
		}
		if (dictionary == GIV(nilObj)) {
			/* begin superclassOf: */
			/* begin followObjField:ofObject: */
			objOop = longAt((void *)((currentClass + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
			assert(isNonImmediate(objOop));
			if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
				objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, currentClass, objOop);
			}
			GIV(lkupClass) = objOop;
			return SelectorCannotInterpret;
		}

		/* begin lookupMethodInDictionary: */
		/* begin numSlotsOf: */
		assert((classIndexOf(dictionary)) > (isForwardedObjectClassIndexPun()));
		length = (((numSlots = byteAt((void *)(dictionary + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(dictionary - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots);
		mask = (length - SelectorStart) - 1;

		/* Use linear search on small dictionaries; its cheaper.
		   Also the limit can be set to force linear search of all dictionaries, which supports the
		   booting of images that need rehashing (e.g. because a tracer has generated an image
		   with different hashes but hasn't rehashed it yet.) */
		if (mask <= GIV(methodDictLinearSearchLimit)) {
			index = 0;
			while (index <= mask) {
				nextSelector = longAt((void *)((dictionary + BaseHeaderSize) + ((((usqInt)((index + SelectorStart)) << (shiftForWord()))))));
				if (/* isOopForwarded: */
					((!(nextSelector & (tagMask()))))
				 && ((!((longAt((void *)(nextSelector))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					nextSelector = fixFollowedFieldofObjectwithInitialValue(index + SelectorStart, dictionary, nextSelector);
				}
				if (nextSelector == GIV(messageSelector)) {
					/* begin followObjField:ofObject: */
					methodArray = longAt((void *)((dictionary + BaseHeaderSize) + ((((usqInt)(MethodArrayIndex) << (shiftForWord()))))));
					assert(isNonImmediate(methodArray));
					if ((!((longAt((void *)(methodArray))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
						methodArray = fixFollowedFieldofObjectwithInitialValue(MethodArrayIndex, dictionary, methodArray);
					}

					/* begin followField:ofObject: */
					objOopSqInt = longAt((void *)((methodArray + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
					if (/* isOopForwarded: */
						((!(objOopSqInt & (tagMask()))))
					 && ((!((longAt((void *)(objOopSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
						objOopSqInt = fixFollowedFieldofObjectwithInitialValue(index, methodArray, objOopSqInt);
					}
					GIV(newMethod) = objOopSqInt;
					found = 1;
					goto l1;
				}
				index += 1;
			}
			found = 0;
			goto l1;
		}
		index = SelectorStart + (mask & ((((GIV(messageSelector) & (tagMask())) != 0)
		? (GIV(messageSelector) >> 3)
		: (long32At((void *)(GIV(messageSelector) + 4))) & (identityHashHalfWordMask()))));

		/* It is assumed that there are some nils in this dictionary, and search will
		   stop when one is encountered. However, if there are no nils, then wrapAround
		   will be detected the second time the loop gets to the end of the table. */
		wrapAround = 0;
		while (1) {
			nextSelector = longAt((void *)((dictionary + BaseHeaderSize) + ((((usqInt)(index) << (shiftForWord()))))));
			if (nextSelector == GIV(nilObj)) {
				found = 0;
				goto l1;
			}
			if (/* isOopForwarded: */
				((!(nextSelector & (tagMask()))))
			 && ((!((longAt((void *)(nextSelector))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				nextSelector = fixFollowedFieldofObjectwithInitialValue(index + SelectorStart, dictionary, nextSelector);
			}
			if (nextSelector == GIV(messageSelector)) {
				/* begin followObjField:ofObject: */
				methodArray = longAt((void *)((dictionary + BaseHeaderSize) + ((((usqInt)(MethodArrayIndex) << (shiftForWord()))))));
				assert(isNonImmediate(methodArray));
				if ((!((longAt((void *)(methodArray))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
					methodArray = fixFollowedFieldofObjectwithInitialValue(MethodArrayIndex, dictionary, methodArray);
				}

				/* begin followField:ofObject: */
				objOopSqInt = longAt((void *)((methodArray + BaseHeaderSize) + ((((usqInt)((index - SelectorStart)) << (shiftForWord()))))));
				if (/* isOopForwarded: */
					((!(objOopSqInt & (tagMask()))))
				 && ((!((longAt((void *)(objOopSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					objOopSqInt = fixFollowedFieldofObjectwithInitialValue(index - SelectorStart, methodArray, objOopSqInt);
				}
				GIV(newMethod) = objOopSqInt;
				found = 1;
				goto l1;
			}
			index += 1;
			if (index == length) {
				if (wrapAround) {
					found = 0;
					goto l1;
				}
				wrapAround = 1;
				index = SelectorStart;
			}
		}
		found = 0;
		/* end lookupMethodInDictionary: */
l1:
		if (found) {
			addNewMethodToCache(class);
			return 0;
		}

		/* begin superclassOf: */
		/* begin followObjField:ofObject: */
		objOopSqInt = longAt((void *)((currentClass + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
		assert(isNonImmediate(objOopSqInt));
		if ((!((longAt((void *)(objOopSqInt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			objOopSqInt = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, currentClass, objOopSqInt);
		}
		currentClass = objOopSqInt;
	}
	GIV(lkupClass) = class;
	return SelectorDoesNotUnderstand;
}