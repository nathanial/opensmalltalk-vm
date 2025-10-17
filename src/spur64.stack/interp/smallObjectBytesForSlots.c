/* Extracted from interp.c:27657 (function smallObjectBytesForSlots). */

/*	Answer the total number of bytes in an object without an overflow header,
	including header bytes.
 */

	/* Spur64BitMemoryManager>>#smallObjectBytesForSlots: */

usqInt
smallObjectBytesForSlots(sqInt numSlots)
{
	return BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
}