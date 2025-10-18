/* Extracted from interp.c:54798 (function includesBehaviorThatOf). */

/*	Return the equivalent of
        aClass includesBehavior: aSuperclass.
        Note: written for efficiency and better inlining (only 1 temp) */

/* StackInterpreter>>#includesBehavior:ThatOf: */

sqInt includesBehaviorThatOf(sqInt aClass, sqInt aSuperclass) {
  sqInt objOop;
  sqInt theClass;

  if (aSuperclass == nilObj) {
    return 0;
  }
  theClass = aClass;
  while (1) {
    if (theClass == aSuperclass) {
      return 1;
    }
    if (!(theClass != nilObj))
      break;
    /* begin superclassOf: */
    objOop = followObjFieldofObject(SuperclassIndex, theClass);
    theClass = objOop;
  }
  return 0;
}