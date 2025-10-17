/* Extracted from interp.c:37182 (function isCompiledMethodFormat). */

	/* SpurMemoryManager>>#isCompiledMethodFormat: */

static NoDbgRegParms int
isCompiledMethodFormat(sqInt format)
{
	return format >= (firstCompiledMethodFormat());
}