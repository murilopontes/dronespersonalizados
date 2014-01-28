using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Axiom.Core;
using Axiom.Graphics;
using Axiom.Math;

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

            _camera = _scene.CreateCamera("MainCamera");

            //
            _camera.Position = new Vector3(0, 300, 300); // x=right/left from screen,y=bottom/up from screen,z=in/out from scren
            _camera.LookAt(Vector3.Zero);

            //The clipping distance of a Camera specifies how close or far something can be before you no longer see it. 
            _camera.Near = 5;


            _camera.AutoAspectRatio = true;

            var vp = _window.AddViewport(_camera, 0, 0, 1.0f, 1.0f, 100);
            vp.BackgroundColor = ColorEx.CornflowerBlue;

            ResourceGroupManager.Instance.InitializeAllResourceGroups();

        }

        public void CreateScene()
        {

            //shadows and ambient light
            _scene.AmbientLight = ColorEx.White;
           // _scene.ShadowTechnique = ShadowTechnique.StencilAdditive;


            Entity ent = _scene.CreateEntity("Penguin", "penguin.mesh");
            SceneNode node = _scene.RootSceneNode.CreateChildSceneNode("PenguinNode");
            node.AttachObject(ent);
            
            Entity ent2 = _scene.CreateEntity("Penguin2", "penguin.mesh");
            //ent2.IsVisible = false;
            //ent2.Name;
            //ent2.ParentSceneNode;
            ent2.CastShadows = true;
            SceneNode node2 = _scene.RootSceneNode.CreateChildSceneNode("Penguin2", new Vector3(100, 0, 0));
            node2.AttachObject(ent2);

            //
            node2.Position += new Vector3(10, 0, 10);
            node.Position += new Vector3(25, 0, 0);
            node.Scale = new Vector3(0.5f, 0.5f, 0.5f);
            //node2.ScaleBy(new Vector3(10, 10, 10));
            node.Yaw(180);
            node.Pitch(30);
            node.Roll(30);
            node.Position += new Vector3(0, 50, 0);

        }


        public void OnUnload()
        {
        }

        public void OnRenderFrame(object s, FrameEventArgs e)
        {
        }

        public void Run()
        {
            using (_engine = new Root())
            {
                _engine.RenderSystem = _engine.RenderSystems[0];
                using (_window = _engine.Initialize(true))
                {
                    OnLoad();
                    CreateScene();
                    _engine.FrameRenderingQueued += OnRenderFrame;
                    _engine.StartRendering();
                    OnUnload();
                }
            }
        }
    }
}
