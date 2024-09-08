
// Truck Platooning System

#include <iostream>
#include <string>
#include <thread>
#include <omp.h>

using namespace std;

class Truck
{

private:
    int id; // 0 to 3 for four trucks

public:
    bool role; // 1 if leader, 0 if follower
    int velocity;
    int direction; // 1 if ahead, 2 if right, 3 if left

    Truck(int iD) { id = iD; } // constructor
    void assignLeadership() { role = 0; };
    void assignFollowership() { role = 1; };
    static void sensor(){};
    static void radar(){};
    static void lidar(){};
    static void gps(){};

    // leader methods
    static void acceptConnectionReq(){};
    void sendData();

    // Follower methods
    static void sendConnectionReq(){};
    void receiveData();
    void steering(Truck);
    void accelerate(Truck);
    void brake(Truck);
};

void Truck::steering(Truck t)
{

    if (t.direction == 0)
    {
        cout << "Drive ahead" << endl;
    }

    if (t.direction == 1)
    {
        cout << "Drive right" << endl;
    }

    if (t.direction == 2)
    {
        cout << "Drive left" << endl;
    }
}

void Truck::accelerate(Truck t)
{

    if (t.velocity < 50)
    {
        cout << "Truck " << id << " is Accelerating";
    }
}

void Truck::brake(Truck t)
{
    if (t.velocity > 50)
    {
        cout << "Truck " << id << " is Breaking";
    }
}

void Truck::sendData()
{
    if (this->role == 0)
    {
        cout << "Sent data is " << endl;
    }
}

void Truck::receiveData()
{
    if (this->role == 1)
    {
        cout << "Received data is " << endl;
    }
}

Truck truck0(0); // name differently on each system

void thread_drive()
{

    while (true)
    {

#pragma omp parallel num_threads(3)
        {

#pragma omp task
            {

                truck0.accelerate(truck0);
                this_thread::sleep_for(1000ms);
            }

#pragma omp task
            {

                truck0.brake(truck0);
                this_thread::sleep_for(1000ms);
            }

#pragma omp task
            {
                truck0.steering(truck0);
                this_thread::sleep_for(1000ms);
            }
        }
    }
}

void thread_communication()
{

    while (true)
    {

#pragma omp parallel num_threads(2)
        {

#pragma omp task
            {

                truck0.sendData();
                this_thread::sleep_for(1000ms);
            }

#pragma omp task
            {

                truck0.receiveData();
                this_thread::sleep_for(1000ms);
            }
        }
    }
}

void thread_input()
{

    while (true)
    {

#pragma omp parallel num_threads(4)
        {

#pragma omp task
            {
                Truck::sensor();
                this_thread::sleep_for(1000ms);
            }

#pragma omp task
            {
                Truck::radar();
                this_thread::sleep_for(1000ms);
            }

#pragma omp task
            {
                Truck::lidar();
                this_thread::sleep_for(1000ms);
            }

#pragma omp task
            {
                Truck::gps();
                this_thread::sleep_for(1000ms);
            }
        }
    }
}

int main()
{

    truck0.assignLeadership();

    if (truck0.role == 1)
    {
        Truck::acceptConnectionReq();
    }

    if (truck0.role == 0)
    {
        Truck::sendConnectionReq();
    }

    thread t1(thread_drive);
    thread t2(thread_communication);
    thread t3(thread_input);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}