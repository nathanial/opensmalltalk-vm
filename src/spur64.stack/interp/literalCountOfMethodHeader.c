/* Extracted from interp.c:38207 (function literalCountOfMethodHeader). */

usqInt
literalCountOfMethodHeader(sqInt header)
{
	assert((((header) & 7) == 1));
	return ((header >> 3)) & AlternateHeaderNumLiteralsMask;
}