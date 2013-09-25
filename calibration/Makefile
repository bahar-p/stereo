CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: calib stereo_calib
calib: camera_calibration.o
	$(CC) camera_calibration.o -o $(BUILDPATH)/calib $(OPENCVLIB)
camera_calibration.o: camera_calibration.cpp
	$(CC) $(CFLAGS) $(OPENCVINC) camera_calibration.cpp
stereo_calib: stereo_calib.o
	$(CC) stereo_calib.o -o $(BUILDPATH)/stereo_calib $(OPENCVLIB)
stereo_calib.o: stereo_calib.cpp
	$(CC) $(CFLAGS) $(OPENCVINC) stereo_calib.cpp

clean:
	rm -rf *.o *~
