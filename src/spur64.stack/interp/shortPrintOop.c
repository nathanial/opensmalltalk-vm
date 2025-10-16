/* Extracted from interp.c:63289 (function shortPrintOop). */

static NoDbgRegParms void
shortPrintOop(sqInt oop)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    char *className;
    sqInt length;

	length = 0;
	if (((oop & (tagMask())) != 0)) {
		printImmediateObjecton(oop, GIV(transcript));
		return;
	}
	if (!(addressCouldBeObj(oop))) {
		printCantBeObjecton(oop, GIV(transcript));
		return;
	}
	if (((longAt((void *)(oop))) & (classIndexMask())) == (isFreeObjectClassIndexPun())) {
		printFreeObjecton(oop, GIV(transcript));
		return;
	}
	if ((!((longAt((void *)(oop))) & ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
		printForwarderon(oop, GIV(transcript));
		return;
	}
	className = nameOfClasslengthInto(fetchClassOfNonImm(oop), (&length));
	fprintf(GIV(transcript),
			"%p: a(n) %.*s\n",
			((void *)oop),
			((int) length),
			className);
}