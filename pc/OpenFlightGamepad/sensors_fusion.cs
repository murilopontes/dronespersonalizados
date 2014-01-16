using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace murix_utils
{

    class sensors_fusion
    {

        //------------------- CALIBRATION DATA ------------------------
        private readonly static double height_in_ground = 900f; //most high frequency - drone fix in ground (from 10k samples)
        private readonly static double height_min_valid_cm = 24f; //from arfreelight
        private readonly static double height_gain = height_in_ground / height_min_valid_cm;
        private readonly static double height_max_valid_cm = 400f * height_gain; //from ultrasonic de facto range
        //
        public readonly static double ax_offset = 2068f; //most high frequency - drone fix in ground (from 10k samples)
        public readonly static double ay_offset = 1976f; //most high frequency - drone fix in ground (from 10k samples)
        public readonly static double az_offset = 2052f; //most high frequency - drone fix in wall (from 10k samples)
        //
        public readonly static double az_in_ground = 3028f; //most high frequency - drone fix in ground (from 10k samples)
        public readonly static double earth_gravity = 9.80665f; //from wikipedia
        public readonly static double accel_gain_z = az_in_ground / earth_gravity;
        //
        public readonly static double gx_offset = 1664f;
        public readonly static double gy_offset = 1669f;
        public readonly static double gz_offset = 1663f;
        //
        public readonly static double rad2degree = 180f / Math.PI;

        //--------------------- CURRENT ESTIMATIVE
        //
        private static double height = 0f;
        //
        public static double ax = 0f;
        public static double ay = 0f;
        public static double az = 0f;
        //
        public static double accel_pitch_rad = 0f;
        public static double accel_roll_rad = 0f;
        //
        public static double accel_pitch_deg = 0f;
        public static double accel_roll_deg = 0f;
        //
        public static double gx = 0f;
        public static double gy = 0f;
        public static double gz = 0f;
        //
        public static double giro_pitch_rad = 0f;
        public static double giro_roll_rad = 0f;
        public static double giro_yaw_rad = 0f;
        //
        public static double giro_pitch_deg = 0f;
        public static double giro_roll_deg = 0f;
        public static double giro_yaw_deg = 0f;


        

        public static void Update(double dt_s, double ax, double ay, double az, double gx, double gy, double gz,double echo) {
            //ok, validated with program.elf/ar-freeflight (minimal detectable height is 24 cm)
            if (echo <= height_max_valid_cm)
            {
                height = (echo / height_gain);
            }
            //Console.WriteLine(height);

            //accelerometer
            ax = (ax - ax_offset) /100f;
            ay = (ay - ay_offset) /100f;
            az = (az - az_offset) /100f;
            //Console.WriteLine(ax + "|" + ay + "|" + az);
            //
            accel_pitch_rad = -Math.Atan2(ax, az);
            accel_roll_rad = Math.Atan2(ay, az);
            //
            accel_pitch_deg = accel_pitch_rad * rad2degree;
            accel_roll_deg = accel_roll_rad * rad2degree;
            Console.WriteLine(accel_pitch_deg + "|" + accel_roll_deg);


            //giroscope
            gx = (gx - gx_offset) ;
            gy = (gy - gy_offset) ;
            gz = (gz - gz_offset) ;
            //
            giro_pitch_rad += gx * dt_s ;
            giro_roll_rad += gy * dt_s ;
            giro_yaw_rad += gz * dt_s ;
            //
            giro_pitch_deg = giro_pitch_rad * rad2degree;
            giro_roll_deg = giro_roll_rad * rad2degree;
            giro_yaw_deg = giro_yaw_rad * rad2degree;
            //Console.WriteLine(accel_pitch_deg + "|" + giro_pitch_deg);



            //Console.WriteLine("dt="+dt_s+"|"+ax + "|" + ay + "|" + az + "|" + giro_pitch_deg + "|" + giro_roll_deg + "|" + giro_yaw_deg + "|" + height);


        }



       public static double avg(double[] x)
        {
            double sum = 0;
            foreach (int i in x) sum += i;
            return sum / x.Length;
        }
       public static double stddev(double[] x)
        {
            double e = avg(x);
            double sum = 0;
            foreach (int i in x) sum += ((i - e) * (i - e));
            return Math.Sqrt(sum/x.Length);
        }

       public static void histrogram(double[] items)
       {
           Dictionary<double, double> histogram = new Dictionary<double, double>();
           foreach (double item in items)
           {
               if (histogram.ContainsKey(item))
               {
                   histogram[item]++;
               }
               else
               {
                   histogram[item] = 1;
               }
           }

           var sortedDict = from entry in histogram orderby entry.Value descending select entry;

           foreach (KeyValuePair<double, double> pair in sortedDict)
           {
               double per = (pair.Value / items.Length) * 100.0;
               if (per > 5)
               {
                   Console.WriteLine("{0} occurred {1}%", pair.Key, per);
               }
           }
       }
    }
}
