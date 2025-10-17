/* Extracted from interp.c:40759 (function newSpaceIsEmpty). */

	/* SpurMemoryManager>>#newSpaceIsEmpty */

static sqInt
newSpaceIsEmpty(void)
{
	return (freeStart == (((eden).start)))
	 && (pastSpaceStart == (((pastSpace).start)));
}