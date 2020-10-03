#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <chrono>
#include <thread>
#include <functional>
using namespace std;

struct Customer {
    int arrival;
    int service;
    int wait = 0;
    int server = -1;
    bool completed = false;
};

struct Server {
    int idle_time = 0;
    int processing_item = -1;
    bool idle = true;
};

struct ServerQueue {
    int queue_time = 0;
    int * queued_items;
    int cur_queue = 0;
    int total_in_queue = -1;
};

static int cur_time;
static int total_queue;
static int items_processed;
static int items_left;
static int interval = 10;
static int num_of_servers;
static int total_items;
static int speed;
Customer * customer;
Server * servers;
ServerQueue * queues;

string create_record();
void queue_single();
void queue_server();
void display();
void display_servers();
void timer_start(std::function<void(void)>, unsigned int);
void results();

/*
    Inputs: arrival time, service time

    Process:

    Required methods: enqueue(), dequeue(), queue_single(), queue_server(), display(), time_update()

    Output:
    --------------------------------------------------------------------------------------------------------------------------
    | NO | ARRIVAL TIME | CURRENT QUEUE | WAIT TIME | SERVICE TIME | SERVICE START | SERVICE END | AVG WAIT TIME | IDLE TIME |
    --------------------------------------------------------------------------------------------------------------------------
*/

int main() {
    string filename;
    int option, server;
    cout << "**************************************************" << endl;
    cout << "\t\tAmway File System" << endl;
    cout << "**************************************************" << endl;
    cout << "1 - Use existing file" << endl;
    cout << "2 - Create new record" << endl << endl;
    cout << "Option:\n> ";
    cin >> option;
    cout << endl;
    if(option == 2)
        filename = create_record();
    else {
        cout << "Enter file name:\n> ";
        cin >> filename;
    }

    system("CLS");
    cout << "**************************************************" << endl;
    cout << "\t\tAmway File System" << endl;
    cout << "**************************************************" << endl;
    cout << "Accessing file -> " << filename << endl << endl;
    cout << "Enter total servers:\n> ";
    cin >> num_of_servers;
    system("CLS");

    cout << "**************************************************" << endl;
    cout << "\t\tAmway File System" << endl;
    cout << "**************************************************" << endl;
    cout << "Total servers [" << num_of_servers << "]" << endl;
    cout << "Accessing file -> " << filename << endl << endl;
    cout << "1 - Queue single stack" << endl;
    cout << "2 - Queue servers" << endl << endl;
    cout << "Option:\n> ";
    cin >> option;
    cout << endl;
    // read file and create array
    ifstream in;
    int queue_size;
    in.open(filename.c_str(), ios::in);
    if(in.good()) {
        in >> queue_size;
        customer = new Customer[queue_size];
        servers = new Server[num_of_servers];

        for(int i = 0; i < queue_size; i++) {
            in >> customer[i].arrival;
        }for(int i = 0; i < queue_size; i++) {
            in >> customer[i].service;
        }
        in.close();
        cout << "Enter update interval: (ms)\n> ";
        cin >> speed;

        cur_time = 0;
        total_queue = 0;
        items_processed = 0;
        items_left = queue_size;
        total_items = queue_size;
    }
    if(option == 1) {
        queue_single();
    }

    else {
        queue_server();
    }


    return 0;
}

// create file with queue records
string create_record() {
    string filename;
    cout << "Enter file name (incl .txt):\n> ";
    cin >> filename;
    cout << endl;
    ofstream out;
    out.open(filename.c_str(), ios::out);

    int len;
    cout << "Enter total queues:\n> ";
    cin >> len;
    cout << endl;
    out << len << endl;
    customer = new Customer[len];
    cout << "Arrival time (ms)" << endl;
    for(int i = 0; i < len; i++) {
        cout << "A#" << (i + 1) << "> ";
        cin >> customer[i].arrival;
        out << customer[i].arrival << "\t";
    }
    out << endl;
    cout << endl << "Service time (ms)" << endl;
    for(int i = 0; i < len; i++) {
        cout << "S#" << (i + 1) << "> ";
        cin >> customer[i].service;
        out << customer[i].service << "\t";
    }
    out.close();

    return filename;
}

void queue_single() {
    timer_start(display, speed);
    while(true);
    delete customer;
    delete servers;
}

void queue_server() {
    int lowest_queue_time, index;
    queues = new ServerQueue[num_of_servers];
    for(int i = 0; i < num_of_servers; i++) {
        queues[i].queued_items = new int[total_items];
    }
    // assign all objects to a server and calculate queue times
    for(int i = 0; i < total_items; i++) {
        index = 0;
        lowest_queue_time = 10000;
        for(int j = 0; j < num_of_servers; j++) {
            // if server has no queue -> queue item to server
            if(queues[j].total_in_queue == -1) {
                queues[j].queued_items[queues[j].total_in_queue + 1] = i;
                queues[j].queue_time += (customer[i].arrival + customer[i].service);
                queues[j].total_in_queue++;
                index = -1;
                break;
            }
            // if server has shortest queue -> queue item to server
            int q = queues[j].queue_time + (customer[i].arrival + customer[i].service);
            if(q < lowest_queue_time) {
                lowest_queue_time = q;
                index = j;
            }
        }
        if(index != -1) {
            queues[index].queued_items[queues[index].total_in_queue + 1] = i;
            queues[index].queue_time += (customer[i].arrival + customer[i].service);
            queues[index].total_in_queue++;
        }
    }

    timer_start(display_servers, speed);
    while(true);
    delete customer;
    delete servers;
}

