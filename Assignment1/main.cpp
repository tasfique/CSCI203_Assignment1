//NAME - TASFIQUE ENAM
//STUDENT ID- 5886429
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer
{
    // declare private variables
private:
    int Customers_ID;
    float ArrivalTime;
    float Service;
    // declare public methods
public:
    Customer();
    void setCusID(int);
    void setArrivalTime(float);
    void setServiceTime(float);
    int getCusID();
    float getArrivalTime();
    float getServiceTime();
    ~Customer();

};

#endif


#ifndef QUEUE_H
#define QUEUE_H

class Queue
{
    // declare private structure and pointer
private:
    struct customerQ
    {
        Customer customer;
        customerQ* nextCustomer;
    };
    customerQ* beg;
    customerQ* endd;
    int Size;
    // declare public methods
public:
    Queue();
    void enQueue(Customer);
    void deQueue();
    int size();
    Customer front();
    ~Queue();
};

#endif


#ifndef SERVER_H
#define SERVER_H


class Server
{
    // declare private variables
private:
    int customerID;
    float Service;
    float Idle;
    float done;
    int sumService;
    bool check;
    // declare public methods
public:
    Server();
    void setCustomer(Customer, float);
    void setIdle(float);
    void increaseTime(float);
    int getCustomerID();
    float getIdle();
    float getFinish();
    bool isAvailable();
    ~Server();
};

#endif


// declare variables
float waitTime[2] = {0, 0};
float idleTime[2] = {0, 0};

// prototype function
void JOB1(Customer[], int, int);
void JOB2(Customer[], int, int);



// Constructor
Customer::Customer()
{
    Customers_ID = 0;
    ArrivalTime = 0;
    Service = 0;
}

// setCusID method
void Customer::setCusID(int cid)
{
    Customers_ID = cid;
}

// setArrivalTime method
void Customer::setArrivalTime(float firstTime)
{
    ArrivalTime = firstTime;
}

// setServiceTime method
void Customer::setServiceTime(float secondTime)
{
    Service = secondTime;
}

// Get CustomerID method
int Customer::getCusID()
{
    return Customers_ID;
}

// Get Arrival Time method
float Customer::getArrivalTime()
{
    return ArrivalTime;
}

// Get Service Time method
float Customer::getServiceTime()
{
    return Service;
}

// Destructor
Customer::~Customer()
{
    //Destructor.
}

//#include "Queue.h"

// Construtor
Queue::Queue()
{
    beg = NULL;
    endd = NULL;
    Size = 0;
}

// enQueue Method
void Queue::enQueue(Customer cus)
{
    customerQ* item = new customerQ();
    item->customer = cus;
    item->nextCustomer = NULL;

    if(endd == nullptr)
    {
        endd = item;
        beg = endd;
    }
    else
    {
        beg->nextCustomer = item;
        beg = item;
    }
    Size++;
}

// deQueue Method
void Queue::deQueue()
{
    customerQ* temp = endd;
    endd = endd->nextCustomer;
    delete temp;
    Size--;
}

// get size Method
int Queue::size()
{
    return Size;
}

// get front method
Customer Queue::front()
{
    return endd->customer;
}

// Destructor
Queue::~Queue()
{
    //Destructor.
}

// Constructor
Server::Server()
{
    customerID = 0;
    Service = 0;
    Idle = 0;
    done = 0;
    sumService = 0;
    check = true;
}

// Set Customer method
void Server::setCustomer(Customer cus, float cusT)
{
    customerID = cus.getCusID();
    Service = cus.getServiceTime();
    done= cusT + cus.getServiceTime();
    check = false;
}

// set idle method
void Server::setIdle(float idleTime)
{
    Idle += idleTime;
}

// IncreaseTime method
void Server::increaseTime(float time)
{
    Service-= 0.01;
    string justfinish= to_string(done);
    string total = to_string(time);
    if((Service<=0)||(justfinish==total))
    {
        sumService++;
        check = true;
    }
}

