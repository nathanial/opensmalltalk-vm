/* Extracted from interp.c:35370 (function firstFixedField). */

	/* SpurMemoryManager>>#firstFixedField: */

void *
firstFixedField(sqInt objOop)
{
	return pointerForOop(objOop + BaseHeaderSize);
}