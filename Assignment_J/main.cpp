#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
using namespace std;

struct Customer
{
	int cust_arrival_time;
	int cust_service_start_time;
	int cust_service_running_time;
	int cust_service_end_time;
	int cust_wait_time;
	int cust_spending_time;
	int cust_server_number;
	int cust_using_queue_number=0;
};

struct Server
{
	int server_idle_time=0;
};

// DECLARE VARIABLES
int total_waiting_time;
int total_idle_time;
int nextStep;
float avg_waiting_time;
int num_of_customer=20;
int num_of_queue;
int choice;
Customer *customers;
Server *servers;
fstream file;
string fileName;

void singleQueue(int num_of_server) // OPTION 1
{
	// INITIALIZE THE TOTAL TIME
	total_waiting_time=0;
	total_idle_time=0;

	// START THE SERVICE OF THE CUSTOMER 1
	customers[0].cust_service_start_time = customers[0].cust_arrival_time;
	servers[0].server_idle_time = customers[0].cust_service_start_time;

	// CUSTOMER 1'S WAITING TIME IS ZERO, BECAUSE CUSTOMER 1 COMES FIRST.
	customers[0].cust_wait_time=0;

	// CUSTOMER 1'S SERVICE END TIME = CUSTOMER SERVICE START TIME + CUSTOMER SERVICE RUNNING TIME
	customers[0].cust_service_end_time = customers[0].cust_service_start_time+customers[0].cust_service_running_time;

	// ASSIGN THE SERVER 1 AND QUEUE 1 TO THE CUSTOMER 1
	customers[0].cust_server_number = 1;
	customers[0].cust_using_queue_number = 1;

	// CALCULATE THE SPENDING TIME
	customers[0].cust_spending_time = customers[0].cust_service_end_time-customers[0].cust_arrival_time;

	// CUSTOMER 2-20
	for(int i=1; i<20; i++)
	{
		customers[i].cust_wait_time = customers[i-1].cust_service_end_time - customers[i].cust_arrival_time;
		customers[i].cust_service_start_time = customers[i-1].cust_service_end_time;
		customers[i].cust_service_end_time = customers[i].cust_service_start_time+customers[i].cust_service_running_time;
		customers[i].cust_spending_time = customers[i].cust_service_end_time-customers[i].cust_arrival_time;
		customers[i].cust_using_queue_number = 1;

		// CHECK CUSTOMER SERVER NUMBER
		if(customers[i-1].cust_server_number <= num_of_server)
		{
			customers[i].cust_server_number = customers[i-1].cust_server_number+1;

			if(customers[i-1].cust_server_number == num_of_server)
			{
				customers[i].cust_server_number = 1;
			}
		}

		// TOTAL WAITING TIME CALCULATION
		total_waiting_time+=customers[i].cust_wait_time;
	}

	// CALCULATE THE SERVER IDLE TIME
	for(int i=1; i<num_of_server; i++)
	{
		servers[i].server_idle_time=customers[i].cust_service_start_time;
	}
	for(int i=1; i<20; i++)
	{
		for(int j=0; j<num_of_server; j++)
		{
			if(customers[i].cust_server_number!=j+1)
			{
				servers[j].server_idle_time+=customers[i].cust_service_running_time;
			}
		}
	}

	for(int i=0; i<num_of_server; i++)
	{
		total_idle_time+=servers[i].server_idle_time;
	}
}

