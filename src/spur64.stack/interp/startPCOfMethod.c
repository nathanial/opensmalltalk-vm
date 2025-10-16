/* Extracted from interp.c:64479 (function startPCOfMethod). */

usqInt
startPCOfMethod(sqInt aCompiledMethod)
{
	return ((literalCountOf(aCompiledMethod)) + LiteralStart) * BytesPerOop;
}