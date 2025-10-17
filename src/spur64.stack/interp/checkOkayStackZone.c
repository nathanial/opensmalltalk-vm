/* Extracted from interp.c:51150 (function checkOkayStackZone). */

/*	Check that all objects in the stack zone are okay */

/* StackInterpreter>>#checkOkayStackZone: */

static sqInt checkOkayStackZone(sqInt writeBack) {
  sqInt i;
  sqInt ok;
  StackPage *thePage;

  if (writeBack) {
    /* begin externalWriteBackHeadFramePointers */
    assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
    assert(stackPage == (mostRecentlyUsedPage));
    assert(!((isFree(stackPage))));

    /* begin setHeadFP:andSP:inPage: */
    assert(stackPointer < framePointer);
    assert((stackPointer < ((stackPage->baseAddress))) &&
           (stackPointer > (((stackPage->realStackLimit)) -
                            (LargeContextSlots * BytesPerOop))));
    assert((framePointer < ((stackPage->baseAddress))) &&
           (framePointer > (((stackPage->realStackLimit)) -
                            ((LargeContextSlots * BytesPerOop) / 2))));
    (stackPage->headFP = framePointer);
    (stackPage->headSP = stackPointer);
    assert(pageListIsWellFormed());
  }
  ok = 1;
  for (i = 0; i < numStackPages; i += 1) {
    /* begin stackPageAt: */
    thePage = stackPageAtpages(i, pages);
    if (!(isFree(thePage))) {
      ok = ok && (checkOkayStackPage(thePage));
    }
  }
  return ok;
}