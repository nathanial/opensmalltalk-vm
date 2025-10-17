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

	if (!(lookupInMethodCacheSelclassTag(messageSelector, classTagArg))) {
		classTag = classTagArg;
		if ((/* isOopForwarded: */
			((!(messageSelector & (tagMask()))))
		 && ((!((longAt((void *)(messageSelector))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))
		 || (classTag == (isForwardedObjectClassIndexPun()))) {
			if (/* isOopForwarded: */
				((!(messageSelector & (tagMask()))))
			 && ((!((longAt((void *)(messageSelector))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				messageSelector = handleForwardedSelectorFaultFor(messageSelector);
			}
			if (classTag == (isForwardedObjectClassIndexPun())) {
				classTag = handleForwardedSendFaultForTag(classTag);
			}
			if (lookupInMethodCacheSelclassTag(messageSelector, classTag)) {
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
		classTablePage = longAt((void *)((hiddenRootsObj + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
		if (classTablePage == nilObj) {
			lkupClass = null;
			goto l1;
		}
		fieldIndex = classTag & ((1U << (classTableMajorIndexShift())) - 1);
		lkupClass = longAt((void *)((classTablePage + BaseHeaderSize) + ((((usqInt)(fieldIndex) << (shiftForWord()))))));
		/* end classForClassTag: */
l1:
		lookupMethodInClass(lkupClass);
		addNewMethodToCache(lkupClass);
	}
	return 0;
}