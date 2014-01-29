using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Axiom.Core;
using Axiom.Graphics;
using Axiom.Math;
using Axiom.Input;
using murix_utils;

namespace AxiomDX9Game2
{
    internal class Game
    {
        private Root _engine;
        private RenderWindow _window;
        private SceneManager _scene;
        private Camera _camera;



        public void OnLoad()
        {
            ResourceGroupManager.Instance.AddResourceLocation("media", "Folder", true);

            _scene = _engine.CreateSceneManager("DefaultSceneManager", "DefaultSM");
            _scene.ClearScene();

            //_scene = Root.Instance.CreateSceneManager(SceneType.ExteriorClose);
            //_scene.LoadWorldGeometry("Terrain.xml");


            _camera = _scene.CreateCamera("MainCamera");

            //
            _camera.Position = new Vector3(0, 170, 300); // x=right/left from screen,y=bottom/up from screen,z=in/out from scren
            _camera.LookAt(Vector3.Zero);

            //The clipping distance of a Camera specifies how close or far something can be before you no longer see it. 
            _camera.Near = 5;


            _camera.AutoAspectRatio = true;

            var vp = _window.AddViewport(_camera, 0, 0, 1.0f, 1.0f, 100);
            vp.BackgroundColor = ColorEx.CornflowerBlue;

            ResourceGroupManager.Instance.InitializeAllResourceGroups();

        }

        SceneNode node = null;

        public void CreateScene()
        {

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

        }


        public void OnUnload()
        {
        }


        void HandleKeyboardInput(FrameEventArgs e) {
            //InputReader input = new InputReader();
            //input.Capture();
        }

     
        public void OnRenderFrame(object s, FrameEventArgs e)
        {
            Gamepad_State_SlimDX joy = new Gamepad_State_SlimDX(SlimDX.XInput.UserIndex.One);
            joy.Update();

            node.Pitch(joy.LeftStick.Position.Y);
            node.Roll(joy.LeftStick.Position.X);
            node.Yaw(joy.RightStick.Position.X);


            node.Position += new Vector3(0, joy.RightStick.Position.Y, 0);

            if (node.Position.y < 25)
            {
                node.Position = new Vector3(node.Position.x, 25, node.Position.z);
            }

            HandleKeyboardInput(e);

        }

        public void Run()
        {
            //Root root = new Root("Game.log");
            //root.RenderSystem = root.RenderSystems["DirectX9"];
            //root.RenderSystem.ConfigOptions["Video Mode"].Value = "1280 x 720 @ 32-bit color";


            _engine = new Root("Game.log");

            _engine.RenderSystem = _engine.RenderSystems[0];
            //_engine.RenderSystem = _engine.RenderSystems["DirectX9"];

            _window = _engine.Initialize(true);
            
                OnLoad();
                CreateScene();
                _engine.FrameRenderingQueued += OnRenderFrame;
                _engine.StartRendering();
                OnUnload();
            

        }
    }
}
