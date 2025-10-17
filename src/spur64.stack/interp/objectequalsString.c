/* Extracted from interp.c:58162 (function objectequalsString). */

/* StackInterpreter>>#object:equalsString: */

static sqInt objectequalsString(sqInt anOop, char *aCString) {
  sqInt len;

  len = strlen(aCString);
  return /* object:equalsString:ofSize: */
      (  /* isBytes: */
       ((!(anOop & (tagMask())))) &&
       (((byteAt((void *)(anOop + (formatFieldByteOffset())))) &
         (formatMask())) >= (firstByteFormat()))) &&
      ((!(((byteAt((void *)(anOop + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat()))) &&
       (((numBytesOfBytes(anOop)) == len) &&
        ((strncmp(aCString, firstIndexableField(anOop), len)) == 0)));
}