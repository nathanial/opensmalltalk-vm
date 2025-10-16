/* Extracted from interp.c:58223 (function pcPreviousToinSqueakV3PlusClosuresOrSistaV1Method). */

usqInt
pcPreviousToinSqueakV3PlusClosuresOrSistaV1Method(sqInt theIP, sqInt aMethod)
{
    sqInt byte;
    static unsigned char  extensionSizes[] = {
		2, 2, 2, 2, 3, 2, 2, 1, 1, 1, 2, 3, 3, 3, 3, 4
	};
    sqInt methodHeader;
    usqInt previousPC;
    usqInt thisIP;

	/* begin methodUsesAlternateBytecodeSet: */
	/* begin methodHeaderOf: */
	assert(isCompiledMethod(aMethod));
	methodHeader = longAt((void *)((aMethod + BaseHeaderSize) + ((((usqInt)(HeaderIndex) << (shiftForWord()))))));
	if ((((sqLong) methodHeader)) < 0) {
		/* begin pcPreviousTo:inSistaV1Method: */
		thisIP = ((((literalCountOf(aMethod)) + LiteralStart) * BytesPerOop) + aMethod) + BaseHeaderSize;
		assert(oopisLessThan(thisIP, theIP));
		while (oopisLessThan(thisIP, theIP)) {
			previousPC = thisIP;
			do {
				byte = byteAt((void *)(thisIP));
				thisIP += /* sizeOfSistaV1Bytecode: */
						(byte < 224
							? 1
							: (byte < 0xF8
									? 2
									: 3));
			} while(((byte >= 224) && (byte <= 225)));
		}
		return previousPC;
	}

	/* begin pcPreviousTo:inSqueakV3PlusClosuresMethod: */
	thisIP = ((((literalCountOf(aMethod)) + LiteralStart) * BytesPerOop) + aMethod) + BaseHeaderSize;
	assert(oopisLessThan(thisIP, theIP));
	while (oopisLessThan(thisIP, theIP)) {
		previousPC = thisIP;
		byte = byteAt((void *)(thisIP));
		thisIP += (byte <= 125
					? 1
					: (byte >= 176
							? 1
							: (byte >= 160
									? 2
									: (byte >= 144
											? 1
											: (byte >= 128
													? extensionSizes[byte - 128]
													: (assertf("illegal bytecode in sizeOfSqueakV3Bytecode:"),
													1))))));
	}
	return previousPC;
}