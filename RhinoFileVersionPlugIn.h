/////////////////////////////////////////////////////////////////////////////
// RhinoFileVersionPlugIn.h : main header file for the RhinoFileVersion plug-in
//

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRhinoFileVersionWatcher
// See RhinoFileVersionPlugIn.cpp for the implementation of this class
//

class CRhinoFileVersionWatcher : public CRhinoEventWatcher
{
  void OnBeginOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference );
};

/////////////////////////////////////////////////////////////////////////////
// CRhinoFileVersionPlugIn
// See RhinoFileVersionPlugIn.cpp for the implementation of this class
//

class CRhinoFileVersionPlugIn : public CRhinoUtilityPlugIn
{
public:
  CRhinoFileVersionPlugIn();
  ~CRhinoFileVersionPlugIn();

  // Required overrides
  const wchar_t* PlugInName() const;
  const wchar_t* PlugInVersion() const;
  GUID PlugInID() const;
  BOOL OnLoadPlugIn();
  void OnUnloadPlugIn();

  plugin_load_time PlugInLoadTime();
  
private:
  ON_wString m_plugin_version;
  CRhinoFileVersionWatcher m_watcher;
};

CRhinoFileVersionPlugIn& RhinoFileVersionPlugIn();



