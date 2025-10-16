/* Extracted from interp.c:53459 (function findSelectorOfMethod). */

sqInt
findSelectorOfMethod(sqInt aMethodOop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classDict;
    usqInt classDictSize;
    sqInt classObj;
    sqInt homeMethod;
    sqInt i;
    sqInt methodArray;
    usqInt numSlots;
    sqInt objOop;
    sqInt referent;
    sqInt selector;

	if (!(addressCouldBeObj(aMethodOop))) {
		return GIV(nilObj);
	}
	if ((!((longAt((void *)(aMethodOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		return findSelectorOfMethod(followForwarded(aMethodOop));
	}
	if (!(/* isOopCompiledMethod: */
			((!(aMethodOop & (tagMask()))))
		 && (((byteAt((void *)(aMethodOop + (formatFieldByteOffset())))) & (formatMask())) >= (firstCompiledMethodFormat())))) {
		return GIV(nilObj);
	}
	homeMethod = homeMethodOf(aMethodOop);
	if ((selector = maybeSelectorOfMethod(homeMethod))) {
		return selector;
	}
	classObj = safeMethodClassOf(homeMethod);
	if (addressCouldBeClassObj(classObj)) {
		/* begin noFixupFollowField:ofObject: */
		classDict = longAt((void *)((classObj + BaseHeaderSize) + ((((usqInt)(MethodDictionaryIndex) << (shiftForWord()))))));
		if (/* isOopForwarded: */
			((!(classDict & (tagMask()))))
		 && ((!((longAt((void *)(classDict))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			/* begin followForwarded: */
			assert(isUnambiguouslyForwarder(classDict));
			referent = longAt((void *)((classDict + BaseHeaderSize) + (0U << (shiftForWord()))));
			while (/* isOopForwarded: */
				((!(referent & (tagMask()))))
			 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
			}
			classDict = referent;
		}

		/* begin numSlotsOf: */
		assert((classIndexOf(classDict)) > (isForwardedObjectClassIndexPun()));
		classDictSize = (((numSlots = byteAt((void *)(classDict + (numSlotsFieldByteOffset()))))) == (numSlotsMask())
					? ((((usqInt)(((sqInt)((usqInt)((longAt((void *)(classDict - BaseHeaderSize)))) << 8)))))) >> 8
					: numSlots);
		if (classDictSize > MethodArrayIndex) {
			/* begin noFixupFollowField:ofObject: */
			methodArray = longAt((void *)((classDict + BaseHeaderSize) + ((((usqInt)(MethodArrayIndex) << (shiftForWord()))))));
			if (/* isOopForwarded: */
				((!(methodArray & (tagMask()))))
			 && ((!((longAt((void *)(methodArray))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(methodArray));
				referent = longAt((void *)((methodArray + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				methodArray = referent;
			}
			i = 0;
			while (i < (classDictSize - SelectorStart)) {
				if (homeMethod == (noFixupFollowFieldofObject(i, methodArray))) {
					/* begin noFixupFollowField:ofObject: */
					objOop = longAt((void *)((classDict + BaseHeaderSize) + ((((usqInt)((i + SelectorStart)) << (shiftForWord()))))));
					if (/* isOopForwarded: */
						((!(objOop & (tagMask()))))
					 && ((!((longAt((void *)(objOop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
						/* begin followForwarded: */
						assert(isUnambiguouslyForwarder(objOop));
						referent = longAt((void *)((objOop + BaseHeaderSize) + (0U << (shiftForWord()))));
						while (/* isOopForwarded: */
							((!(referent & (tagMask()))))
						 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
							referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
						}
						objOop = referent;
					}
					return objOop;
				}
				i += 1;
			}
		}
	}
	return GIV(nilObj);
}