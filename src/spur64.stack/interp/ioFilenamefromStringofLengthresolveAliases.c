/* Extracted from interp.c:54940 (function ioFilenamefromStringofLengthresolveAliases). */

/*	the vm has to convert aFilenameString via any canonicalization and
	char-mapping and put the result in aCharBuffer.
	Note the resolveAliases flag - this is an awful artefact of OSX and Apples
	demented alias handling. When opening a file, the flag must be true, when
	closing or renaming it must be false. Sigh.
 */

	/* StackInterpreter>>#ioFilename:fromString:ofLength:resolveAliases: */

void
ioFilenamefromStringofLengthresolveAliases(char *aCharBuffer, char *aFilenameString, sqInt filenameLength, sqInt aBoolean)
{
	sqGetFilenameFromString(aCharBuffer, aFilenameString, filenameLength, aBoolean);
}