// getCustomerID method
int Server::getCustomerID()
{
    return customerID;
}

// getIdle Method
float Server::getIdle()
{
    return Idle;
}

// get Finish method
float Server::getFinish()
{
    return done;
}

// isAvailable method
bool Server::isAvailable()
{
    return check;
}

// Destructor
Server::~Server()
{
}

int main()
{

    Customer customer[21];
    int nServer= 0;
    string fileN;
    int count = 0;
    ifstream fin;

    string filename;

    cout << "FILE NAME ";
    cin >> fileN;
    fileN += ".txt";
    fin.open(fileN, ios::in);


    if (!fin.good()) {
        cout << "EXITING THE PROGRAM " << endl;
        return 1;
    }

    else {

        while (!fin.eof()) {
            int id = count + 1;
            float timeA;
            float timeS;
            fin >> timeA >> timeS;
            customer[count].setCusID(id);
            customer[count].setArrivalTime(timeA);
            customer[count].setServiceTime(timeS);
            count++;
        }
    }

    fin.close();


    cout << "-----------------------CUSTOMER-----------------------" << endl;
    cout << "+----------------------------------------------------+" << endl;
    cout << "|     CUSTOMER NUM     |  ARRIVAL  |  SERVICE TIME    |" << endl;
    cout << "+----------------------------------------------------+" << endl;
    for(int i=0; i<count-1 ; i++)
        cout << "|       " << left << setw(11) <<  customer[i].getCusID() << "|    " << left << setw(12) << customer[i].getArrivalTime() << "|    " << left << setw(12) << customer[i].getServiceTime() << "|" << endl;
    cout << "+----------------------------------------------------+" << endl;

    Customer method1[21] = customer;
    Customer method2[21] = customer;

   ///ASK FOR THE NUMBER OF SERVERS.
    cout << "Enter the number of server(s): ";
    cin >> nServer;
    JOB1(method1, nServer, count - 1);
    JOB2(method2, nServer, count - 1);

    // results display
    cout << endl;
    cout << "************************************************ DETAIL ***********************************************" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| STRATIGIES:            |   " << left << setw(34) << "FIRST STRATEGY" << left << setw(38) <<" |   SECOND STRATEGY" << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| TOTAL WAITING TIME:    |   " << left << setw(34) << waitTime[0] << " |   " << left << setw(33) << waitTime[1] << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| AVERAGE WAITING TIME:  |   " << left << setw(34) << waitTime[0] / (count - 1) << " |   " << left << setw(33) << waitTime[1] / (count - 1) << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| TOTAL IDLE TIME:       |   " << left << setw(34) << idleTime[0] << " |   " << left << setw(33) << idleTime[1] << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;


    return 0;
}

