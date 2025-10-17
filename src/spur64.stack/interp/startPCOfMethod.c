/* Extracted from interp.c:64479 (function startPCOfMethod). */

/*	Zero-relative version of CompiledMethod>>startpc. */

	/* StackInterpreter>>#startPCOfMethod: */

usqInt
startPCOfMethod(sqInt aCompiledMethod)
{
	return ((literalCountOf(aCompiledMethod)) + LiteralStart) * BytesPerOop;
}