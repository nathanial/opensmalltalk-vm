/* Extracted from interp.c:57656 (function maybeSelectorOfMethod). */

/*	Answer the selector of a method, assuming its penultimate literal is
        either a symbol or a pointer object whose first slot references the
   method and whose second slot is a symbol (i.e. an AdditionalMethodState). If
   a Symbol can't be found answer nil. This isn't satisfactory, as it puts a lot
   of information into the VM, but it is needed for adequate crash debugging at
   Cadence. With full blocks as of 9/2016 the last literal of a CompiledBlock is
   a back pointer to the enclosing block or compiled method. */

/* StackInterpreter>>#maybeSelectorOfMethod: */

sqInt maybeSelectorOfMethod(sqInt methodObj) {
  sqInt maybeSelector;
  usqInt numSlots;
  sqInt penultimateLiteral;
  sqInt ultimateLiteral;

  assert(isOopCompiledMethod(methodObj));
  ultimateLiteral = ultimateLiteralOf(methodObj);
  if (/* isOopCompiledMethod: */
      ((!(ultimateLiteral & (tagMask())))) &&
      (((byteAt((void *)(ultimateLiteral + (formatFieldByteOffset())))) &
        (formatMask())) >= (firstCompiledMethodFormat()))) {
    return maybeSelectorOfMethod(ultimateLiteral);
  }
  penultimateLiteral = penultimateLiteralOf(methodObj);
  if (/* isWordsOrBytes: */
      ((!(penultimateLiteral & (tagMask())))) &&
      (isWordsOrBytesNonImm(penultimateLiteral))) {
    return (((longAt((void *)(penultimateLiteral))) & (classIndexMask())) ==
                    (fetchClassTagOfNonImm(fetchPointerofObject(
                        SelectorDoesNotUnderstand, specialObjectsOop)))
                ? penultimateLiteral
                : 0);
  }
  return (
      (/* isPointers: */
       ((!(penultimateLiteral & (tagMask())))) &&
       (((byteAt((void *)(penultimateLiteral + (formatFieldByteOffset())))) &
         (formatMask())) <= 5 /* lastPointerFormat */)) &&
              ((((
                  assert((classIndexOf(penultimateLiteral)) >
                         (isForwardedObjectClassIndexPun())),
                  numSlotsOf(penultimateLiteral))) >= 2) &&
               (((fetchPointerofObject(0U, penultimateLiteral)) == methodObj) &&
                (((maybeSelector =
                       fetchPointerofObject(1U, penultimateLiteral)),
                  (/* isWordsOrBytes: */
                   ((!(maybeSelector & (tagMask())))) &&
                   (isWordsOrBytesNonImm(maybeSelector))) &&
                      (((longAt((void *)(maybeSelector))) &
                        (classIndexMask())) ==
                       (fetchClassTagOfNonImm(fetchPointerofObject(
                           SelectorDoesNotUnderstand, specialObjectsOop))))))))
          ? maybeSelector
          : 0);
}