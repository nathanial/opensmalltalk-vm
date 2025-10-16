/* Extracted from interp.c:61257 (function putLongtoFile). */

static NoDbgRegParms void
putLongtoFile(sqInt aLong, sqImageFile aFile)
{   DECL_MAYBE_SQ_GLOBAL_STRUCT
    sqInt objectsWritten;

	objectsWritten = sqImageFileWrite((&aLong), sizeof(sqInt), 1, aFile);

	/* begin success: */
	if (!(objectsWritten == 1)) {
		if (!GIV(primFailCode)) {
			GIV(primFailCode) = 1;
		}
	}
}