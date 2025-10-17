/* Extracted from interp.c:56444 (function lookupSelectorinClass). */

/*	Lookup selector in class. Answer the method or nil. This is a debugging
	routine. It does /not/ side-effect lookupClass or newMethod. */

	/* StackInterpreter>>#lookupSelector:inClass: */

sqInt
lookupSelectorinClass(sqInt selector, sqInt class)
{
    sqInt currentClass;
    sqInt dictionary;
    usqInt index;
    usqInt length;
    sqInt mask;
    sqInt meth;
    sqInt methodArray;
    sqInt nextSelector;
    usqInt numSlots;
    sqInt objOop;
    sqInt objOopSqInt;
    sqInt wrapAround;

	currentClass = class;
	while (currentClass != nilObj) {
		/* begin followObjField:ofObject: */
		dictionary = longAt((void *)((currentClass + BaseHeaderSize) + ((((usqInt)(MethodDictionaryIndex) << (shiftForWord()))))));
		assert(isNonImmediate(dictionary));
		if ((!((longAt((void *)(dictionary))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			dictionary = fixFollowedFieldofObjectwithInitialValue(MethodDictionaryIndex, currentClass, dictionary);
		}
		if (dictionary == nilObj) {
			return null;
		}

		/* begin lookupMethodFor:InDictionary: */
		/* begin numSlotsOf: */
		assert((classIndexOf(dictionary)) > (isForwardedObjectClassIndexPun()));
		length = (((numSlots = byteAt((void *)(dictionary + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(dictionary - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots);
		mask = (length - SelectorStart) - 1;
		index = SelectorStart + (mask & ((((selector & (tagMask())) != 0)
		? (selector >> 3)
		: (long32At((void *)(selector + 4))) & (identityHashHalfWordMask()))));

		/* It is assumed that there are some nils in this dictionary, and search will
		   stop when one is encountered. However, if there are no nils, then wrapAround
		   will be detected the second time the loop gets to the end of the table. */
		wrapAround = 0;
		while (1) {
			nextSelector = longAt((void *)((dictionary + BaseHeaderSize) + ((index << (shiftForWord())))));
			if (nextSelector == nilObj) {
				meth = null;
				goto l1;
			}
			if (/* isOopForwarded: */
				((!(nextSelector & (tagMask()))))
			 && ((!((longAt((void *)(nextSelector))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				nextSelector = fixFollowedFieldofObjectwithInitialValue(index + SelectorStart, dictionary, nextSelector);
			}
			if (nextSelector == selector) {
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
				meth = objOopSqInt;
				goto l1;
			}
			index += 1;
			if (index == length) {
				if (wrapAround) {
					meth = null;
					goto l1;
				}
				wrapAround = 1;
				index = SelectorStart;
			}
		}
		meth = null;
		/* end lookupMethodFor:InDictionary: */
l1:
		if (meth) {
			return meth;
		}

		/* begin superclassOf: */
		/* begin followObjField:ofObject: */
		objOop = longAt((void *)((currentClass + BaseHeaderSize) + ((((usqInt)(SuperclassIndex) << (shiftForWord()))))));
		assert(isNonImmediate(objOop));
		if ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
			objOop = fixFollowedFieldofObjectwithInitialValue(SuperclassIndex, currentClass, objOop);
		}
		currentClass = objOop;
	}
	return null;
}