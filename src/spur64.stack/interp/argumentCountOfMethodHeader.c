/* Extracted from interp.c:50118 (function argumentCountOfMethodHeader). */

	/* StackInterpreter>>#argumentCountOfMethodHeader: */

usqInt
argumentCountOfMethodHeader(sqInt header)
{
	return (((usqInt)(header)) >> MethodHeaderArgCountShift) & 15;
}