/* Extracted from interp.c:52096 (function dummyReferToProxy). */

/*	Presumably this exists to squash C compiler warnings about unused
	variables... 
 */

	/* StackInterpreter>>#dummyReferToProxy */

struct VirtualMachine * *
dummyReferToProxy(void)
{
	return (&interpreterProxy);
}