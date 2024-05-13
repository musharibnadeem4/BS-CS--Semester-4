/*Musharib
20i-1764*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

using namespace std;

pthread_mutex_t mutex_bridge;
pthread_cond_t bridge_condition;
int num_cars_on_bridge = 0;
int num_buses_on_bridge = 0;

void ArriveAtBridge(int direction, const string& vehicleType) {
    pthread_mutex_lock(&mutex_bridge);

    while ((vehicleType == "bus" && (num_cars_on_bridge >= 4 || num_buses_on_bridge > 0)) ||
           (vehicleType == "car" && (num_buses_on_bridge >= 1 || num_cars_on_bridge >= 2))) {
        pthread_cond_wait(&bridge_condition, &mutex_bridge);
    }

    if (vehicleType == "bus") {
        num_buses_on_bridge++;
    } else {
        num_cars_on_bridge++;
    }

    cout << "Vehicle (type: " << vehicleType << ", direction: " << direction << ") is crossing the bridge." << std::endl;

    pthread_mutex_unlock(&mutex_bridge);
}

void ExitTheBridge(const string& vehicleType) {
    pthread_mutex_lock(&mutex_bridge);

    if (vehicleType == "bus") {
        num_buses_on_bridge--;
    } else {
        num_cars_on_bridge--;
    }

    cout << "Vehicle (type: " << vehicleType << ") has exited the bridge." <<endl;

    pthread_cond_broadcast(&bridge_condition);

    pthread_mutex_unlock(&mutex_bridge);
}

void* vehicle_thread(void* arg) {
    int direction = *((int*)arg);
    string vehicle_type(reinterpret_cast<char*>(arg) + sizeof(int));

     ArriveAtBridge(direction, vehicle_type);

    sleep(1);

    ExitTheBridge(vehicle_type);

    return NULL;
}

int main() {
    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex_bridge, NULL);
    pthread_cond_init(&bridge_condition, NULL);

    // Create vehicle threads
    const int num_vehicles = 10;
    pthread_t vehicle_threads[num_vehicles];
    char vehicle_args[num_vehicles][20]; 
    for (int i = 0; i < num_vehicles; ++i) {
        int direction = i % 2;
        string vehicle_type = (i / 5 == 0) ? "car" : "bus";
        memcpy(vehicle_args[i], &direction, sizeof(int));
        strcpy(vehicle_args[i] + sizeof(int), vehicle_type.c_str());
        pthread_create(&vehicle_threads[i], NULL, vehicle_thread, vehicle_args[i]);
    }

    // Wait for vehicle threads to finish
    for (int i = 0; i < num_vehicles; ++i) {
        pthread_join(vehicle_threads[i], NULL);
    }
  
    pthread_mutex_destroy(&mutex_bridge);
    pthread_cond_destroy(&bridge_condition);

    return 0;
}

