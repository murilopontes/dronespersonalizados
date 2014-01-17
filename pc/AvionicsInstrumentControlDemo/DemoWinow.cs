/*****************************************************************************/
/* Project  : AvionicsInstrumentControlDemo                                  */
/* File     : DemoWondow.cs                                                  */
/* Version  : 1                                                              */
/* Language : C#                                                             */
/* Summary  : Start window of the project, use to test the instruments       */
/* Creation : 30/06/2008                                                     */
/* Autor    : Guillaume CHOUTEAU                                             */
/* History  :                                                                */
/*****************************************************************************/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace AvionicsInstrumentControlDemo
{
    public partial class DemoWinow : Form
    {
        public DemoWinow()
        {
            InitializeComponent();
        }

        private void trackBarAirSpeed_Scroll(object sender, EventArgs e)
        {
            airSpeedInstrumentControl1.SetAirSpeedIndicatorParameters(trackBarAirSpeed.Value);
        }

        private void trackBarVerticalSpeed_Scroll(object sender, EventArgs e)
        {
            verticalSpeedInstrumentControl1.SetVerticalSpeedIndicatorParameters(trackBarVerticalSpeed.Value);
        }

        private void trackPitchAngle_Scroll(object sender, EventArgs e)
        {
            horizonInstrumentControl1.SetAttitudeIndicatorParameters(trackPitchAngle.Value, trackBarRollAngle.Value);
        }

        private void trackBarRollAngle_Scroll(object sender, EventArgs e)
        {
            horizonInstrumentControl1.SetAttitudeIndicatorParameters(trackPitchAngle.Value, trackBarRollAngle.Value);
        }

        private void trackBarAltitude_Scroll(object sender, EventArgs e)
        {
            altimeterInstrumentControl1.SetAlimeterParameters(trackBarAltitude.Value);
        }

        private void trackBarHeading_Scroll(object sender, EventArgs e)
        {
            headingIndicatorInstrumentControl1.SetHeadingIndicatorParameters(trackBarHeading.Value);
        }

        private void trackBarTurnRate_Scroll(object sender, EventArgs e)
        {
            turnCoordinatorInstrumentControl1.SetTurnCoordinatorParameters((trackBarTurnRate.Value / 10), trackBarTurnQuality.Value);
        }

        private void trackBarTurnQuality_Scroll(object sender, EventArgs e)
        {
            turnCoordinatorInstrumentControl1.SetTurnCoordinatorParameters((trackBarTurnRate.Value / 10), trackBarTurnQuality.Value);
        }

        private void DemoWinow_Load(object sender, EventArgs e)
        {

        }

        private void horizonInstrumentControl1_Click(object sender, EventArgs e)
        {

        }



    }
}