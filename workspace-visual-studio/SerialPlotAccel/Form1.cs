using Axiom.Core;
using Axiom.Graphics;
using Axiom.Math;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace SerialPlotAccel
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }


        Series sax = new Series("ax");
        Series say = new Series("ay");
        Series saz = new Series("az");

        Series ssx = new Series("sx");
        Series ssy = new Series("sy");
        Series ssz = new Series("sz");

        Series spx = new Series("px");
        Series spy = new Series("py");
        Series spz = new Series("pz");




        private Root _engine;
        private RenderWindow _window;
        private SceneManager _scene;
        private Camera _camera;
        private System.Windows.Forms.Timer _timer_frame = new System.Windows.Forms.Timer();
        SceneNode node = null;

        void axiom_init() {


            //
            System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US", false);

            //
            _engine = new Root("Game.log");
            _engine.RenderSystem = _engine.RenderSystems[0];
            _engine.Initialize(false);

            //
            Axiom.Collections.NamedParameterList paramList = new Axiom.Collections.NamedParameterList();
            paramList["externalWindowHandle"] = pictureBox1.Handle;
            _window = _engine.CreateRenderWindow("RenderWindow", pictureBox1.Width, pictureBox1.Height, false, paramList);

            //
            ResourceGroupManager.Instance.AddResourceLocation("media", "Folder", true);

            //
            _scene = _engine.CreateSceneManager("DefaultSceneManager", "DefaultSM");
            _scene.ClearScene();
            //_scene = Root.Instance.CreateSceneManager(SceneType.ExteriorClose);
            //_scene.LoadWorldGeometry("Terrain.xml");

            _camera = _scene.CreateCamera("MainCamera");
            _camera.Position = new Vector3(0, 170, 300); // x=right/left from screen,y=bottom/up from screen,z=in/out from scren
            _camera.LookAt(Vector3.Zero);
            _camera.Near = 5;
            _camera.AutoAspectRatio = true;

            //
            var vp = _window.AddViewport(_camera, 0, 0, 1.0f, 1.0f, 100);
            vp.BackgroundColor = ColorEx.CornflowerBlue;
            ResourceGroupManager.Instance.InitializeAllResourceGroups();



            //shadows and ambient light
            _scene.AmbientLight = ColorEx.Black;

            //_scene.ShadowTechnique = ShadowTechnique.StencilAdditive;
            _scene.ShadowTechnique = ShadowTechnique.StencilModulative;
            //_scene.ShadowTechnique = ShadowTechnique.TextureAdditive;
            //_scene.ShadowTechnique = ShadowTechnique.TextureModulative;


            Entity ent = _scene.CreateEntity("Penguin", "penguin.mesh");
            ent.CastShadows = true;

           
            node = _scene.RootSceneNode.CreateChildSceneNode("PenguinNode");
            node.AttachObject(ent);
            node.Position += new Vector3(25, 0, 0);
            //node.Scale = new Vector3(0.5f, 0.5f, 0.5f);
            //node2.ScaleBy(new Vector3(10, 10, 10));
            node.Yaw(180);
            node.Pitch(30);
            node.Roll(30);
            node.Position += new Vector3(0, 50, 0);
            node.Orientation = Quaternion.FromEulerAngles(0, 0, 0);

            Entity ent2 = _scene.CreateEntity("Penguin2", "penguin.mesh");
            //ent2.IsVisible = false;
            //ent2.Name;
            //ent2.ParentSceneNode;
            ent2.CastShadows = true;
            SceneNode node2 = _scene.RootSceneNode.CreateChildSceneNode("Penguin2", new Vector3(0, 0, 0));
            node2.AttachObject(ent2);
            node2.Position += new Vector3(-50, 25, -50);

            //create ground
            Plane plane = new Plane(Vector3.UnitY, 0);
            MeshManager.Instance.CreatePlane("ground", ResourceGroupManager.DefaultResourceGroupName, plane, 300, 300, 20, 20, true, 1, 5, 5, Vector3.UnitZ);
            Entity groundEnt = _scene.CreateEntity("GroundEntity", "ground");
            _scene.RootSceneNode.CreateChildSceneNode().AttachObject(groundEnt);
            groundEnt.CastShadows = false;
            groundEnt.MaterialName = "Rockwall";

            //create light 
            Light pointLight = _scene.CreateLight("pointLight");
            pointLight.Type = LightType.Point;
            pointLight.Position = new Vector3(0, 200, 0);
            pointLight.Diffuse = ColorEx.White;
            pointLight.Specular = ColorEx.White;


            _timer_frame.Interval = 30;
            _timer_frame.Tick += _timer_frame_Tick;
            _timer_frame.Start();
        }

        void _timer_frame_Tick(object sender, EventArgs e)
        {
            //Tell Axiom to render a single frame
            _engine.RenderOneFrame();

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.serialPort1.Close();
            Environment.Exit(0);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            axiom_init();


            sax.ChartType = SeriesChartType.Line;
            say.ChartType = SeriesChartType.Line;
            saz.ChartType = SeriesChartType.Line;

            ssx.ChartType = SeriesChartType.Line;
            ssy.ChartType = SeriesChartType.Line;
            ssz.ChartType = SeriesChartType.Line;

            spx.ChartType = SeriesChartType.Line;
            spy.ChartType = SeriesChartType.Line;
            spz.ChartType = SeriesChartType.Line;

            //chart_acc.ChartAreas[0].AxisY.Maximum = 3;
            //chart_acc.ChartAreas[0].AxisY.Minimum = -3;
            chart_acc.Series.Add(sax);
            chart_acc.Series.Add(say);
            chart_acc.Series.Add(saz);

            chart_speed.Series.Add(ssx);
            chart_speed.Series.Add(ssy);
            chart_speed.Series.Add(ssz);

            chart_position.Series.Add(spx);
            chart_position.Series.Add(spy);
            chart_position.Series.Add(spz);



            this.serialPort1.PortName = "COM3";

            try
            {
                this.serialPort1.Open();
            }
            catch (Exception ex) { 
            
            }
            //while (true) {
            //    Console.WriteLine(this.serialPort1.ReadExisting());
            //}
        }

        double[] gravity=new double[3];
        double[] linear_acceleration=new double[3];
        double[] linear_speed = new double[3];
        double[] linear_speed_filter = new double[3];
        double[] linear_position = new double[3];
        double[] linear_acc_deadzone_count = new double[3];

        int samples = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {

                samples++;
                
                this.serialPort1.WriteLine("g");
                this.serialPort1.ReadTimeout = 10;

                string[] aa = this.serialPort1.ReadExisting().Split('|');
                if (aa.Length >= 3)
                {
                    double ax = double.Parse(aa[0], CultureInfo.InvariantCulture);
                    double ay = double.Parse(aa[1], CultureInfo.InvariantCulture);
                    double az = double.Parse(aa[2], CultureInfo.InvariantCulture);
                    double at = Math.Sqrt(ax * ax + ay * ay + az * az);

                    // alpha is calculated as t / (t + dT)
                    // with t, the low-pass filter's time-constant
                    // and dT, the event delivery rate

                    double dt = 5;
                    double alpha = 1/(1+dt);
                    alpha = 0.8;

                    // Isolate the force of gravity with the low-pass filter.
                    gravity[0] = alpha * gravity[0] + (1 - alpha) * ax;
                    gravity[1] = alpha * gravity[1] + (1 - alpha) * ay;
                    gravity[2] = alpha * gravity[2] + (1 - alpha) * az;

                    // Remove the gravity contribution with the high-pass filter.
                    linear_acceleration[0] = ax - gravity[0];
                    linear_acceleration[1] = ay - gravity[1];
                    linear_acceleration[2] = az - gravity[2];

                    //linear accelaretion deadzone
                    double th=0.025;
                    if (linear_acceleration[0] > -th && linear_acceleration[0] < th) linear_acceleration[0] = 0;
                    if (linear_acceleration[1] > -th && linear_acceleration[1] < th) linear_acceleration[1] = 0;
                    if (linear_acceleration[2] > -th && linear_acceleration[2] < th) linear_acceleration[2] = 0;

                    //deadzone counter
                    if (linear_acceleration[0] == 0) { linear_acc_deadzone_count[0]++; } else { linear_acc_deadzone_count[0] = 0; }
                    if (linear_acceleration[1] == 0) { linear_acc_deadzone_count[1]++; } else { linear_acc_deadzone_count[1] = 0; }
                    if (linear_acceleration[2] == 0) { linear_acc_deadzone_count[2]++; } else { linear_acc_deadzone_count[2] = 0; }

                    if (samples > 30)
                    {
                        //integrate accel -> obtain speed
                        linear_speed[0] += linear_acceleration[0];
                        linear_speed[1] += linear_acceleration[1];
                        linear_speed[2] += linear_acceleration[2];

                        //clamp speed -> control integrate windup 
                        int speed_limit = 80;
                        if (linear_speed[0] < -speed_limit) linear_speed[0] = -speed_limit;
                        if (linear_speed[0] > speed_limit) linear_speed[0] = speed_limit;
                        if (linear_speed[1] < -speed_limit) linear_speed[1] = -speed_limit;
                        if (linear_speed[1] > speed_limit) linear_speed[1] = speed_limit;
                        if (linear_speed[2] < -speed_limit) linear_speed[2] = -speed_limit;
                        if (linear_speed[2] > speed_limit) linear_speed[2] = speed_limit;

                        //if not accel for long period -> assume no speed
                        int reset_speed = 10;
                        if (linear_acc_deadzone_count[0] > reset_speed) linear_speed[0] = 0;
                        if (linear_acc_deadzone_count[1] > reset_speed) linear_speed[1] = 0;
                        if (linear_acc_deadzone_count[2] > reset_speed) linear_speed[2] = 0;

                        //integrate speed -> obtain position
                        linear_position[0] += linear_speed[0];
                        linear_position[1] += linear_speed[1];
                        linear_position[2] += linear_speed[2];

                        //clamp position to 3d world box
                        double limit_x = 80;
                        if(linear_position[0]<-limit_x) linear_position[0] = -limit_x;
                        if (linear_position[0] > limit_x) linear_position[0] = limit_x;
                        if (linear_position[1] < -limit_x) linear_position[1] = -limit_x;
                        if (linear_position[1] > limit_x) linear_position[1] = limit_x;
                        if (linear_position[2] < 25) linear_position[2] = 25;
                        if (linear_position[2] > limit_x) linear_position[2] = limit_x;
                    }

                    while (sax.Points.Count > 100) sax.Points.RemoveAt(0); sax.Points.AddY(linear_acceleration[0]);
                    while (say.Points.Count > 100) say.Points.RemoveAt(0); say.Points.AddY(linear_acceleration[1]);
                    while (saz.Points.Count > 100) saz.Points.RemoveAt(0); saz.Points.AddY(linear_acceleration[2]);

                    while (ssx.Points.Count > 100) ssx.Points.RemoveAt(0); ssx.Points.AddY(linear_speed[0]);
                    while (ssy.Points.Count > 100) ssy.Points.RemoveAt(0); ssy.Points.AddY(linear_speed[1]);
                    while (ssz.Points.Count > 100) ssz.Points.RemoveAt(0); ssz.Points.AddY(linear_speed[2]);

                    while (spx.Points.Count > 100) spx.Points.RemoveAt(0); spx.Points.AddY(linear_position[0]);
                    while (spy.Points.Count > 100) spy.Points.RemoveAt(0); spy.Points.AddY(linear_position[1]);
                    while (spz.Points.Count > 100) spz.Points.RemoveAt(0); spz.Points.AddY(linear_position[2]);

                    node.Position = new Vector3(linear_position[1],linear_position[2],linear_position[0] );

                    chart_acc.ChartAreas[0].RecalculateAxesScale();
                    chart_speed.ChartAreas[0].RecalculateAxesScale();
                    chart_position.ChartAreas[0].RecalculateAxesScale();

                    //while (sat.Points.Count > 100) sat.Points.RemoveAt(0);
                    //sat.Points.AddY(at);



                }
            }
            catch (Exception ex) { 
            
            }

        }


        int prevRotation = 0;
        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            //Rotate the ogre mesh
            node.Rotate(Axiom.Math.Vector3.UnitY, trackBar1.Value - prevRotation);
            prevRotation = trackBar1.Value;
        }



    }
}
