/* Extracted from interp.c:62991 (function setInterruptCheckChain). */

	/* StackInterpreter>>#setInterruptCheckChain: */

void *
setInterruptCheckChain(void (*aFunction)())
{
    void (*prevFunction)();

	prevFunction = interruptCheckChain;
	interruptCheckChain = aFunction;
	return prevFunction;
}