


echo "DEPLOY------start"
..\..\..\pc\DroneTelnetCmd\bin\Debug\DroneTelnetCmd.exe "killall -9 drone ; exit"
ftp -s:..\ftp_drone.txt
..\..\..\pc\DroneTelnetCmd\bin\Debug\DroneTelnetCmd.exe "chmod +x /data/video/drone ; exit"
echo "DEPLOY------end"
