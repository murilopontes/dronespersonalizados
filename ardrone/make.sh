rm -fv bin/*

cd motorboard
./make.sh
cd ..

cd navboard
./make.sh
cd ..

cd attitude
./make.sh
cd ..

cd vbat
./make.sh
cd ..

cd gpio
./make.sh
cd ..

cd video
./make.sh
cd ..

cd fly
./make.sh
cd ..