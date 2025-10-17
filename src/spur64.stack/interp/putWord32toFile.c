/* Extracted from interp.c:61417 (function putWord32toFile). */

/*	Append aWord32 to aFile in this platform's 'natural' byte order. aWord32
	is 32 bits,
	depending on ObjectMemory. (Bytes will be swapped, if necessary, when the
	image is read
	on a different platform.) Set successFlag to false if the write fails. */

	/* StackInterpreter>>#putWord32:toFile: */

static NoDbgRegParms void
putWord32toFile(int aWord32, sqImageFile aFile)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objectsWritten;

	objectsWritten = sqImageFileWrite((&aWord32), 4, 1, aFile);

	/* begin success: */
	if (!(objectsWritten == 1)) {
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
}