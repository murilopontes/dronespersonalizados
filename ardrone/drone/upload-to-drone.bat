echo "start"


..\..\..\pc\DroneTelnetCmd\bin\Debug\DroneTelnetCmd.exe "killall -9 inetd ; exit"
..\..\..\pc\DroneTelnetCmd\bin\Debug\DroneTelnetCmd.exe "echo 21 stream tcp nowait root ftpd ftpd -w /tmp > /tmp/inetd.conf  ; exit"
..\..\..\pc\DroneTelnetCmd\bin\Debug\DroneTelnetCmd.exe "inetd /tmp/inetd.conf ; sleep 1; exit"


..\..\..\pc\DroneTelnetCmd\bin\Debug\DroneTelnetCmd.exe "killall -9 drone ; exit"
ftp -s:..\ftp_drone.txt
..\..\..\pc\DroneTelnetCmd\bin\Debug\DroneTelnetCmd.exe "chmod +x /tmp/drone ; exit"


echo "end"
