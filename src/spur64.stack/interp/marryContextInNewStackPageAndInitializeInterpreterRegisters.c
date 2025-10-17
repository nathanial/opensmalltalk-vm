/* Extracted from interp.c:57418 (function marryContextInNewStackPageAndInitializeInterpreterRegisters). */

/*	Establish aContext at the base of a new stackPage, make the stackPage the
	active one and set-up the interreter registers. This is used to boot the
	system and bring it back after a snapshot. */

	/* StackInterpreter>>#marryContextInNewStackPageAndInitializeInterpreterRegisters: */

static NoDbgRegParms void
marryContextInNewStackPageAndInitializeInterpreterRegisters(sqInt aContext)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt aMethodObj;
    sqInt methodHeader;
    StackPage *newPage;
    sqInt top;

	assert(!GIV(stackPage));
	newPage = makeBaseFrameFor(aContext);

	/* begin setStackPageAndLimit: */
	assert(newPage);
	GIV(stackPage) = newPage;
	if (GIV(stackLimit) != (((char *) (((usqInt) -1))))) {
		GIV(stackLimit) = (GIV(stackPage)->stackLimit);
	}
	markStackPageMostRecentlyUsed(newPage);

	/* begin setStackPointersFromPage: */
	GIV(stackPointer) = (newPage->headSP);
	GIV(framePointer) = (newPage->headFP);
	aMethodObj = longAt(((GIV(stackPage)->headFP)) + FoxMethod);

	/* begin setMethod: */
	GIV(method) = aMethodObj;
	assert(isOopCompiledMethod(GIV(method)));

	/* begin methodUsesAlternateBytecodeSet: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(GIV(method)));
	methodHeader = longAt((void *)((GIV(method) + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if ((((sqLong) methodHeader)) < 0) {
		GIV(bytecodeSetSelector) = 0x100;
	}
	else {
		GIV(bytecodeSetSelector) = 0;
	}

	/* begin popStack */
	top = longAt(GIV(stackPointer));
	GIV(stackPointer) += BytesPerWord;
	GIV(instructionPointer) = top;
}