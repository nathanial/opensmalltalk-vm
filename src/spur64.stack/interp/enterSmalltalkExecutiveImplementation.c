/* Extracted from interp.c:52442 (function enterSmalltalkExecutiveImplementation). */

/*	Main entry-point into the interpreter at each execution level, where an
	execution level is either the start of execution or reentry for a
	callback. This is the actual implementation, separated from
	enterSmalltalkExecutive so the
	simulator can wrap it in an exception handler and hence simulate the
	setjmp/longjmp.  */
/*	Setjmp for reentry into interpreter from elsewhere, e.g. FFI exception
	primitive failure.
 */

	/* StackInterpreter>>#enterSmalltalkExecutiveImplementation */

static sqInt
enterSmalltalkExecutiveImplementation(void)
{
	_setjmp(reenterInterpreter);
	interpret();
	return 0;
}