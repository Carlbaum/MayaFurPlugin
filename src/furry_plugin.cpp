#include "furry_plugin.h"
#include "furry_system_node.h"

MStatus initializePlugin(MObject obj) {
  MStatus status;
  MFnPlugin plugin(obj, "Kebaum, Nova, Kingen", "0.1", "Any");

  // Create the hair system node
  status = plugin.registerNode("FurrySystem", FurrySystemNode::id,
                               FurrySystemNode::creator,
                               FurrySystemNode::initialize);
  CHECK_MSTATUS_AND_RETURN_IT(status);
  return status;
}

MStatus uninitializePlugin(MObject obj) {
  cout << ">> Uninitializing plugin \n";
  MStatus   status;
  MFnPlugin plugin(obj);
  status = plugin.deregisterNode(FurrySystemNode::id);
  CHECK_MSTATUS_AND_RETURN_IT(status);
  return status;
}
