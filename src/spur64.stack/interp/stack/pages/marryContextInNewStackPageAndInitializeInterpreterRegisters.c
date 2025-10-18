/* Extracted from interp.c:57418 (function
 * marryContextInNewStackPageAndInitializeInterpreterRegisters). */

/*	Establish aContext at the base of a new stackPage, make the stackPage
   the active one and set-up the interreter registers. This is used to boot the
        system and bring it back after a snapshot. */

/* StackInterpreter>>#marryContextInNewStackPageAndInitializeInterpreterRegisters:
 */

static void
marryContextInNewStackPageAndInitializeInterpreterRegisters(sqInt aContext) {
  sqInt aMethodObj;
  sqInt methodHeader;
  StackPage *newPage;
  sqInt top;

  assert(!stackPage);
  newPage = makeBaseFrameFor(aContext);

  /* begin setStackPageAndLimit: */
  assert(newPage);
  stackPage = newPage;
  if (stackLimit != (((char *)(((usqInt)-1))))) {
    stackLimit = (stackPage->stackLimit);
  }
  markStackPageMostRecentlyUsed(newPage);

  /* begin setStackPointersFromPage: */
  stackPointer = (newPage->headSP);
  framePointer = (newPage->headFP);
  aMethodObj = longAt(((stackPage->headFP)) + FoxMethod);

  /* begin setMethod: */
  method = aMethodObj;
  assert(isOopCompiledMethod(method));

  /* begin methodUsesAlternateBytecodeSet: */
  methodHeader = methodHeaderOf(method);
  if ((((sqLong)methodHeader)) < 0) {
    bytecodeSetSelector = 0x100;
  } else {
    bytecodeSetSelector = 0;
  }

  /* begin popStack */
  top = longAt(stackPointer);
  stackPointer += BytesPerWord;
  instructionPointer = top;
}