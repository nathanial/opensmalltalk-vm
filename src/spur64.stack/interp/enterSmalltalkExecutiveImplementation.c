/* Extracted from interp.c:52442 (function enterSmalltalkExecutiveImplementation). */

static sqInt
enterSmalltalkExecutiveImplementation(void)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
	_setjmp(GIV(reenterInterpreter));
	interpret();
	return 0;
}