


echo "DEPLOY------start"
..\..\..\workspace-visual-studio\Binaries\DroneTelnetCmd.exe "killall -9 OpenUAV-ardrone ; exit"
ftp -s:..\ftp_drone.txt
..\..\..\workspace-visual-studio\Binaries\DroneTelnetCmd.exe "chmod +x /data/video/OpenUAV-ardrone ; exit"
echo "DEPLOY------end"
