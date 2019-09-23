g++ -ggdb main.cpp loadParticle.cpp particleDescriptors.cpp -o main `pkg-config --cflags --libs /usr/lib/pkgconfig/opencv4.pc`
./main
