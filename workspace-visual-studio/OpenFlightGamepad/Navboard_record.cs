using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace murix_utils
{
    class Navboard_record
    {
        public UInt16 size { get; set; }
        public UInt16 sequence { get; set; }
        public UInt16 ax { get; set; }
        public UInt16 ay { get; set; }
        public UInt16 az { get; set; }
        public UInt16 gx { get; set; }
        public UInt16 gy { get; set; }
        public UInt16 gz { get; set; }
        public UInt16 gx2 { get; set; }
        public UInt16 gy2 { get; set; }
        public UInt16 acc_temp { get; set; }
        public UInt16 gyro_temp { get; set; }
        public UInt16 vrefEpson { get; set; }
        public UInt16 vrefIDG500 { get; set; }
        public UInt16 us_echo { get; set; }
        public UInt16 checksum { get; set; }
        public UInt16 us_echo_start { get; set; }
        public UInt16 us_echo_end { get; set; }
        public UInt16 us_association_echo { get; set; }
        public UInt16 us_distance_echo { get; set; }
        public UInt16 us_courbe_temps { get; set; }
        public UInt16 us_courbe_valeur { get; set; }
        public UInt16 us_courbe_ref { get; set; }
        public UInt16 uk1 { get; set; }
        public UInt16 uk2 { get; set; }
        public UInt16 uk3 { get; set; }
    }
}
