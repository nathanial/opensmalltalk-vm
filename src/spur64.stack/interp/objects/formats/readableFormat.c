/* Extracted from interp.c:61454 (function readableFormat). */

/*	Anwer if images of the given format are readable by this interpreter.
        Allows a virtual machine to accept selected older image formats. */

/* StackInterpreter>>#readableFormat: */

sqInt readableFormat(sqInt imageVersion) {
  return (((imageVersion | MultipleBytecodeSetsBitmask) -
           MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */;
}