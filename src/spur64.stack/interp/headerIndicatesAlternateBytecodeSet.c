/* Extracted from interp.c:27084 (function headerIndicatesAlternateBytecodeSet). */

/*	A negative header selects the alternate bytecode set. */

	/* Spur64BitMemoryManager>>#headerIndicatesAlternateBytecodeSet: */

int
headerIndicatesAlternateBytecodeSet(sqInt methodHeader)
{
	return (((sqLong) methodHeader)) < 0;
}