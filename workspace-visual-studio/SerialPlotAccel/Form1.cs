using Axiom.Core;
using Axiom.Graphics;
using Axiom.Math;
using murix_utils;
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

        //mpu6050 temperature
        series_murix s_temp = new series_murix("temp", 100);
        //tracking interval time
        series_murix s_track_dt = new series_murix("track_dt", 100);
        //accel
        series_murix s_ax = new series_murix("ax",100);
        series_murix s_ay = new series_murix("ay", 100);
        series_murix s_az = new series_murix("az", 100);
        //accel integrate -> speed
        series_murix s_sx = new series_murix("sx", 100);
        series_murix s_sy = new series_murix("sy", 100);
        series_murix s_sz = new series_murix("sz", 100);
        //accel integrate2 -> position 
        series_murix s_px = new series_murix("track_px", 100);
        series_murix s_py = new series_murix("track_py", 100);
        series_murix s_pz = new series_murix("track_pz", 100);
        //accel gravity
        series_murix s_gravity_x = new series_murix("gravity_x", 100);
        series_murix s_gravity_y = new series_murix("gravity_y", 100);
        series_murix s_gravity_z = new series_murix("gravity_z", 100);
        //accel pitch and roll
        series_murix s_track_acc_pitch = new series_murix("track_acc_pitch", 100);
        series_murix s_track_acc_roll = new series_murix("track_acc_roll", 100);
        //gyro
        series_murix s_gx = new series_murix("gx", 100);
        series_murix s_gy = new series_murix("gy", 100);
        series_murix s_gz = new series_murix("gz", 100);
        //gyro integrate -> pitch, roll, yaw
        series_murix s_track_gxi = new series_murix("track_gyro_pitch", 100);
        series_murix s_track_gyi = new series_murix("track_gyro_roll", 100);
        series_murix s_track_gzi = new series_murix("track_gyro_yaw", 100);
        //fusion pitch, roll
        series_murix s_track_fusion_pitch = new series_murix("track_fusion_pitch", 100);
        series_murix s_track_fusion_roll = new series_murix("track_fusion_roll", 100);



        public class drone_3d
        {
            //
            private Root _engine;
            private RenderWindow _window;
            private SceneManager _scene;
            private Camera _camera;
            private System.Windows.Forms.Timer _timer_frame = new System.Windows.Forms.Timer();
            public SceneNode node = null;
            //
            public void axiom_init(PictureBox pictureBox1)
            {


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

                /*
                Entity ent2 = _scene.CreateEntity("Penguin2", "penguin.mesh");
                //ent2.IsVisible = false;
                //ent2.Name;
                //ent2.ParentSceneNode;
                ent2.CastShadows = true;
                SceneNode node2 = _scene.RootSceneNode.CreateChildSceneNode("Penguin2", new Vector3(0, 0, 0));
                node2.AttachObject(ent2);
                node2.Position += new Vector3(-50, 25, -50);
                 */

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


        }

        public class series_murix : Series {
            int max;
            public series_murix(string name,int max):base(name) {
                this.ChartType = SeriesChartType.Line;
                this.max = max;
            }
            public void addy(double y)
            {
                while (base.Points.Count >= max) {
                    base.Points.RemoveAt(0);                    
                }
                base.Points.AddY(y);
            }
        }




        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.serialPort1.Close();
            Environment.Exit(0);
        }

        drone_3d drone3d = new drone_3d();

        private void Form1_Load(object sender, EventArgs e)
        {
            drone3d.axiom_init(this.pictureBox1);

            chart_acc.Series.Add(s_ax);
            chart_acc.Series.Add(s_ay);
            chart_acc.Series.Add(s_az);

            chart_acc_speed.Series.Add(s_sx);
            chart_acc_speed.Series.Add(s_sy);
            chart_acc_speed.Series.Add(s_sz);

            chart_acc_position.Series.Add(s_px);
            chart_acc_position.Series.Add(s_py);
            chart_acc_position.Series.Add(s_pz);



            this.serialPort1.PortName = "COM4";
            this.serialPort1.BaudRate = 115200;

 
     
        }


        int samples = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
                if (!this.serialPort1.IsOpen)
                {
                    this.serialPort1.Open();
                }


                Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(SlimDX.XInput.UserIndex.One);
                joy.Update();

                string cmd = "pilot(";
                cmd += (int)( joy.RightStick.Position.Y * 254);
                cmd += ",";
                cmd += (int)( joy.LeftStick.Position.X * 254);
                cmd += ",";
                cmd += (int)( joy.LeftStick.Position.Y * 254);
                cmd += ",";
                cmd += (int)( joy.RightStick.Position.X * 254);
                cmd += ",";

                if (joy.A)
                {
                    cmd += "1";
                }
                else {
                    cmd += "0";
                }
                
                cmd += ",";

                if (joy.B)
                {
                    cmd += "1";
                }
                else
                {
                    cmd += "0";
                }

                cmd += ")\r\n";

                Console.WriteLine(cmd);
                this.serialPort1.WriteLine(cmd);
                return;






       
                samples++;
                
                this.serialPort1.WriteLine("imu");
                this.serialPort1.ReadTimeout = 10;
                string txt = this.serialPort1.ReadExisting();
                txt = txt.Replace("<","");
                txt = txt.Replace(">", "");
                txt = txt.Replace("imu", "");
                txt = txt.Replace("\r\n", "");
                Console.WriteLine(txt);

                string[] aa = txt.Split('|');
                if (aa.Length >= 24)
                {
                    //temperature
                    double temp = double.Parse(aa[0], CultureInfo.InvariantCulture);
                    //interval
                    double dt = double.Parse(aa[1], CultureInfo.InvariantCulture);
                    //accel
                    double ax = double.Parse(aa[2], CultureInfo.InvariantCulture);
                    double ay = double.Parse(aa[3], CultureInfo.InvariantCulture);
                    double az = double.Parse(aa[4], CultureInfo.InvariantCulture);
                    //accel integrate -> speed
                    double sx = double.Parse(aa[5], CultureInfo.InvariantCulture);
                    double sy = double.Parse(aa[6], CultureInfo.InvariantCulture);
                    double sz = double.Parse(aa[7], CultureInfo.InvariantCulture);
                    //accel integrate2 -> position
                    double px = double.Parse(aa[8], CultureInfo.InvariantCulture);
                    double py = double.Parse(aa[9], CultureInfo.InvariantCulture);
                    double pz = double.Parse(aa[10], CultureInfo.InvariantCulture);
                    //accel gravity
                    double grav_x = double.Parse(aa[11], CultureInfo.InvariantCulture);
                    double grav_y = double.Parse(aa[12], CultureInfo.InvariantCulture);
                    double grav_z = double.Parse(aa[13], CultureInfo.InvariantCulture);
                    //accel pitch roll
                    double a_pitch = double.Parse(aa[14], CultureInfo.InvariantCulture);
                    double a_roll = double.Parse(aa[15], CultureInfo.InvariantCulture);
                    //gyro
                    double gx = double.Parse(aa[16], CultureInfo.InvariantCulture);
                    double gy = double.Parse(aa[17], CultureInfo.InvariantCulture);
                    double gz = double.Parse(aa[18], CultureInfo.InvariantCulture);
                    //gyro integrate
                    double gxi = double.Parse(aa[19], CultureInfo.InvariantCulture);
                    double gyi = double.Parse(aa[20], CultureInfo.InvariantCulture);
                    double gzi = double.Parse(aa[21], CultureInfo.InvariantCulture);
                    //fusion pitch roll
                    double fusion_pitch = double.Parse(aa[22], CultureInfo.InvariantCulture);
                    double fusion_roll = double.Parse(aa[23], CultureInfo.InvariantCulture);

                    //
                    s_ax.addy(a_pitch);
                    s_ay.addy(a_roll);
                    //
                    s_sx.addy(gxi);
                    s_sy.addy(gyi);
                    s_sz.addy(gzi);
                    //
                    s_px.addy(fusion_pitch);
                    s_py.addy(fusion_roll);


                    //
                    if (pz < 0) pz = 0;
                   // drone3d.node.Position = new Vector3(px, py, pz);
                    drone3d.node.Orientation = Quaternion.FromEulerAnglesInDegrees(fusion_pitch, 0, -fusion_roll); 

                    chart_acc.ChartAreas[0].RecalculateAxesScale();
                    chart_acc_speed.ChartAreas[0].RecalculateAxesScale();
                    chart_acc_position.ChartAreas[0].RecalculateAxesScale();


                }
            }
            catch (Exception ex) { 
            
            }

        }


    


    }
}
