/* Extracted from interp.c:55219 (function isWidowedContextDuringGC). */

static NoDbgRegParms sqInt
isWidowedContextDuringGC(sqInt aOnceMarriedContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt anInteger;
    char *maybeFrame;
    sqInt maybeFrameCtxt;
    sqInt referent;
    sqInt senderOop;
    char *shouldBeFrameCallerField;
    StackPage *thePage;

	assert((isContext(aOnceMarriedContext))
	 && (isMarriedOrWidowedContext(aOnceMarriedContext)));

	/* begin frameOfMarriedContext: */
	senderOop = longAt((void *)((aOnceMarriedContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))));
	assert((((senderOop) & 7) == 1));
	maybeFrame = ((char *)(senderOop - (smallIntegerTag())));

	/* begin stackPageFor: */
	thePage = stackPageAtpages(pageIndexForstackMemorybytesPerPage(maybeFrame, GIV(stackMemory), GIV(bytesPerPage)), GIV(pages));
	if (!((isFree(thePage))
		 || (maybeFrame < ((thePage->headFP))))) {
		anInteger = longAt((void *)((aOnceMarriedContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))));

		/* begin withoutSmallIntegerTags: */
		assert((((anInteger) & 7) == 1));
		shouldBeFrameCallerField = ((char *)(anInteger - (smallIntegerTag())));
		if (((((char *)(longAt(maybeFrame + FoxSavedFP)))) == shouldBeFrameCallerField)
		 && ((byteAt((maybeFrame + FoxFrameFlags) + 2)) != 0)) {
			maybeFrameCtxt = longAt(maybeFrame + FoxThisContext);

			/* On Spur we need to follow the context to check for a match, but since the VM is
			   only speculating about maybeFrame being a frame, and only speculating about
			   maybeContext being a context, we need to be sure before we can safely follow. */
			if ((isFrameonPage(maybeFrame, thePage))
			 && ((!((longAt((void *)(maybeFrameCtxt))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
				/* begin followForwarded: */
				assert(isUnambiguouslyForwarder(maybeFrameCtxt));
				referent = longAt((void *)((maybeFrameCtxt + BaseHeaderSize) + (0U << (shiftForWord()))));
				while (/* isOopForwarded: */
					((!(referent & (tagMask()))))
				 && ((!((longAt((void *)(referent))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
					referent = longAt((void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
				}
				maybeFrameCtxt = referent;

				/* begin setFrameContext:to: */
				longAtput(maybeFrame + FoxThisContext,maybeFrameCtxt);
			}
			if (maybeFrameCtxt == aOnceMarriedContext) {
				return 0;
			}
		}
	}

	/* The frame pointer is within the bounds of a live page.
	   Now check if it matches a frame.
	   It is out of range or doesn't match the frame's context.
	   It is widowed. Time to wear black. */

	/* begin markContextAsDead: */
	assert(isContext(aOnceMarriedContext));
	assert((isNonImmediate(aOnceMarriedContext))
	 && (!(isForwarded(aOnceMarriedContext))));
	assert(validStorePointerUncheckedArgs(SenderIndex, aOnceMarriedContext, GIV(nilObj)));
	longAtput((void *)((aOnceMarriedContext + BaseHeaderSize) + ((((usqInt)(SenderIndex) << (shiftForWord()))))),GIV(nilObj));

	/* begin storePointerUnchecked:ofObject:withValue: */
	assert((isNonImmediate(aOnceMarriedContext))
	 && (!(isForwarded(aOnceMarriedContext))));
	assert(validStorePointerUncheckedArgs(InstructionPointerIndex, aOnceMarriedContext, GIV(nilObj)));
	longAtput((void *)((aOnceMarriedContext + BaseHeaderSize) + ((((usqInt)(InstructionPointerIndex) << (shiftForWord()))))),GIV(nilObj));
	return 1;
}