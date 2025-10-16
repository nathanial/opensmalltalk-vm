/* Extracted from interp.c:37939 (function isinstanceOfcompactClassIndex). */

static NoDbgRegParms sqInt
isinstanceOfcompactClassIndex(sqInt oop, sqInt classOop, sqInt compactClassIndex)
{
    sqInt ccIndex;

	if (((oop & (tagMask())) != 0)) {
		return 0;
	}

	/* begin isClassOfNonImm:equalTo:compactClassIndex: */
	assert(!(isImmediate(oop)));
	ccIndex = (longAt((void *)(oop))) & (classIndexMask());
	if (compactClassIndex) {
		return compactClassIndex == ccIndex;
	}
	else {
		return classOop == (classAtIndex(ccIndex));
	}
}