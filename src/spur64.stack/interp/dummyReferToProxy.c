/* Extracted from interp.c:52096 (function dummyReferToProxy). */

struct VirtualMachine * *
dummyReferToProxy(void)
{
	return (&interpreterProxy);
}