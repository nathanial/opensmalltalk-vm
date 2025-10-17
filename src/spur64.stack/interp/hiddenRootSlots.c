/* Extracted from interp.c:36216 (function hiddenRootSlots). */

/*	Answer the number of extra root slots in the root of the hidden root
	object. 
 */

	/* SpurMemoryManager>>#hiddenRootSlots */

static sqInt
hiddenRootSlots(void)
{
	return 8;
}