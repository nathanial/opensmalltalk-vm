/* Extracted from interp.c:53299 (function
 * findMethodWithPrimitiveFromContextUpToContext). */

/*	See findUnwindThroughContext:. Alas this is mutually recursive with
        findMethodWithPrimitive:FromFP:SP:ThroughContext: instead of iterative.
        We're doing the simplest thing that could possibly work. Niceties can
        wait.  */
/*	Being mutually-recursive with
   findMethodWithPrimitive:FromFP:UpToContext: gives the author's type inference
   algorithm headaches. Wimp out by declaring the return type.
 */

/* StackInterpreter>>#findMethodWithPrimitive:FromContext:UpToContext: */

static sqInt findMethodWithPrimitiveFromContextUpToContext(sqInt primitive,
                                                           sqInt senderContext,
                                                           sqInt homeContext) {
  sqInt objOop;
  sqInt senderContextSqInt;
  sqInt senderOop;
  char *startFP;
  sqInt theContext;
  char *theFP;
  char *theFPAbove;
  sqInt theMethod;
  sqInt theMethodSqInt;
  char *theSP;

  assert((senderContext == (nilObject())) || (isContext(senderContext)));
  assert((homeContext == (nilObject())) || (isContext(homeContext)));
  theContext = senderContext;
  while (1) {
    if (theContext == nilObj) {
      return theContext;
    }
    if (((((fetchPointerofObject(SenderIndex, theContext))) &
          7) == 1))
      break;
    if (theContext == homeContext) {
      return 0;
    }
    if (!((primitive == 0) ||
          ((fetchPointerofObject(ClosureIndex, theContext)) != nilObj))) {
      /* begin followObjField:ofObject: */
      theMethod =
          fetchPointerofObject(MethodIndex, theContext);
      assert(isNonImmediate(theMethod));
      if ((!((longAt((void *)(theMethod))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
        theMethod = fixFollowedFieldofObjectwithInitialValue(
            MethodIndex, theContext, theMethod);
      }
      if ((primitiveIndexOf(theMethod)) == primitive) {
        return theContext;
      }
    }

    /* begin followObjField:ofObject: */
    objOop = fetchPointerofObject(SenderIndex, theContext);
    assert(isNonImmediate(objOop));
    if ((!((longAt((void *)(objOop))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      objOop = fixFollowedFieldofObjectwithInitialValue(SenderIndex, theContext,
                                                        objOop);
    }
    theContext = objOop;
  }
  if (isWidowedContext(theContext)) {
    return nilObj;
  }

  /* begin frameOfMarriedContext: */
  senderOop = fetchPointerofObject(SenderIndex, theContext);
  assert((((senderOop) & 7) == 1));
  startFP = ((char *)(senderOop - (smallIntegerTag())));

  /* begin findMethodWithPrimitive:FromFP:UpToContext: */
  theFP = startFP;
  theFPAbove = startFP;
  do {
    if (((byteAt((theFP + FoxFrameFlags) + 2)) != 0) &&
        (homeContext == (longAt(theFP + FoxThisContext)))) {
      return 0;
    }
    if (!((primitive == 0) || ((byteAt((theFP + FoxFrameFlags) + 3)) != 0))) {
      theMethodSqInt = longAt(theFP + FoxMethod);
      if ((primitiveIndexOf(theMethodSqInt)) == primitive) {
        if (theFP == theFPAbove) {
          theSP = findSPOfon(theFP, stackPageFor(theFP));
        } else {
          /* begin frameCallerStackPointer: */
          assert(!(isBaseFrame(theFPAbove)));
          theSP = (theFPAbove +
                   ((FoxCallerSavedIP + BytesPerWord) +
                    ((((usqInt)((byteAt((theFPAbove + FoxFrameFlags) + 1)))
                       << (shiftForWord())))))) +
                  BytesPerWord;
        }

        /* begin ensureFrameIsMarried:SP: */
        if (byteAt((theFP + FoxFrameFlags) + 2)) {
          assert(isContext(frameContext(theFP)));
          return longAt(theFP + FoxThisContext);
        }
        return marryFrameSP(theFP, theSP);
      }
    }
    theFPAbove = theFP;
    theFP = ((char *)(longAt(theFP + FoxSavedFP)));
  } while (theFP != 0);

  /* begin frameCallerContext: */
  assert(isBaseFrame(theFPAbove));
  senderContextSqInt = longAt(theFPAbove + FoxCallerContext);
  if (!(/* isContext: */
        ((!(senderContextSqInt & (tagMask())))) &&
        (((longAt((void *)(senderContextSqInt))) & (classIndexMask())) ==
         ClassMethodContextCompactIndex))) {
    return nilObj;
  }
  return findMethodWithPrimitiveFromContextUpToContext(
      primitive, senderContextSqInt, homeContext);
}