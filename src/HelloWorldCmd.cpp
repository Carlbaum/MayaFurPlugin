//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include <iostream>
#include <maya/MSimple.h>
#include <maya/MIOStream.h>
DeclareSimpleCommand(HelloWorld, "Autodesk", "2016");
MStatus HelloWorld::doIt(const MArgList&) {
  std::cout << "Hello World\n" << std::endl;
  return MS::kSuccess;
}
