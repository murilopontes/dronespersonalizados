using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace OpenFlight
{
    class TimeSerie : Series
    {
        int limit;
        
        public TimeSerie(int limit) {
            this.limit = limit;
            base.ChartType = SeriesChartType.FastLine;
        }

        public void smart_addy(double value) {
            //remove oldest data
            while (base.Points.Count > this.limit) base.Points.RemoveAt(0);
            //add new data
            base.Points.AddY(value);
        }
    }
}
