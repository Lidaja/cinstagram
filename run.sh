g++ -std=c++11 cinstacam.cpp `pkg-config --libs --cflags opencv` -o cinstacam
./cinstacam
rm cinstacam