///JOB1 1ST STRATEGY
void JOB1(Customer customer[], int noServer, int noCustomer)
{
    // declare variables
    float currentTime = 0;
    int finish= 0;
    Queue customerquee;
    Server server[noServer];

    cout << "\n---------------------------------------------FIRST STRATEGY--------------------------------------------" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| " << setw(10) << left << "   TIME" << left << setw(90) << " |           EVENTS   " <<  "|" <<  endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    do
    {
        if(currentTime != 0)
        {
            for(int i=0; i<noServer; i++)
            {
                if(!server[i].isAvailable())
                {
                    server[i].increaseTime(currentTime);
                    if((server[i].isAvailable()))
                    {
                        cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << server[i].getCustomerID() << " HAS FININSHED IN SERVER " << left << setw(44) << i + 1 << " |" << endl;
                        finish++;
                    }
                }
            }
        }

        if(customerquee.size()!=0)
        {
            for(int i=0; i<noServer; i++)
            {

                if(server[i].isAvailable())
                {
                    server[i].setCustomer(customerquee.front(), currentTime);
                    waitTime[0] += currentTime - customerquee.front().getArrivalTime();
                    cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << customerquee.front().getCusID() << " FROM THE QUEUE HAS STARTED IN SERVER " << i + 1 << ", AND IT FINISHED IN " << left << setw(23) << server[i].getFinish() << " |" << endl;
                    customerquee.deQueue();
                }
            }
        }

        bool checking= false;

        for(int i=0; i<noCustomer; i++)
        {
            string first = to_string(currentTime);
            string second = to_string(customer[i].getArrivalTime());
            if(first == second)
            {
                for(int j=0; j<noServer; j++)
                {

                    if(server[j].isAvailable())
                    {

                        if(server[j].getFinish()<customer[i].getArrivalTime())
                        {
                            float idleTime = customer[i].getArrivalTime() - server[j].getFinish();
                            server[j].setIdle(idleTime);
                        }
                        server[j].setCustomer(customer[i], currentTime);
                        cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << customer[i].getCusID() << " STARTS IN SERVER " << j + 1 << " AND FINISHES AT " << left << setw(40) << server[j].getFinish() << " |" << endl;

                        customer[i].setArrivalTime(-1);
                        checking = true;
                        break;
                    }
                }
                if(!checking)
                {
                    cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << customer[i].getCusID() << left << setw(76) << " HAS ARRIVED." << " |" << endl;
                    customerquee.enQueue(customer[i]);

                    customer[i].setArrivalTime(-1);
                }
            }
            if(checking)
                break;
        }


        if(finish!=noCustomer)
            currentTime++;

    }while(finish!=noCustomer);



    for(int i=0; i<noServer; i++)
    {
        server[i].setIdle(currentTime - server[i].getFinish());
        idleTime[0] += server[i].getIdle();
    }

    // display results
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| CURRENT TIME: " << "\t\t |  " << left << setw(65) << currentTime << " |" << endl;
    cout << "| TOTAL CUSTOMERS: " << "\t\t |  " << left << setw(65)  << noCustomer << " |" << endl;
    cout << "| TOTAL SERVICE: " << "\t\t |  " << left << setw(65) << finish << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "|" << "\t SERVERS" << "\t |" << "\t IDLE TIME" << "\t |" << left << setw(46) << "\t JOB FINISHED" << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    for(int i=0; i<noServer; i++)
    {
        cout << "|" << "\t Server " << i+1 << "\t | \t" << left << setw(12) << server[i].getIdle() << " | \t" << left << setw(53) << server[i].getFinish() << " |" << endl;
    }
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << left << setw(40) << "| THE TOTAL WAITING TIME: " << " |  " << left << setw(57) << waitTime[0] << " |" << endl;
    cout << left << setw(40) << "| AAVERAGE WAITING TIME: " << " |  " << left << setw(57) << waitTime[0]/noCustomer << " |" << endl;
    cout << left << setw(40) << "| TOTAL IDLE TIME FOR ALL SERVERS(s): " << " |  " << left << setw(57) << idleTime[0] << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
}

