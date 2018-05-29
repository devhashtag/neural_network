#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <ctime>

#include <termios.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include "Matrix.h"
#include "NeuralNet.h"

using namespace std;

/* 
	open serial port
	use write(2) and read(2) to write and read, and close() to close the connection
*/
int openport(char * name) {
	// opening port
	int fd = open(name, O_RDWR | O_NOCTTY);

	// struct for settings
	struct termios toptions;

	// get the current settings
	tcgetattr(fd, &toptions);

	// set input/output baud rate
	cfsetispeed(&toptions, B9600);
	cfsetospeed(&toptions, B9600);
 
	// set custom options
	toptions.c_cflag &= ~PARENB;
	toptions.c_cflag &= ~CSTOPB;
	toptions.c_cflag &= ~CSIZE;
	toptions.c_cflag |= CS8;
	toptions.c_cflag &= ~CRTSCTS;
	toptions.c_cflag |= CREAD | CLOCAL;
	toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
	toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	toptions.c_oflag &= ~OPOST;

	// update the settings
	tcsetattr(fd, TCSANOW, &toptions);

	// flush the data that is never read/transmitted
	tcflush(fd, TCIFLUSH);

	// prevent blocking read calls
	fcntl(fd, F_SETFL, FNDELAY);

	return fd;
}

int main() {
	srand(5);

	// create neural network from file
	NeuralNet nn = NeuralNet("wall-detect.net");


	// open serial connection
	int fd = openport("/dev/rfcomm0");
	if (fd < 0)
		cout << "Error errno: " << errno << endl;

	// int n is used to check for errors when writing and reading from serial port
	int n = 0;

	// dist is used to store the distance value returned by arduino
	unsigned char dist; 

	// input is used as input for the neural network. Its values is dist in a range from 0 to 1
	Matrix input = Matrix(1, 1);


	// variable used for the while loop
	int running = 1;

	// turn the motors on
	n = write(fd, "1", 1);
	if (n < 0)
		cout << "write(2) failed!" << endl << "errno: " << errno << endl;
	else
		cout << "Status: Driving" << endl;



	while (running) {
		// give command to measure distance
		n = write(fd, "2", 1);
		if (n < 0)
			cout << "write(2) failed!" << endl << "errno: " << errno << endl;

		// wait some time until the distance is measured
		usleep(1000 * 100);
		
		// read the distance from the serial port
		n = read(fd, &dist, sizeof(dist));
		if (n < 0)
			cout << "read(2) failed!" << endl << "errno: " << errno << endl;
		else {
			// if it didn't fail

			// since 255 is the max value for a char, input will be between 0 and 1
			input(0) = dist / 255.0;

			// get the output of the matrix (prediction)
			Matrix output = nn.predict(input);

			// write input and output to screen
			cout << "input: " << input(0) << "\toutput: " << output(0) << endl;

			// if the output is closer to 0 than 1, stop the motor
			if (output(0) < 0.5) {
				running = 0;
				n = write(fd, "0", 1);
				if (n < 0)
					cout << "write(2) failed!" << endl << "errno: " << errno << endl;
			}
		}
	}
	

	// close the serial connection
	close(fd);
	return 0;
}