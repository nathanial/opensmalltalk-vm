/* Extracted from interp.c:61278 (function putShorttoFile). */

/*	Append the 16-bit aShort to aFile in this platform's 'natural' byte order.
	(Bytes will be swapped, if necessary, when the image is read on a
	different platform.) Set successFlag to false if the write fails. */

	/* StackInterpreter>>#putShort:toFile: */

static NoDbgRegParms void
putShorttoFile(short aShort, sqImageFile aFile)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objectsWritten;

	objectsWritten = sqImageFileWrite((&aShort), sizeof(short), 1, aFile);

	/* begin success: */
	if (!(objectsWritten == 1)) {
		if (!primFailCode) {
			primFailCode = 1;
		}
	}
}