// timeout function
void timer_start(std::function<void(void)> func, unsigned int interval)
{
    std::thread([func, interval]() {
        while (true)
        {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}

// option 1
void display() {
    system("CLS");
    total_queue = 0;
    for(int i = 0; i < total_items; i++) {
            // skip loop if the item is completed or not arrived yet
            if(customer[i].completed || customer[i].arrival > cur_time) {
                continue;
            }
            // if item just got completed -> mark as complete and skip, reset server as idle
            else if(customer[i].service <= 0 && !customer[i].completed) {
                customer[i].completed = true;
                servers[customer[i].server].idle = true;
                servers[customer[i].server].processing_item = -1;
                customer[i].server = -1;
                items_processed++;
                items_left--;
                continue;
            }

    /////////////////////////////////////////////////////////////////////
        for(int j = 0; j < num_of_servers; j++) {
            // if server idle and not assigned server -> assign a job
            if(servers[j].idle && customer[i].server == -1 && customer[i].arrival <= cur_time) {
                customer[i].server = j;
                servers[j].processing_item = i;
                servers[j].idle = false;
                customer[i].service -= interval;
                break;
            }

            // item is being processed
            else if(customer[i].server != -1) {
                customer[i].service -= interval;
                break;
            }
        }
        // all servers full
        if(!customer[i].completed && customer[i].service <= 0) {
            customer[i].wait += interval;
            total_queue++;
        }
    }

    cout << "**************************************************" << endl;
    printf("%12s:%6d    %12s:%6d\n%12s:%6d       %12s:%6d\n", "Time", cur_time, "Items Processed", items_processed, "On Queue", total_queue, "Items Left", items_left);
    cout << "**************************************************" << endl;
    for(int i = 0; i < num_of_servers; i++) {
            cout << "Server " << (i + 1) << "\t|\t";
        if(!servers[i].idle) {
            cout << "Item " << (servers[i].processing_item + 1) << "\t|\t";
            cout << customer[servers[i].processing_item].service;
        } else {
            servers[i].idle_time += interval;
        }
        cout << endl;
    }

    cur_time += interval;

    if(items_left <= 0) {
        results();
        exit(0);
    }
}

// option 2
void display_servers() {
    system("CLS");
    total_queue = items_left;
    for(int i = 0; i < num_of_servers; i++) {
        // if server has a queue
        if(queues[i].total_in_queue > -1) {
            int queue_index = queues[i].queued_items[queues[i].cur_queue];
            // if server is done processing
            if(customer[queue_index].service <= 0) {
                servers[i].idle = true;
                customer[queue_index].completed = true;
                customer[queue_index].server = -1;
                items_left--;
                items_processed++;
                // if there are remaining queued items in the server -> increase index
                if(queues[i].cur_queue < queues[i].total_in_queue) {
                    queues[i].cur_queue++;
                } else {
                    queues[i].total_in_queue = -1;
                    continue;
                }
            }

            // if item arrived -> add item to server
            if(customer[queue_index].arrival <= cur_time) {
                servers[i].idle = false;
                customer[queue_index].server = i;
                servers[i].processing_item = queues[i].queued_items[queues[i].cur_queue];
                customer[queue_index].service -= interval;
                total_queue--;
            }
            // item is being processed
            else {
                customer[queue_index].service -= interval;
                total_queue--;
            }
        }
    }

    cout << "**************************************************" << endl;
    printf("%12s:%6d    %12s:%6d\n%12s:%6d       %12s:%6d\n", "Time", cur_time, "Items Processed", items_processed, "On Queue", total_queue, "Items Left", items_left);
    cout << "**************************************************" << endl;
    for(int i = 0; i < num_of_servers; i++) {
            cout << "Server " << (i + 1) << "\t|\t";
        if(!servers[i].idle) {
            cout << "Item " << (servers[i].processing_item + 1) << "\t|\t";
            cout << customer[servers[i].processing_item].service;
        } else {
            servers[i].idle_time += interval;
        }
        cout << endl;
    }

    cur_time += interval;

    if(items_left <= 0) {
        results();
        exit(0);
    }
}

// print final results
void results() {
    system("CLS");
    float avg_idle, avg_wait;
    // display
    cout << "**************************************************" << endl;
    printf("%12s:%6d    %12s:%6d\n%12s:%6d       %12s:%6d\n", "Time", cur_time, "Items Processed", items_processed, "On Queue", total_queue, "Items Left", items_left);
    cout << "**************************************************" << endl;
    for(int i = 0; i < num_of_servers; i++) {
        cout << "Server " << (i + 1) << "\t|\tTotal idle:\t" << servers[i].idle_time << endl;
        avg_idle += servers[i].idle_time;
    }
    for(int i = 0; i < total_items; i++) {
        avg_wait += customer[i].wait;
    }
    avg_idle /= num_of_servers;
    avg_wait /= total_items;

    cout << endl;
    cout << "**************************************************" << endl;
    printf("%12s:%6.2f    %12s:%6.2f\n", "Avg Idle", avg_idle, "Avg Wait", avg_wait);
    cout << "**************************************************" << endl;
    cout << endl << "Press Enter to continue..." << endl;
    cin.ignore();
    cin.ignore();
}
