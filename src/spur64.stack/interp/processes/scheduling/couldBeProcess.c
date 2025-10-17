/* Extracted from interp.c:51594 (function couldBeProcess). */

/* StackInterpreter>>#couldBeProcess: */

static NeverInline sqInt couldBeProcess(sqInt oop) {
  return (addressCouldBeObj(oop)) &&
         ((((byteAt((void *)(oop + (formatFieldByteOffset())))) &
            (formatMask())) <= 5 /* lastPointerFormat */) &&
          ((!(((longAt((void *)(oop))) & (classIndexMask())) ==
              ClassMethodContextCompactIndex)) &&
           (((lengthOf(oop)) > MyListIndex) &&
            (isContext(fetchPointerofObject(SuspendedContextIndex, oop))))));
}