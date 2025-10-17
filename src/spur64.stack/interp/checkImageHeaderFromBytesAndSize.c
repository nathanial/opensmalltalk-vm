/* Extracted from interp.c:50701 (function checkImageHeaderFromBytesAndSize). */

/*	Support for embedded images. Check that the first few bytes of a
   potential header and answer if it looks like something the VM can load, The
   method checks the first three fields of the header (magic, header size & data
   size) & the total size. The magic number should be correct. The header size
   should be correct. The size of the data should be at least as long as the
   headerSize plus the data size in the header */
/*	Need at least headerSize bytes; no point going further if not... */

/* StackInterpreter>>#checkImageHeaderFromBytes:AndSize: */

sqInt checkImageHeaderFromBytesAndSize(char *bytes, sqInt totalSize) {
  sqInt dataSize;
  sqInt headerSize;
  sqInt version;

  if (totalSize < (BytesPerWord * 16)) {
    return 0;
  }
  version = long32At(bytes);
  headerSize = long32At(bytes + 4);
  dataSize = longAt(bytes + 8);
  if (!((((version | MultipleBytecodeSetsBitmask) -
          MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */)) {
    if (!(((((SQ_SWAP_4_BYTES(version)) | MultipleBytecodeSetsBitmask) -
            MultipleBytecodeSetsBitmask)) == 68021 /* imageFormatVersion */)) {
      return 0;
    }
    headerSize = SQ_SWAP_8_BYTES(headerSize);
    dataSize = SQ_SWAP_8_BYTES(dataSize);
  }
  return (headerSize == (BytesPerWord * 16)) &&
         (totalSize >= (headerSize + dataSize));
}