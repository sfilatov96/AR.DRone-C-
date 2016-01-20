#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#include <QApplication>


#define PORT 5556 //port that listens for AT commands
#define LED_FREQUENCY 1073741824
#define AMP_p_0_05 1028443341
#define AMP_p_0_1 1036831949
#define AMP_p_0_2 1045220557
#define AMP_p_0_5 1056964608
#define AMP_n_0_05 -1119040307
#define AMP_n_0_1 -1110651699
#define AMP_n_0_2 -1102263091
#define AMP_n_0_5 -1090519040
#define AMP_p_1 1065353216
#define AMP_n_0_7 -1086324736
#define AMP_p_0_7  1061158912
#define AMP_n_1 -1082130432
const int step = 15;
class drone{
public:
	int i;
	struct sockaddr_in receiver_addr;
	int sock_fd;
	char droneBuf[50];
	drone() {
			//struct sockaddr_in receiver_addr;
			i = 1;
			sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			receiver_addr.sin_family = AF_INET;
			if( inet_aton( "192.168.1.1",  &receiver_addr.sin_addr )== 0) {
				printf("Couldn't establish connection\n");
				close(sock_fd);
				return;
			}
			receiver_addr.sin_port = htons( PORT );
		}
	void led(int regime,int delay) {
		sprintf(droneBuf,"AT*LED=%u,%u,%u,%u\r",i++,regime,LED_FREQUENCY,delay);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //sleep(delay);
	}
	void anim(int delay) {
		sprintf(droneBuf,"AT*CONFIG=%u,\"control:flight_anim\",\"%u,%u\" \r",i++,3,1000);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //sleep(delay);
	}
	void droneTakeOff() {
		sprintf(droneBuf,"AT*REF=%u,290718208\r",i++);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
				std::cout << "away from land" << std::endl;
        sleep(2);
        QApplication::processEvents();
        sleep(2);
        QApplication::processEvents();
        sleep(2);
        QApplication::processEvents();
        sleep(1);
        QApplication::processEvents();
		
	}
	void droneLand() {
		sprintf(droneBuf,"AT*REF=%u,290717696\r",i++);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
		std::cout << "landed" << std::endl;
        //sleep(2);
        QApplication::processEvents();

	}
	/*void droneMove(void) {
		sprintf(droneBuf, MAX_COMMAND_LEN,"AT*PCMD=%u,1,%d,%d,%d,%d",droneSeq,pitch,roll,gaz,yaw);
	}*/
	void droneHover() {
		sprintf(droneBuf, "AT*PCMD=%u,1,0,0,0,0\r", i++);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //usleep(250000);
	}
	void droneUp() {
		sprintf(droneBuf, "AT*PCMD=%u,1,0,0,%u,0\r" ,i++,AMP_p_0_5);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //usleep(250000);
	}
	void droneDown() {
		sprintf(droneBuf,"AT*PCMD=%u,1,0,0,%u,0\r", i++,AMP_n_0_5);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //usleep(250000);
	}
	int droneForward(float repeat) {
		int n = ceil(repeat);
		if (repeat < 30) {
			n /= 10;
		}
		else n /= 25; 
		if (n == 0) n = 1; 
		for (int k = 0; k != n ; k++) {
			sprintf(droneBuf,"AT*PCMD=%u,1,0,%u,0,0\r", i++,AMP_n_1);
            sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
           usleep(250000);
           QApplication::processEvents();

		}
		return n;
        sleep(2);
        QApplication::processEvents();
		
	}
	void droneBack() {
		sprintf(droneBuf,"AT*PCMD=%u,1,0,%u,0,0\r",i++,AMP_p_0_5);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //usleep(250000);
	}
	void droneLeft() {
		sprintf(droneBuf,"AT*PCMD=%u,1,%u,0,0,0\r",i++,AMP_n_0_5);
		sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //usleep(250000);
	}
	void droneRight() {

		sprintf(droneBuf, "AT*PCMD=%u,1,%u,0,0,0\r", i++,AMP_p_0_5);
        sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        //usleep(250000);
	}
	void droneRotateLeft(int angle) {
		int repeat = angle/step;
		for(int k = 0; k != repeat; k++) {
			sprintf(droneBuf, "AT*PCMD=%u,1,0,0,0,%u\r", i++,AMP_n_0_5);
            sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
            usleep(250000);
            QApplication::processEvents();
		}
        sleep(2);
        QApplication::processEvents();
	}
	void droneRotateRight(int angle) {
		int repeat = angle/step;
		for(int k = 0; k != repeat; k++) {
			sprintf(droneBuf,"AT*PCMD=%u,1,0,0,0,%u\r", i++,AMP_p_0_5);
            sendto(sock_fd, droneBuf, 50, 0,(struct sockaddr*)&receiver_addr, sizeof(receiver_addr));
        usleep(250000);
		}
        sleep(2);
        QApplication::processEvents();

	}

};


	
