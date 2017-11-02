using System.IO;
using Rhino;
using Rhino.FileIO;
using Rhino.PlugIns;

namespace RhinoFileVersion
{
  /// <summary>
  /// RhinoFileVersionPlugIn plug-in class
  /// </summary>
  public class RhinoFileVersionPlugIn : PlugIn
  {
    /// <summary>
    /// PlugIn.PlugInLoadTime override
    /// </summary>
    public override PlugInLoadTime LoadTime => PlugInLoadTime.AtStartup;

    /// <summary>
    /// PlugIn.OnLoad override
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      RhinoDoc.BeginOpenDocument += OnBeginOpenDocument;
      return LoadReturnCode.Success;
    }

    /// <summary>
    /// Called when Rhino begins to open a new document
    /// </summary>
    protected void OnBeginOpenDocument(object sender, DocumentOpenEventArgs e)
    {
      if (string.IsNullOrEmpty(e.FileName))
        return;

      var extension = Path.GetExtension(e.FileName);
      if (!extension.Equals(".3dm") && !extension.Equals(".3dmbak"))
        return;

      if (!File.Exists(e.FileName))
        return;

      var action = e.Merge ? "Importing" : "Opening";
      string version;

      var archive_version = File3dm.ReadArchiveVersion(e.FileName);
      switch (archive_version)
      {
        case 1:
          version = "Rhino 1.0";
          break;
        case 2:
          version = "Rhino 2.0";
          break;
        case 3:
          version = "Rhino 3.0";
          break;
        case 4:
          version = "Rhino 4.0";
          break;
        case 50:
          version = "Rhino 5";
          break;
        case 60:
          version = "Rhino 6";
          break;
        default:
          version = "unknown Rhino";
          break;
      }

      RhinoApp.Write("\n{0} a {1} format file.\n", action, version);
    }
  }
}