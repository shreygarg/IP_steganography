make: encode.cpp decode.cpp
	g++ -g -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o encode  encode.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lz
	g++ -g -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o decode  decode.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lz
	g++ -g -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o steganography  steganography.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lz

