/* Extracted from interp.c:57194 (function markAndTraceStackPage). */

/* StackInterpreter>>#markAndTraceStackPage: */

static NoDbgRegParms void markAndTraceStackPage(StackPage *thePage) {
  char *callerFP;
  char *frameRcvrOffset;
  sqInt oop;
  sqInt referent;
  char *theFP;
  char *theSP;

  callerFP = ((char *)0);
  assert(!(isFree(thePage)));
  assert(ifCurrentStackPageHasValidHeadPointers(thePage));
  assert(((thePage->trace)) != StackPageTraced);
  (thePage->trace = StackPageTraced);
  theSP = (thePage->headSP);
  theFP = (thePage->headFP);

  /* Skip the instruction pointer on top of stack of inactive pages. */
  if (!(thePage == stackPage)) {
    theSP += BytesPerWord;
  }
  while (1) {
    frameRcvrOffset = theFP + FoxReceiver;
    while (theSP <= frameRcvrOffset) {
      oop = longAt(theSP);
      if (/* isOopForwarded: */
          ((!(oop & (tagMask())))) &&
          ((!((longAt((void *)(oop))) &
              ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        /* begin followForwarded: */
        assert(isUnambiguouslyForwarder(oop));
        referent =
            longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
        while (/* isOopForwarded: */
               ((!(referent & (tagMask())))) &&
               ((!((longAt((void *)(referent))) &
                   ((classIndexMask()) -
                    (isForwardedObjectClassIndexPun())))))) {
          referent = longAt(
              (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
        }
        oop = referent;
        longAtput(theSP, oop);
      }
      if (!(((oop & (tagMask())) != 0))) {
        markAndTrace(oop);
      }
      theSP += BytesPerWord;
    }
    if (byteAt((theFP + FoxFrameFlags) + 2)) {
      assert(isContext(frameContext(theFP)));
      markAndTrace(frameContext(theFP));
    }
    markAndTrace(iframeMethod(theFP));
    if (!(((callerFP = ((char *)(longAt(theFP + FoxSavedFP))))) != 0))
      break;
    theSP = (theFP + FoxCallerSavedIP) + BytesPerWord;
    theFP = callerFP;
  }

  /* caller ip is frameCallerContext in a base frame */
  theSP = theFP + FoxCallerSavedIP;
  while (theSP <= ((thePage->baseAddress))) {
    oop = longAt(theSP);
    if (/* isOopForwarded: */
        ((!(oop & (tagMask())))) &&
        ((!((longAt((void *)(oop))) &
            ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
      /* begin followForwarded: */
      assert(isUnambiguouslyForwarder(oop));
      referent =
          longAt((void *)((oop + BaseHeaderSize) + (0U << (shiftForWord()))));
      while (/* isOopForwarded: */
             ((!(referent & (tagMask())))) &&
             ((!((longAt((void *)(referent))) &
                 ((classIndexMask()) - (isForwardedObjectClassIndexPun())))))) {
        referent = longAt(
            (void *)((referent + BaseHeaderSize) + (0U << (shiftForWord()))));
      }
      oop = referent;
      longAtput(theSP, oop);
    }
    if (!(((oop & (tagMask())) != 0))) {
      markAndTrace(oop);
    }
    theSP += BytesPerWord;
  }
}