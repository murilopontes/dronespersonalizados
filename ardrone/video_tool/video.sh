#!/bin/sh

while [ 1 ]; do
/data/video/video_tool -d /dev/video0 -c 1
/data/video/udp_broadcast_file 7070 /tmp/horizontal.yuv
/data/video/video_tool -d /dev/video1 -c 1
/data/video/udp_broadcast_file 6969 /tmp/vertical.yuv
done


