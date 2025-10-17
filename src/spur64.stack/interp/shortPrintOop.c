/* Extracted from interp.c:63289 (function shortPrintOop). */

	/* StackInterpreter>>#shortPrintOop: */

static NoDbgRegParms void
shortPrintOop(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *className;
    sqInt length;

	length = 0;
	if (((oop & (tagMask())) != 0)) {
		printImmediateObjecton(oop, transcript);
		return;
	}
	if (!(addressCouldBeObj(oop))) {
		printCantBeObjecton(oop, transcript);
		return;
	}
	if (((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
		printFreeObjecton(oop, transcript);
		return;
	}
	if ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		printForwarderon(oop, transcript);
		return;
	}
	className = nameOfClasslengthInto(fetchClassOfNonImm(oop), (&length));
	fprintf(transcript,
			"%p: a(n) %.*s\n",
			((void *)oop),
			((int) length),
			className);
}