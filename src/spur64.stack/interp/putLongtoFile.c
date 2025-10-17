/* Extracted from interp.c:61257 (function putLongtoFile). */

/*	Append aLong to aFile in this platform's 'natural' byte order. aLong is
        either 32 or 64 bits,
        depending on ObjectMemory. (Bytes will be swapped, if necessary, when
   the image is read on a different platform.) Set successFlag to false if the
   write fails. */

/* StackInterpreter>>#putLong:toFile: */

static NoDbgRegParms void putLongtoFile(sqInt aLong, sqImageFile aFile) {
  sqInt objectsWritten;

  objectsWritten = sqImageFileWrite((&aLong), sizeof(sqInt), 1, aFile);

  /* begin success: */
  if (!(objectsWritten == 1)) {
    if (!primFailCode) {
      primFailCode = 1;
    }
  }
}