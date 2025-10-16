/* Extracted from interp.c:27303 (function integerObjectOf). */

sqInt
integerObjectOf(sqInt value)
{
	return (((((usqInt)value)) << (numTagBits()))) + 1;
}