void multipleQueues(int num_of_server) // OPTION 2
{
	// INITIALIZE THE TOTAL TIME, NEXT CUSTOMER'S INDEX
	total_waiting_time=0;
	total_idle_time=0;
	nextStep=0;

	// ACCORDING TO THE NUMBER OF SERVER, CUSTOMERS STAY IN EACH QUEUE THAT IS SERVED BY EACH SERVER WITHOUT ANY WAITING TIME
	for(int i=0; i<num_of_server; i++)
	{
		customers[i].cust_service_start_time = customers[i].cust_arrival_time;
		customers[i].cust_wait_time = 0;
		customers[i].cust_service_end_time = customers[i].cust_service_start_time+customers[i].cust_service_running_time;
		customers[i].cust_server_number = (i+1);
		customers[i].cust_using_queue_number = (i+1);
		customers[i].cust_spending_time = customers[i].cust_service_end_time-customers[i].cust_arrival_time;
		servers[i].server_idle_time+=customers[i].cust_service_start_time;

		// NEXT CUSTOMER'S INDEX NUMBER PLUS ONE AFTER THE FORMER CUSTOMER IS SERVED.
		nextStep++;
	}

	// SET THE TIMER
	for(int timer=0; ; timer++)
	{
		// THE NUMBER OF QUEUES FOR EACH SERVER
		for(int j=0; j<num_of_server; j++)
		{
			// THE NUMBER OF CUSTOMERS
			for(int i=0; i<20; i++)
			{
				// IF CUSTOMER'S USING QUEUE NUMBER IS j+1
				if(customers[i].cust_using_queue_number==j+1)
				{
					// CHECK A CUSTOMER SERVICE ENDING TIME
					if(timer==customers[i].cust_service_end_time)
					{
						// IF THE NEXT CUSTOMER'S INDEX NUMBER IS NOT 20
						if(nextStep!=20)
						{
							// IF A CUSTOMER'S ARRIVAL TIME IS MORE THAN FORMER CUSTOMER'S SERVICE ENDING TIME
							if(customers[nextStep].cust_arrival_time>customers[i].cust_service_end_time)
							{
								customers[nextStep].cust_service_start_time=customers[nextStep].cust_arrival_time;
								servers[j].server_idle_time+=customers[nextStep].cust_service_start_time-customers[i].cust_service_end_time;
							}
							else
							{
								customers[nextStep].cust_service_start_time=customers[i].cust_service_end_time;
							}

							// SERVE A NEXT CUSTOMER
							customers[nextStep].cust_wait_time=customers[nextStep].cust_service_start_time-customers[nextStep].cust_arrival_time;
							customers[nextStep].cust_service_end_time=customers[nextStep].cust_service_start_time+customers[nextStep].cust_service_running_time;
							customers[nextStep].cust_server_number=j+1;
							customers[nextStep].cust_using_queue_number=j+1;
							customers[nextStep].cust_spending_time=customers[nextStep].cust_service_end_time-customers[nextStep].cust_arrival_time;

							// NEXT PLUS ONE
							nextStep++;
						}
					}
				}
			}
		}
		// IF THE CUSTOMER'S NEXT INDEX NUMBER IS 20, ESCAPE THE FOR LOOP
		if(nextStep==20)
			break;
	}

	// CALCULATE THE AVERAGE WAITING TIME OF CUSTOMERS
	for(int i=0; i<20; i++)
	{
		total_waiting_time+=customers[i].cust_wait_time;
	}

	// CALCULATE IDLE TIME OF SERVERS
	for(int i=0; i<num_of_server; i++)
	{
		total_idle_time+=servers[i].server_idle_time;
	}
}

