/* Extracted from interp.c:57896 (function methodUsesAlternateBytecodeSet). */

/*	A negative header selects the alternate bytecode set. */

/* StackInterpreter>>#methodUsesAlternateBytecodeSet: */

int methodUsesAlternateBytecodeSet(sqInt aMethodObj) {
  sqInt methodHeader;

  methodHeader = methodHeaderOf(aMethodObj);

  /* begin headerIndicatesAlternateBytecodeSet: */
  return (((sqLong)methodHeader)) < 0;
}