/* Extracted from interp.c:53740 (function
 * followForwardingPointersOfReceiversInStackZone). */

/*	Spur's become: is lazy, turning the becommed object into a forwarding
        object to the other.
        The read-barrier is minimised by arranging that forwarding pointers will
        fail a method cache
        probe, since notionally objects' internals are accessed only via sending
        messages to them,
        the exception is primitives that access the internals of the
   non-receiver argument(s). To avoid a read barrier on bytecode, literal and
   inst var fetch and non-local return, we scan the receivers (including the
   stacked receiver for non-local return) and method references in the stack
   zone and follow any forwarded ones. This is of course way cheaper than
        scanning all of memory as in the old become. */

/* StackInterpreter>>#followForwardingPointersOfReceiversInStackZone */

static void followForwardingPointersOfReceiversInStackZone(void) {
  sqInt aValue;
  char *callerFP;
  sqInt delta;
  sqInt i;
  sqInt newOop;
  sqInt offset;
  sqInt oop;
  char *theFP;
  usqInt theIPPtr;
  StackPage *thePage;

  callerFP = ((char *)0);
  if (!stackPage) {
    assert((!(mostRecentlyUsedPage)) || (isFree(mostRecentlyUsedPage)));
    return;
  }

  /* the system must be snapshotting; nothing to do... */

  externalWriteBackHeadFramePointers();
  for (i = 0; i < numStackPages; i += 1) {
    /* begin stackPageAt: */
    thePage = stackPageAtpages(i, pages);
    if ((thePage->baseFP)) {
      assert(ifCurrentStackPageHasValidHeadPointers(thePage));
      theFP = (thePage->headFP);

      /* Skip the instruction pointer on top of stack of inactive pages. */
      theIPPtr = (!(thePage == stackPage) ? ((usqInt)((thePage->headSP))) : 0);
      while (1) {
        assert(addressIsInPage(thePage, theFP));
        assert((!theIPPtr) || (addressIsInPage(thePage, ((void *)theIPPtr))));
        oop = longAt(theFP + FoxReceiver);
        if (isOopForwarded(oop)) {
          longAtput(theFP + FoxReceiver, followForwarded(oop));
        }
        if (((byteAt((theFP + FoxFrameFlags) + 2)) != 0) &&
            ((!((longAt((void *)(longAt(theFP + FoxThisContext)))) &
                ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
          longAtput(theFP + FoxThisContext,
                    followForwarded(longAt(theFP + FoxThisContext)));
        }
        oop = longAt(theFP + FoxMethod);
        if ((!((longAt((void *)(oop))) &
               ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
          newOop = followForwarded(oop);
          if (theIPPtr) {
            assert((longAt((void *)(theIPPtr))) > (frameMethod(theFP)));
            delta = newOop - oop;
            longAtput((void *)(theIPPtr), (longAt((void *)(theIPPtr))) + delta);
          }
          longAtput(theFP + FoxMethod, (oop = newOop));
        }
        offset = (FoxCallerSavedIP + BytesPerWord) +
                 ((((usqInt)((byteAt((theFP + FoxFrameFlags) + 1)))
                    << (shiftForWord()))));
        oop = longAt(theFP + offset);
        if (isOopForwarded(oop)) {
          longAtput(theFP + offset, followForwarded(oop));
        }
        if (!(((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0))
          break;
        theIPPtr = ((usqInt)(theFP + FoxCallerSavedIP));
        theFP = callerFP;
      }

      /* And finally follow the caller context. */
      assert(theFP == ((thePage->baseFP)));

      /* begin frameCallerContext: */
      assert(isBaseFrame(theFP));
      oop = longAt(theFP + FoxCallerContext);
      if ((!((longAt((void *)(oop))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        aValue = followForwarded(oop);

        /* begin frameCallerContext:put: */
        assert(isBaseFrame(theFP));
        assert((aValue == (nilObject())) || (isContext(aValue)));
        longAtput(theFP + FoxCallerContext, aValue);
      }
    }
  }
}
