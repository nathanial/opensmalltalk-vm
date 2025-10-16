/* Extracted from interp.c:65620 (function validStackPageBaseFrame). */

static NoDbgRegParms sqInt
validStackPageBaseFrame(StackPage *aPage)
{
    sqInt senderContextOrNil;

	if (!(asserta(isBaseFrame((aPage->baseFP))))) {
		return 0;
	}

	/* begin frameCallerContext: */
	assert(isBaseFrame((aPage->baseFP)));
	senderContextOrNil = longAt(((aPage->baseFP)) + FoxCallerContext);
	if (!(asserta(addressCouldBeObj(senderContextOrNil)))) {
		return 0;
	}
	if (!(asserta((senderContextOrNil == (nilObject()))
		 || (isContextNonImm(senderContextOrNil))))) {
		return 0;
	}
	return 1;
}