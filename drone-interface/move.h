#include <iostream>
#include <stdio.h>
#include "libdrone.h"
#include <paintscene.h>
#include <fstream>
#define pi 3.1415926535897932384626433832

typedef int data_t;

int COUNT = 2;

int abs(int  value) {

	if (value >= 0) {
		return value;
	}
	else {
		return (-1) * value; 
	}
}

int turn(int curent_drone_angle, int need_drone_angle, drone Ardrone) {      //выравнивание дрона перед началом движения
	//std::cout << "need: " << need_drone_angle << endl;
	//std::cout << "current: " << curent_drone_angle << endl;

	if (curent_drone_angle == need_drone_angle) {
        std::ofstream fout("cout.txt", std::ios_base::app);
        fout << COUNT++ << " not turn ";
        fout.close();
		return curent_drone_angle;
	}

	int delta_angle = need_drone_angle - curent_drone_angle;
	//cout << "дельта: "<< delta_angle << endl;

	if ((delta_angle > 0) && (delta_angle <= 180) ) {
		for (int pos = 15; pos <= 180; pos += 15) {
			if (delta_angle == pos) {
                std::ofstream fout ("cout.txt", std::ios_base::app);
                fout << COUNT++ << ". q right" << pos << ' ';
                fout.close();
				Ardrone.droneRotateRight(pos);
			
			}
		}

	} else if ((delta_angle > 0) && (delta_angle > 180) ) {
		for (int pos = 180; pos < 360; pos += 15) {
			if (delta_angle == pos) { 
                std::ofstream fout("cout.txt", std::ios_base::app);
                fout << COUNT++ << " w left " << 360 - abs(pos);
                fout.close();
				Ardrone.droneRotateLeft(360 - abs(pos));
			}	
				
		}
		
	} else if ((delta_angle < 0) && (delta_angle >= -180) ) {
		for (int pos = 0; pos >= -180; pos -= 15) {
			if (delta_angle == pos) {
                std::ofstream fout("cout.txt", std::ios_base::app);
                fout << COUNT++ << " e left " << abs(pos);
                fout.close();
				Ardrone.droneRotateLeft(abs(pos));
			}	
			
		}
		
	} else if ((delta_angle < 0) && (delta_angle < -180) ) {
		for (int pos = -180; pos > -360; pos -= 15) {
			if (delta_angle == pos) { 
                std::ofstream fout("cout.txt", std::ios_base::app);
                fout << COUNT++ << " d right " << 360 - abs(pos);
                fout.close();
				Ardrone.droneRotateRight(360 - abs(pos));
			}	
				
		}
	}	
	return need_drone_angle;		 
}

int Find_drone_angle(int delta_x, int delta_y) {

	double angle_trajectory = asin(abs(delta_x) / sqrt(delta_y * delta_y + delta_x * delta_x)); //!!!!!delta_x
	angle_trajectory = 180 * angle_trajectory / pi;   //перевод из радиан
	int integer_part = angle_trajectory / 15;
    if (integer_part < 1)
        return 0;
    else
        return (integer_part + 1) * 15;
}

