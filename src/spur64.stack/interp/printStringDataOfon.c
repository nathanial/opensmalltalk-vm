/* Extracted from interp.c:44636 (function printStringDataOfon). */

	/* SpurMemoryManager>>#printStringDataOf:on: */

static NoDbgRegParms void
printStringDataOfon(sqInt oop, FILE *aStream)
{
    char *buffer;
    sqInt byte;
    sqInt i;
    sqInt limit;
    sqInt n;
    unsigned int *wideBuffer;
    sqInt word;

	if (((byteAt((void *)(oop + (formatFieldByteOffset())))) & (formatMask())) >= (firstByteFormat())) {
		buffer = ((char *) (alloca(0x400)));
		n = (i = 0);
		limit = (((numBytesOfBytes(oop)) < 0x100) ? (numBytesOfBytes(oop)) : 0x100);
		while (n < limit) {
			byte = byteAt((void *)((oop + BaseHeaderSize) + i));
			i += 1;
			if ((byte < 32)
			 && (byte != 9)) {
				buffer[n] = '<';
				n += 1;
				if ((byte == 10)
				 || (byte == 13)) {
					if (byte == 10) {
						buffer[n] = 'L';
						buffer[n + 1] = 'F';
					}
					else {
						buffer[n] = 'C';
						buffer[n + 1] = 'R';
					}
					n += 2;
				}
				else {
					if (byte >= 10) {
						buffer[n] = ((byte / 10) + (((sqInt)'0')));
						n += 1;
					}
					buffer[n] = ((byte % 10) + (((sqInt)'0')));
					n += 1;
				}
				buffer[n] = '>';
				n += 1;
			}
			else {
				buffer[n] = byte;
				n += 1;
			}
		}
		fprintf(aStream,
				"%.*s%s\n",
				((int) n),
				buffer,
				((numBytesOfBytes(oop)) > limit
				? "..."
				: ""));
	}
	else {
		assert(isWordsNonImm(oop));
		wideBuffer = ((unsigned int *) (alloca(0x1000)));
		n = (i = 0);
		limit = (((lengthOf(oop)) < 0x100) ? (lengthOf(oop)) : 0x100);
		while (i < limit) {
			word = long32At((void *)((oop + BaseHeaderSize) + ((((usqInt)(i) << 2)))));
			i += 1;
			if ((word < 32)
			 && (word != 9)) {
				wideBuffer[n] = '<';
				n += 1;
				if ((word == 10)
				 || (word == 13)) {
					if (word == 10) {
						wideBuffer[n] = 'L';
						wideBuffer[n + 1] = 'F';
					}
					else {
						wideBuffer[n] = 'C';
						wideBuffer[n + 1] = 'R';
					}
					n += 2;
				}
				else {
					if (word >= 10) {
						wideBuffer[n] = ((word / 10) + (((sqInt)'0')));
						n += 1;
					}
					wideBuffer[n] = ((word % 10) + (((sqInt)'0')));
					n += 1;
				}
				wideBuffer[n] = '>';
				n += 1;
			}
			else {
				wideBuffer[n] = word;
				n += 1;
			}
		}
		fwprintf(aStream,
				L"%.*ls%s\n",
				((int) n),
				wideBuffer,
				((lengthOf(oop)) > limit
				? "..."
				: ""));
	}
}