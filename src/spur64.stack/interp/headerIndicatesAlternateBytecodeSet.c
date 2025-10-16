/* Extracted from interp.c:27084 (function headerIndicatesAlternateBytecodeSet). */

int
headerIndicatesAlternateBytecodeSet(sqInt methodHeader)
{
	return (((sqLong) methodHeader)) < 0;
}