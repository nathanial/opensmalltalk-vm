/* Extracted from interp.c:54633 (function
 * handleStackOverflowOrEventAllowContextSwitch). */

/*	The stackPointer is below the stackLimit. This is either because of a
        stack overflow or the setting of stackLimit to indicate a possible
        interrupt. Check for stackOverflow and interrupts anddeal with each
        appropriately. Answer if a context switch occurred. */

/* StackInterpreter>>#handleStackOverflowOrEventAllowContextSwitch: */

static NoDbgRegParms sqInt
handleStackOverflowOrEventAllowContextSwitch(sqInt mayContextSwitch) {
  /* begin checkForStackOverflow */
  /* begin externalWriteBackHeadFramePointers */
  assert((framePointer - stackPointer) < (LargeContextSlots * BytesPerOop));
  assert(stackPage == (mostRecentlyUsedPage));
  assert(!((isFree(stackPage))));

  /* begin setHeadFP:andSP:inPage: */
  assert(stackPointer < framePointer);
  assert((stackPointer < ((stackPage->baseAddress))) &&
         (stackPointer >
          (((stackPage->realStackLimit)) - (LargeContextSlots * BytesPerOop))));
  assert((framePointer < ((stackPage->baseAddress))) &&
         (framePointer > (((stackPage->realStackLimit)) -
                          ((LargeContextSlots * BytesPerOop) / 2))));
  (stackPage->headFP = framePointer);
  (stackPage->headSP = stackPointer);
  assert(pageListIsWellFormed());
  if (stackPointer < ((stackPage->realStackLimit))) {
    handleStackOverflow();
  }

  /* If the stackLimit differs from the realStackLimit then the stackLimit
     has been set to indicate an event or interrupt that needs servicing. */
  if (stackLimit == ((stackPage->realStackLimit))) {
    return 0;
  }
  return checkForEventsMayContextSwitch(mayContextSwitch);
}