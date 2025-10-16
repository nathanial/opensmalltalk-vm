/* Extracted from interp.c:27313 (function integerValueOf). */

sqInt
integerValueOf(sqInt oop)
{
	return (((((usqInt)(oop)) >> 0x3F)) == 1
			? ((((((-(numTagBits())) < 0) ? (((usqInt)(oop)) >> (-(-(numTagBits())))) : ((((usqInt)(oop) << (-(numTagBits()))))))) & 0x1FFFFFFFFFFFFFFFLL) - 0x1FFFFFFFFFFFFFFFLL) - 1
			: (((-(numTagBits())) < 0) ? (((usqInt)(oop)) >> (-(-(numTagBits())))) : (((sqInt)((usqInt)(oop) << (-(numTagBits())))))));
}