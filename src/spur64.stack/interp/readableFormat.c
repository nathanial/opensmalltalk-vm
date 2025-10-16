/* Extracted from interp.c:61454 (function readableFormat). */

sqInt
readableFormat(sqInt imageVersion)
{
	return (((imageVersion | MultipleBytecodeSetsBitmask) - MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */;
}