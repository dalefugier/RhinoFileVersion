using Rhino;
using Rhino.Commands;
using Rhino.Input.Custom;
using Rhino.UI;

namespace RhinoFileVersion
{
  public class RhinoFileVersionCommand : Command
  {
    public override string EnglishName => "RhinoFileVersion";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      string filename;
      if (mode == RunMode.Interactive)
      {
        var fd = new Eto.Forms.OpenFileDialog { Title = "Open" };
        fd.Filters.Add(new Eto.Forms.FileFilter("Rhino 3D Models(*.3dm)", RhinoFileVersionPlugIn.FileExtensions()));
        var rc = fd.ShowDialog(RhinoEtoApp.MainWindow);
        if (rc != Eto.Forms.DialogResult.Ok)
          return Result.Cancel;

        filename = fd.FileName;
      }
      else
      {
        var gs = new GetString();
        gs.SetCommandPrompt("Name of Rhino file to query");
        gs.Get();
        if (gs.CommandResult() != Result.Success)
          return gs.CommandResult();

        filename = gs.StringResult();
      }

      filename.Trim(' ', '"');
      if (string.IsNullOrEmpty(filename))
        return Result.Nothing;

      var version = RhinoFileVersionPlugIn.FileVersion(filename, false);
      if (version == RhinoVersion.Error)
        RhinoApp.WriteLine("Specified file is not a Rhino format file.");
      else
        RhinoApp.Write("\"{0}\" is a {1} format file.\n", filename, version.ToFriendlyString());

      return Result.Success;
    }
  }
}