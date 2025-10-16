/* Extracted from interp.c:53865 (function forceInterruptCheck). */

sqInt
forceInterruptCheck(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    void (*iccFunc)();
    StackPage *thePage;


	/* Do _not_ set stackLimit until the stack system has been initialized.
	   stackLimit is the initialization flag for the stack system. */
	if (!GIV(stackLimit)) {
		return null;
	}
	thePage = GIV(stackPage);
	if ((thePage)
	 && (thePage != 0)) {
		(thePage->stackLimit = ((char *) (((usqInt) -1))));
	}
	GIV(stackLimit) = ((char *) (((usqInt) -1)));
	sqLowLevelMFence();

	/* There is a race condition if we test the function and then dereference
	   it a second time to call it.  This is called from interrupt code but at the
	   same time other code could be clearing the interruptCheckChain via
	   setInterruptCheckChain:. */
	if (((iccFunc = interruptCheckChain))) {
		iccFunc();
	}
	GIV(statForceInterruptCheck) += 1;
	return 0;
}