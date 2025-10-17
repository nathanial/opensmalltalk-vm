/* Extracted from interp.c:17826 (function primitiveImageFormatVersion). */

/*	Answer an integer identifying the type of image. The image version
   number may identify the format of the image (e.g. 32 or 64-bit word size) or
        specific requirements
        of the image (e.g. block closure support required).

        This is a named (not numbered) primitive in the null module (ie the VM)
 */

/* InterpreterPrimitives>>#primitiveImageFormatVersion */

EXPORT(sqInt)
primitiveImageFormatVersion(void) {
  sqInt oop;
  char *sp;

  oop = ((((((usqInt)(/* imageFormatVersionForSnapshot */
                      (multipleBytecodeSetsActive
                           ? 68533 /* (imageFormatVersion bitOr:
                                      MultipleBytecodeSetsBitmask) */
                           : 68021 /* imageFormatVersion */)))) &
           0xFFFFFFFFU)
          << 3) |
         1);

  /* begin pop:thenPush: */
  longAtput((sp = stackPointer), oop);
  stackPointer = sp;
  return 0;
}