void Movement(point_t* arr, int size, drone Ardrone, paint *w) {
	                                                          //перед началом движения "морда" дрона смотрит на увеличение координат оси х
	point_t prev_point;
	prev_point.x = arr[0].x;
	prev_point.y = arr[0].y;

	point_t current_point;
	current_point.x = arr[0].x;
	current_point.y = arr[0].y;
	int curent_drone_angle = 0;
	
	for (int pos = 1; pos < size; pos++) {

		current_point.x = arr[pos].x;
		current_point.y = arr[pos].y;

		data_t delta_x = current_point.x - prev_point.x;
		data_t delta_y = current_point.y - prev_point.y;

		if ( (delta_x == 0) && (delta_y > 0) ) {
			curent_drone_angle = turn(curent_drone_angle, 0, Ardrone);
			Ardrone.droneForward(delta_y);

            std::ofstream fout ("cout.txt", std::ios_base::app);
            fout<<" forward " << delta_y << std::endl;
            fout.close();
			
		} else if ( (delta_x == 0) && (delta_y < 0) ) {
			curent_drone_angle = turn(curent_drone_angle, 180, Ardrone);                                         
			Ardrone.droneForward(abs(delta_y)); 

            std::ofstream fout("cout.txt", std::ios_base::app);
            fout<<" forward " << abs(delta_y) << std::endl;
            fout.close();

		} else if ( (delta_x > 0) && (delta_y == 0) ) {
			curent_drone_angle = turn(curent_drone_angle, 90, Ardrone);
			Ardrone.droneForward(delta_x);

            std::ofstream fout("cout.txt", std::ios_base::app);
            fout<<" forward " << delta_x << std::endl;
            fout.close();
			
		} else if ( (delta_x < 0) && (delta_y == 0) ) {
			curent_drone_angle = turn(curent_drone_angle, 270, Ardrone); 
			Ardrone.droneForward(abs(delta_x));

            std::ofstream fout("cout.txt", std::ios_base::app);
            fout<<" forward " << abs(delta_x) << std::endl;
            fout.close();
			 


		} else if ( (delta_x > 0) && (delta_y > 0)){
			
			int angle_drone = Find_drone_angle(delta_x, delta_y); //!!!!!delta_x
			curent_drone_angle = turn(curent_drone_angle, angle_drone, Ardrone);
            int f = sqrt(delta_y * delta_y + delta_x * delta_x);
			Ardrone.droneForward(f);
			
            std::ofstream fout("cout.txt", std::ios_base::app);
            fout<<" forward " << f << std::endl;
            fout.close();

			 

		} else if ( (delta_x > 0) && (delta_y < 0) ) {
	
			int angle_drone = Find_drone_angle(delta_y, delta_x);      //!!!!!delta_y

			curent_drone_angle = turn(curent_drone_angle, angle_drone + 90, Ardrone);

			int f = sqrt(delta_y * delta_y + delta_x * delta_x);
			Ardrone.droneForward(f);
		
            std::ofstream fout("cout.txt", std::ios_base::app);
            fout<<" forward " << f << std::endl;
            fout.close();
			 

		} else if ( (delta_x < 0) && (delta_y < 0) ) {
			
			int angle_drone = Find_drone_angle(delta_x, delta_y);       //!!!!!delta_x

			curent_drone_angle = turn(curent_drone_angle, angle_drone + 180, Ardrone);

			int f = sqrt(delta_y * delta_y + delta_x * delta_x);
			Ardrone.droneForward(f);
			
            std::ofstream fout("cout.txt", std::ios_base::app);
            fout<<" forward " << f << std::endl;
            fout.close();
			 

		} else if ( (delta_x < 0) && (delta_y > 0) ) {
			
			int angle_drone = Find_drone_angle(delta_y, delta_x);     //!!!!!delta_y
			std::cout << "угол дрона " << angle_drone << std::endl;
			curent_drone_angle = turn(curent_drone_angle, angle_drone + 270, Ardrone);

			int f = sqrt(delta_y * delta_y + delta_x * delta_x);
			Ardrone.droneForward(f);
			
            std::ofstream fout("cout.txt", std::ios_base::app);
            fout<<" forward " << f << std::endl;
            fout.close();
			
		}



		/*else if ( (delta_x == delta_y) && (delta_x > 0) ) {


			curent_drone_angle = turn(curent_drone_angle, 45, Ardrone);
			Ardrone.droneForward(sqrt(2) * abs(delta_x)); //гипотенуза равностороннего треугольника

			sleep(2);
		} else if ( (delta_x == delta_y) && (delta_x < 0) ) {
			curent_drone_angle = turn(curent_drone_angle, 225, Ardrone);
			Ardrone.droneForward(sqrt(2) * abs(delta_x));
			sleep(2);
		} else if ( (abs(delta_x) == abs(delta_y)) && (delta_x > delta_y) ) {
			curent_drone_angle = turn(curent_drone_angle, 135, Ardrone);
			Ardrone.droneForward(sqrt(2) * abs(delta_x));
			sleep(2);
			
		} else if ( (abs(delta_x) == abs(delta_y)) && (delta_y > delta_x) ) {
			curent_drone_angle = turn(curent_drone_angle, 315, Ardrone);
			Ardrone.droneForward(sqrt(2) * abs(delta_x));
			sleep(2);
		} */

		prev_point.x = current_point.x;
		prev_point.y = current_point.y;

        w->pointReachEvent(pos);
    }
}

void action(paint *w) {
	int size;
    std::ifstream fin ("points.txt");
    fin >> size;
    point_t* arr_crd = new point_t[size];
    int x, y, prev_x, prev_y;
    for (int pos = 0; pos < size; pos++) {
        fin >> x >> y;
        if (pos > 0) {
            int delta_x = x - prev_x;
            int delta_y = y - prev_y;
            arr_crd[pos].x = x;
            arr_crd[pos].y = arr_crd[pos-1].y - delta_y;
        } else {
            arr_crd[pos].x = x;
            arr_crd[pos].y = y;
        }
        prev_x = x;
        prev_y = y;
	} 
    fin.close();
    std::ofstream fout("cout.txt");
	for (int pos = 0; pos < size; pos++) {
        fout << std::endl;
        fout << arr_crd[pos].x << " " << arr_crd[pos].y;
	} 
    fout << std::endl;
	
	drone Ardrone;
	Ardrone.droneTakeOff();
	sleep(2);
    Movement(arr_crd, size, Ardrone, w);

    std::cout << "конец" << std::endl;
    fout.close();
    sleep(3);
	Ardrone.droneLand();

}










