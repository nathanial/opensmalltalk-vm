/* Extracted from interp.c:50877 (function checkIsStillMarriedContextcurrentFP). */

/*	Another version of isWidowedContext: for debugging.
	This will not bereave a widowed context. */

	/* StackInterpreter>>#checkIsStillMarriedContext:currentFP: */

static NoDbgRegParms sqInt
checkIsStillMarriedContextcurrentFP(sqInt aContext, char *currentFP)
{
    char *limitFP;
    char *maybeFP;
    sqInt maybeFrameCtxt;
    sqInt referent;
    sqInt senderOop;
    StackPage *thePage;

	if (!((/* isContext: */
			((!(aContext & (tagMask()))))
		 && (((longAt((void *)(aContext))) & (classIndexMask())) == ClassMethodContextCompactIndex))
		 && (((((longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord())))))))) & 7) == 1)))) {
		return 0;
	}

	/* begin frameOfMarriedContext: */
	senderOop = longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	assert((((senderOop) & 7) == 1));
	maybeFP = ((char *)(senderOop - (smallIntegerTag())));

	/* begin stackPageFor: */
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(maybeFP, stackMemory, bytesPerPage), pages);
	limitFP = ((thePage == stackPage)
		 && (currentFP)
				? currentFP
				: (thePage->headFP));
	if (!((maybeFP >= limitFP)
		 && (((!((((sqInt)(((char *)(longAt(maybeFP + FoxSavedFP)))))) & (tagMask()))))
		 && ((((/* begin withSmallIntegerTags: */
			assert(((oopForPointer(((char *)(longAt(maybeFP + FoxSavedFP))))) & (BytesPerWord - 1)) == 0),
		(oopForPointer(((char *)(longAt(maybeFP + FoxSavedFP))))) + (smallIntegerTag()))) == (longAt((void *)((aContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))))))
		 && ((byteAt((maybeFP + FoxFrameFlags) + 2)) != 0))))) {
		return 0;
	}
	maybeFrameCtxt = longAt(maybeFP + FoxThisContext);

	/* On Spur we need to follow the context to check for a match, but since the VM is
	   only speculating about maybeFrame being a frame, and only speculating about
	   maybeContext being a context, we need to be sure before we can safely follow. */
	if ((!(isFree(thePage)))
	 && ((isFrameonPage(maybeFP, thePage))
	 && ((!((longAt((void *)(maybeFrameCtxt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))) {
		/* begin followForwarded: */
		assert(isUnambiguouslyForwarder(maybeFrameCtxt));
		referent = longAt((void *)((maybeFrameCtxt + BaseHeaderSize) + (0U << (shiftForWord()))));
		while (/* isOopForwarded: */
			((!(referent & (tagMask()))))
		 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
			referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
		}
		maybeFrameCtxt = referent;
	}
	return maybeFrameCtxt == aContext;
}