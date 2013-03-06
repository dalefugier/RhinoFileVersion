/////////////////////////////////////////////////////////////////////////////
// RhinoFileVersionPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "RhinoFileVersionPlugIn.h"

// For calculating the size of arrays for safe string functions
#ifndef cch
 #define ccht(Array, EltType) (sizeof(Array) / sizeof(EltType))
 #define cch(Array) ccht(Array, (Array)[0])
#endif

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"Rhino 3DM File Version" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"74139787-1BD1-4673-9F65-3A7F12D931B3" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__"  "__TIME__ )

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION( L"Robert McNeel & Associates" );
RHINO_PLUG_IN_DEVELOPER_ADDRESS( L"3670 Woodland Park Avenue North\015\012Seattle WA 98103" );
RHINO_PLUG_IN_DEVELOPER_COUNTRY( L"United States" );
RHINO_PLUG_IN_DEVELOPER_PHONE( L"206-545-6877" );
RHINO_PLUG_IN_DEVELOPER_FAX( L"206-545-7321" );
RHINO_PLUG_IN_DEVELOPER_EMAIL( L"tech@mcneel.com" );
RHINO_PLUG_IN_DEVELOPER_WEBSITE( L"http://www.rhino3d.com" );
RHINO_PLUG_IN_UPDATE_URL( L"http://www.rhino3d.com" );

// The one and only CRhinoFileVersionPlugIn object
static CRhinoFileVersionPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CRhinoFileVersionPlugIn definition

CRhinoFileVersionPlugIn& RhinoFileVersionPlugIn()
{ 
  return thePlugIn; 
}

CRhinoFileVersionPlugIn::CRhinoFileVersionPlugIn()
{
  m_plugin_version = RhinoPlugInVersion();
}

CRhinoFileVersionPlugIn::~CRhinoFileVersionPlugIn()
{
}

const wchar_t* CRhinoFileVersionPlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CRhinoFileVersionPlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CRhinoFileVersionPlugIn::PlugInID() const
{
  // {74139787-1BD1-4673-9F65-3A7F12D931B3}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CRhinoFileVersionPlugIn::OnLoadPlugIn()
{
  m_watcher.Register();
  m_watcher.Enable();

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CRhinoFileVersionPlugIn::OnUnloadPlugIn()
{
  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

CRhinoPlugIn::plugin_load_time CRhinoFileVersionPlugIn::PlugInLoadTime()
{
  return CRhinoPlugIn::load_plugin_at_startup;
}

void CRhinoFileVersionWatcher::OnBeginOpenDocument( CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference )
{
  if( 0 == filename || 0 == filename[0] || !CRhinoFileUtilities::FileExists(filename) )
    return;
  
  wchar_t ext[_MAX_EXT];
  wmemset( ext, 0, cch(ext) );
  errno_t rc = _wsplitpath_s( filename, 0, 0, 0, 0, 0, 0, ext, cch(ext) );
  if( 0 == ext[0] || (0 != _wcsicmp(L".3dm", ext) && 0 != _wcsicmp(L".3dmbak", ext)) )
    return;

  int rhino_file_version = 0;

  FILE* archive_fp = ON::OpenFile( filename, L"rb" );
  if( archive_fp )
  {
    ON_BinaryFile archive( ON::read3dm, archive_fp );

    int file_version = 0;
    ON_String start_section_comments;
    if( archive.Read3dmStartSection(&file_version, start_section_comments) )
      rhino_file_version = file_version;

    ON::CloseFile( archive_fp );
  }

  switch( rhino_file_version )
  {
  case 1:
    {
      if( bMerge )
        RhinoApp().Print( L"\nImporting a Rhino 1.0 format file.\n" );
      else
        RhinoApp().Print( L"\nOpening a Rhino 1.0 format file.\n" );
    }
    break;
  case 2:
      if( bMerge )
        RhinoApp().Print( L"\nImporting a Rhino 2.0 format file.\n" );
      else
        RhinoApp().Print( L"\nOpening a Rhino 2.0 format file.\n" );
    break;
  case 3:
      if( bMerge )
        RhinoApp().Print( L"\nImporting a Rhino 3.0 format file.\n" );
      else
        RhinoApp().Print( L"\nOpening a Rhino 3.0 format file.\n" );
    break;
  case 4:
      if( bMerge )
        RhinoApp().Print( L"\nImporting a Rhino 4.0 format file.\n" );
      else
        RhinoApp().Print( L"\nOpening a Rhino 4.0 format file.\n" );
    break;
  case 5:
  case 50:
      if( bMerge )
        RhinoApp().Print( L"\nImporting a Rhino 5.0 format file.\n" );
      else
        RhinoApp().Print( L"\nOpening a Rhino 5.0 format file.\n" );
    break;
  }
}
