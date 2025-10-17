/* Extracted from interp.c:53865 (function forceInterruptCheck). */

/*	Force an interrupt check ASAP.
        Must set the stack page's limit before stackLimit to avoid
        a race condition if this is called from an interrupt handler. */

/* StackInterpreter>>#forceInterruptCheck */

sqInt forceInterruptCheck(void) {
  void (*iccFunc)();
  StackPage *thePage;

  /* Do _not_ set stackLimit until the stack system has been initialized.
     stackLimit is the initialization flag for the stack system. */
  if (!stackLimit) {
    return null;
  }
  thePage = stackPage;
  if ((thePage) && (thePage != 0)) {
    (thePage->stackLimit = ((char *)(((usqInt)-1))));
  }
  stackLimit = ((char *)(((usqInt)-1)));
  sqLowLevelMFence();

  /* There is a race condition if we test the function and then dereference
     it a second time to call it.  This is called from interrupt code but at the
     same time other code could be clearing the interruptCheckChain via
     setInterruptCheckChain:. */
  if (((iccFunc = interruptCheckChain))) {
    iccFunc();
  }
  statForceInterruptCheck += 1;
  return 0;
}