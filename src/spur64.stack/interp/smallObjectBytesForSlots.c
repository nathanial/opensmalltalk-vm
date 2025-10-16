/* Extracted from interp.c:27657 (function smallObjectBytesForSlots). */

usqInt
smallObjectBytesForSlots(sqInt numSlots)
{
	return BaseHeaderSize + ((numSlots < 1
		? 8 /* allocationUnit */
		: numSlots * BytesPerOop));
}