///JOB2 THE 2ND STRATEGY
void JOB2(Customer customer[], int noServer, int noCustomer)
{

    float currentTime = 0;
    int numCustomerDone = 0;
    Queue customerquee[noServer];
    Server server[noServer];

    cout << "\n---------------------------------------------SECOND STRATEGY-------------------------------------------" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| " << setw(10) << left << "   TIME" << left << setw(90) << " |           EVENTS   " <<  "|" <<  endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    do
    {
        //rUN SERVER WHICH HAS CUSTOMER.
        if(currentTime != 0)
        {
            for(int i=0; i<noServer; i++)
            {
                if(!server[i].isAvailable())
                {
                    server[i].increaseTime(currentTime);
                    if((server[i].isAvailable()))
                    {
                        cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << server[i].getCustomerID() << " HAS FINISHED IN SERVER  " << left << setw(44) << i + 1 << " |" << endl;
                        numCustomerDone++;
                    }
                }
            }
        }

        for(int i=0; i<noServer; i++)
        {
            // Queue is not empty
            if(customerquee[i].size()!=0)
            {

                if(server[i].isAvailable())
                {
                    server[i].setCustomer(customerquee[i].front(), currentTime);
                    waitTime[1] += currentTime - customerquee[i].front().getArrivalTime();
                    cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << customerquee[i].front().getCusID() << " FROM THE QUEUE " << i + 1 << " HAS STARTED IN SERVER " << i + 1 << ", FINISHED IN " << left << setw(25) << server[i].getFinish() << " |" << endl;
                    customerquee[i].deQueue();
                }
            }
        }


        int minQueue = 0;
        for(int i=1; i<noServer; i++)
        {
            if(customerquee[minQueue].size() > customerquee[i].size())
            {
                minQueue = i;
            }
        }

        bool checking = false;


        for(int i=0; i<noCustomer; i++)
        {
            if(currentTime == customer[i].getArrivalTime())
            {
                for(int j=0; j<noServer; j++)
                {

                    if(server[j].isAvailable() && minQueue == j)
                    {

                        if(server[j].getFinish()<customer[i].getArrivalTime())
                        {
                            float idleTime = customer[i].getArrivalTime() - server[j].getFinish();
                            server[j].setIdle(idleTime);
                        }
                        server[j].setCustomer(customer[i], currentTime);
                        cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << customer[i].getCusID() << " STARTS IN SERVER " << j + 1 << ", AND FINISHED AT " << left << setw(39) << server[j].getFinish() << " |" << endl;

                        customer[i].setArrivalTime(0);
                        checking = true;
                        break;
                    }
                }
                if(checking == false)
                {
                    cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << customer[i].getCusID() << left << setw(76) << " HAS ARRIVED." << " |" << endl;
                    cout << "|   " << left << setw(8) << currentTime << " | CUSTOMER " << customer[i].getCusID() << " HAS JOINED THE QUEUE " << minQueue + 1 << " WITH A SIZE OF " << left << setw(41) << customerquee[minQueue].size() << " |" << endl;
                    customerquee[minQueue].enQueue(customer[i]);

                    customer[i].setArrivalTime(0);
                }
            }
            if(checking == true)
                break;
        }


        if(numCustomerDone!=noCustomer)
            currentTime++;

    }while(numCustomerDone!=noCustomer);

    for(int i=0; i<noServer; i++)
    {
        server[i].setIdle(currentTime - server[i].getFinish());
        idleTime[1] += server[i].getIdle();
    }

    /// DISPLAY OF RESULTS


    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << left << setw(40) << "| Total waiting time: " << " |  " << left << setw(57) << waitTime[1] << " |" << endl;
    cout << left << setw(40) << "| Average waiting time: " << " |  " << left << setw(57) << waitTime[1]/noCustomer << " |" << endl;
    cout << left << setw(40) << "| Total idle time of all server(s): " << " |  " << left << setw(57) << idleTime[1] << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;

     cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "| Current Time: " << "\t\t |  " << left << setw(65) << currentTime << " |" << endl;
    cout << "| Total customers: " << "\t\t |  " << left << setw(65)  << noCustomer << " |" << endl;
    cout << "| Total service: " << "\t\t |  " << left << setw(65) << numCustomerDone << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
    cout << "|" << "\t Servers" << "\t |" << "\t Idle Time" << "\t |" << left << setw(46) << "\t Job Finish" << " |" << endl;
    cout << "+-----------------------------------------------------------------------------------------------------+" << endl;
        for(int i=0; i<noServer; i++)
        {
            cout << "|" << "\t Server " << i+1 << "\t | \t" << left << setw(12) << server[i].getIdle() << " | \t" << left << setw(53) << server[i].getFinish() << " |" << endl;
        }
}