void display(int num_of_server)
{
	cout << endl;
	int custNo=1;
	int serverNo=1;
	printf("+-------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
	printf("|                                              C U S T O M E R     D A T A      I N     Q U E U E                                                       |\n");
	printf("+-------------------------------------------------------------------------------------------------------------------------------------------------------+\n\n");
	printf("+-----------------+--------------+--------------+--------------------+------------------+----------------+---------------+---------------+--------------+\n");
	printf("| Customer Number | Arrival Time | Waiting Time | Service Start Time | Service End Time | Service Length | Spending Time | Server Number | Queue Number |\n");
	printf("+-----------------+--------------+--------------+--------------------+------------------+----------------+---------------+---------------+--------------+\n");
	for(int i=0; i<20; i++)
	{
		printf("| %15d | %12d | %12d | %18d | %16d | %14d | %13d | %13d | %12d |\n", custNo, customers[i].cust_arrival_time, customers[i].cust_wait_time, customers[i].cust_service_start_time, customers[i].cust_service_end_time, customers[i].cust_service_running_time, customers[i].cust_spending_time, customers[i].cust_server_number, customers[i].cust_using_queue_number);
		printf("+-----------------+--------------+--------------+--------------------+------------------+----------------+---------------+---------------+--------------+\n");
		custNo++;
	}
	cout << endl;

	printf("+----------------------------------+\n");
	printf("|       S E R V E R   D A T A      |\n");
	printf("+----------------------------------+\n\n");
	printf("+---------------+------------------+\n");
	printf("| Server Number | Server Idle Time |\n");
	printf("+---------------+------------------+\n");
	for(int i=0; i<num_of_server; i++)
	{
		printf("| %13d | %16d |\n", serverNo, servers[i].server_idle_time);
		printf("+---------------+------------------+\n");
		serverNo++;
	}
	cout << endl;
	avg_waiting_time=(float)total_waiting_time/20;
	printf("+-----------------------------------+\n");
	printf("|  T I M E   I N F O R M A T I O N  |\n");
	printf("+-----------------------------------+\n\n");
	printf("+----------------------+------------+\n");
	printf("| Average Waiting Time | %10.2f |\n", avg_waiting_time);
	printf("+----------------------+------------+\n");
	printf("|   Total Idle Time    | %10d |\n", total_idle_time);
	printf("+----------------------+------------+\n");
	cout << endl;
}

int num_of_server;
int arrivalTime, runtime;

int main()
{
	do
	{
		cout << "Choose the menu, please" << endl;
		cout << "1 : A single queue." << endl;
		cout << "2 : A queue for each server." << endl;
		cout << "3 : Exit the program." << endl;
		cout << "> ";
		cin >> choice;

		if(choice==1||choice==2)
		{
			cout << "Input a file's name : ";
			cin >> fileName;
			fileName = fileName + ".txt";
			cout << endl;
			file.open(fileName.c_str(), ios::in);
			cout << "Input the number of servers : ";
			cin >> num_of_server;

			// MAKE A DYNAMIC ARRAY FOR CUSTOMERS
			customers = new Customer[num_of_customer];

			// MAKE A DYNAMIC ARRAY FOR SERVERS
			servers = new Server[num_of_server];
		}
		else if(choice==3)
		{
			customers=new Customer[num_of_customer];
			servers=new Server[num_of_server];
		}

		switch(choice)
		{
			case 1: // A SINGLE QUEUE
			{
				if(file.is_open())
				{
					for(int i=0; i<num_of_customer; i++)
					{
						file >> arrivalTime >> runtime;
						customers[i].cust_arrival_time = arrivalTime;
						customers[i].cust_service_running_time = runtime;
					}
					singleQueue(num_of_server);
					display(num_of_server);
				}
				else
				{
					cout << "File Not Found or File cannot be opened!" << endl;
				}
				file.close();
				break;
			}

			case 2: // A QUEUE FOR EACH SERVER
			{
				if(file.is_open())
				{
					for(int i=0; i<num_of_customer; i++)
					{
						file >> arrivalTime >> runtime;
						customers[i].cust_arrival_time = arrivalTime;
						customers[i].cust_service_running_time = runtime;
						 //customers[i].cust_service_running_time;
						//cout << customers[i].cust_arrival_time;
					}
					multipleQueues(num_of_server);
					display(num_of_server);
				}
				else
				{
					cout << "File Not Found or File cannot be opened!" << endl;
				}
				file.close();
				break;
			}

			case 3: // END THE PROGRAM
			{
				cout << "Thank you for using the program. End the program." << endl;
				break;
			}

			default: // VALIDATE THE INPUT VALUE
			{
				cout << "Your input is wrong!" << endl;
			}
		}
		// DE-ALLOCATE ALL MEMORIES USED
		delete [] customers;
		delete [] servers;
	} while(choice!=3);

	return 0;
}
