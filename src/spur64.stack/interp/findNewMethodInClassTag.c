/* Extracted from interp.c:53408 (function findNewMethodInClassTag). */

/*	Find the compiled method to be run when the current messageSelector is
	looked up in the
	class/classIndex classTagArg, setting the values of newMethod and
	primitiveFunctionPointer. 
 */

	/* StackInterpreter>>#findNewMethodInClassTag: */

static NoDbgRegParms sqInt
findNewMethodInClassTag(sqInt classTagArg)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt classTablePage;
    sqInt classTag;
    sqInt fieldIndex;

	if (!(lookupInMethodCacheSelclassTag(GIV(messageSelector), classTagArg))) {
		classTag = classTagArg;
		if ((/* isOopForwarded: */
			((!(GIV(messageSelector) & (tagMask()))))
		 && ((!((longAt((void *)(GIV(messageSelector)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))
		 || (classTag == (isForwardedObjectClassIndexPun()))) {
			if (/* isOopForwarded: */
				((!(GIV(messageSelector) & (tagMask()))))
			 && ((!((longAt((void *)(GIV(messageSelector)))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				GIV(messageSelector) = handleForwardedSelectorFaultFor(GIV(messageSelector));
			}
			if (classTag == (isForwardedObjectClassIndexPun())) {
				classTag = handleForwardedSendFaultForTag(classTag);
			}
			if (lookupInMethodCacheSelclassTag(GIV(messageSelector), classTag)) {
				return null;
			}
		}

		/* begin classForClassTag: */
		/* begin classAtIndex: */
		assert((classTag >= 0)
		 && ((classTag <= (tagMask()))
		 || ((classTag >= (arrayClassIndexPun()))
		 && (classTag <= (classIndexMask())))));
		fieldIndex = ((usqInt)(classTag)) >> (classTableMajorIndexShift());

		/* begin fetchPointer:ofObject: */
		classTablePage = longAt((void *)((GIV(hiddenRootsObj) + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
		if (classTablePage == GIV(nilObj)) {
			GIV(lkupClass) = null;
			goto l1;
		}
		fieldIndex = classTag & ((1U << (classTableMajorIndexShift())) - 1);
		GIV(lkupClass) = longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
		/* end classForClassTag: */
l1:
		lookupMethodInClass(GIV(lkupClass));
		addNewMethodToCache(GIV(lkupClass));
	}
	return 0;
}