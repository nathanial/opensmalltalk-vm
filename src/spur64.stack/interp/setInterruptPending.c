/* Extracted from interp.c:63017 (function setInterruptPending). */

/*	This is invoked frrom the platform GUI event subsystem when the user
	inputs a a char code matching interruptKeycode. Set the value and then
	forceInterruptCheck so as to call the interruptCheckChain function if set.
	It's important to set interruptPending *before* calling
	forceInterruptCheck so that functions on the interruptCheckChain can check
	interruptPending.  */

	/* StackInterpreter>>#setInterruptPending: */

void
setInterruptPending(sqInt value)
{
	interruptPending = value;
	forceInterruptCheck();
}