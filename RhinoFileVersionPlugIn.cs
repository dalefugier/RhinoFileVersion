using System.IO;
using System.Linq;
using Rhino;
using Rhino.FileIO;
using Rhino.PlugIns;

namespace RhinoFileVersion
{
  public enum RhinoVersion
  {
    Error,
    Rhino1,
    Rhino2,
    Rhino3,
    Rhino4,
    Rhino5,
    Rhino6,
    Rhino7,
    Rhino8,
    Rhino9,
    Unknown
  }

  public static class RhinoVersionExtensions
  {
    public static string ToFriendlyString(this RhinoVersion ver)
    {
      switch (ver)
      {
        case RhinoVersion.Rhino1:
          return "Rhino 1.0";
        case RhinoVersion.Rhino2:
          return "Rhino 2.0";
        case RhinoVersion.Rhino3:
          return "Rhino 3.0";
        case RhinoVersion.Rhino4:
          return "Rhino 4.0";
        case RhinoVersion.Rhino5:
          return "Rhino 5";
        case RhinoVersion.Rhino6:
          return "Rhino 6";
        case RhinoVersion.Rhino7:
          return "Rhino 7";
        case RhinoVersion.Rhino8:
          return "Rhino 8";
        case RhinoVersion.Rhino9:
          return "Rhino 9";
        case RhinoVersion.Unknown:
        default:
          return "unknown Rhino";
      }
    }
  }

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
      var action = e.Merge ? "Importing" : "Opening";
      var version = FileVersion(e.FileName, true);
      if (version != RhinoVersion.Error)
        RhinoApp.Write("\n{0} a {1} format file.\n", action, version.ToFriendlyString());
    }

    public static string[] FileExtensions()
    {
      return new string[] { ".3dm", ".3dmbak" };
    }

    public static RhinoVersion FileVersion(string filename, bool quiet)
    {
      var rc = RhinoVersion.Error;

      filename.Trim(' ', '"');
      if (string.IsNullOrEmpty(filename))
        return rc;

      var extension = Path.GetExtension(filename);
      if (string.IsNullOrEmpty(extension))
      {
        if (!quiet)
          RhinoApp.WriteLine("Specified file is not a Rhino file.");
        return rc;
      }

      var rhino_extensions = FileExtensions();
      if (!rhino_extensions.Contains(extension))
      {
        if (!quiet)
          RhinoApp.WriteLine("Specified file is not a Rhino file.");
        return rc;
      }

      if (!File.Exists(filename))
      {
        if (!quiet)
          RhinoApp.WriteLine("Specified file not found.");
        return rc;
      }

      try
      { 
        var archive_version = File3dm.ReadArchiveVersion(filename);
        switch (archive_version)
        {
          case 1:
          case 10:
            rc = RhinoVersion.Rhino1;
            break;
          case 2:
          case 20:
            rc = RhinoVersion.Rhino2;
            break;
          case 3:
          case 30:
            rc = RhinoVersion.Rhino3;
            break;
          case 4:
          case 40:
            rc = RhinoVersion.Rhino4;
            break;
          case 5:
          case 50:
            rc = RhinoVersion.Rhino5;
            break;
          case 6:
          case 60:
            rc = RhinoVersion.Rhino6;
            break;
          case 7:
          case 70:
            rc = RhinoVersion.Rhino7;
            break;
          case 8:
          case 80:
            rc = RhinoVersion.Rhino8;
            break;
          case 9:
          case 90:
            rc = RhinoVersion.Rhino9;
            break;
          default:
            rc = RhinoVersion.Unknown;
            break;
        }
      }
      catch
      {
        rc = RhinoVersion.Error;
      }

      return rc;
    }
  }
}