/* Extracted from interp.c:54633 (function
 * handleStackOverflowOrEventAllowContextSwitch). */

/*	The stackPointer is below the stackLimit. This is either because of a
        stack overflow or the setting of stackLimit to indicate a possible
        interrupt. Check for stackOverflow and interrupts anddeal with each
        appropriately. Answer if a context switch occurred. */

/* StackInterpreter>>#handleStackOverflowOrEventAllowContextSwitch: */

static sqInt
handleStackOverflowOrEventAllowContextSwitch(sqInt mayContextSwitch) {
  /* begin checkForStackOverflow */
  externalWriteBackHeadFramePointers();
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