using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AxiomDX9Game2
{
    internal class Program
    {
        private static void Main()
        {
            try
            {
                System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US", false);
                (new Game()).Run();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(Axiom.Core.LogManager.BuildExceptionString(ex));
            }
        }
    }
}
