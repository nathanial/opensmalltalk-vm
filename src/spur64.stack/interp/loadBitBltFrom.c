/* Extracted from interp.c:55585 (function loadBitBltFrom). */

/*	This entry point needs to be implemented for the interpreter proxy.
        Since BitBlt is now a plugin we need to look up
        BitBltPlugin:=loadBitBltFrom and call it. This entire mechanism should
        eventually go away and be
        replaced with a dynamic lookup from BitBltPlugin itself but for backward
        compatibility this stub is provided
 */

/* StackInterpreter>>#loadBitBltFrom: */

sqInt loadBitBltFrom(sqInt bb) {
  void *fn;

  fn = ioLoadFunctionFrom("loadBitBltFrom", "BitBltPlugin");
  if (!fn) {
    /* begin primitiveFail */
    if (!primFailCode) {
      primFailCode = 1;
    }
    return null;
  }
  return ((sqInt (*)(sqInt))fn)(bb);
}