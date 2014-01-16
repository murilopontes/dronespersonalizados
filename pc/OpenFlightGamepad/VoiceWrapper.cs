using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Speech.Synthesis;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace murix_utils
{
    class Voice_Alarm
    {
        private static SpeechSynthesizer synth = new SpeechSynthesizer();
        private static BlockingCollection<string> voice_queue = new BlockingCollection<string>(1);

        static Thread fd;

        public static void Speak(string text){
            voice_queue.TryAdd(text);
        }

        private static void voice_work() {
            while (true) {
                synth.Speak(voice_queue.Take());
            }
        }

        public static void Start() {
            fd = new Thread(voice_work);
            fd.Start();
        }

        public static void Stop() {
            fd.Abort();
        }


    }
}
