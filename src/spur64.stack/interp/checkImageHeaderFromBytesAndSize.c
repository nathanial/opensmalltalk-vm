/* Extracted from interp.c:50701 (function checkImageHeaderFromBytesAndSize). */

sqInt
checkImageHeaderFromBytesAndSize(char *bytes, sqInt totalSize)
{
    sqInt dataSize;
    sqInt headerSize;
    sqInt version;

	if (totalSize < (BytesPerWord * 16)) {
		return 0;
	}
	version = long32At(bytes);
	headerSize = long32At(bytes + 4);
	dataSize = longAt(bytes + 8);
	if (!((((version | MultipleBytecodeSetsBitmask) - MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */)) {
		if (!(((((SQ_SWAP_4_BYTES(version)) | MultipleBytecodeSetsBitmask) - MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */)) {
			return 0;
		}
		headerSize = SQ_SWAP_8_BYTES(headerSize);
		dataSize = SQ_SWAP_8_BYTES(dataSize);
	}
	return (headerSize == (BytesPerWord * 16))
	 && (totalSize >= (headerSize + dataSize));
}