/* Extracted from interp.c:32020 (function characterValueOf). */

usqInt
characterValueOf(sqInt oop)
{
	return ((((usqInt)oop))) >> (numTagBits());
}