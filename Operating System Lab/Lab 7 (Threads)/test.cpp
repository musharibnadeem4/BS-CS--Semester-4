#include <iostream>
#include <pthread.h>

using namespace std;

class Rectangle {
public:
    Rectangle(double w, double h) : width(w), height(h) {}

    void calculatePerimeter() {
        double perimeter = 2 * (width + height);
        cout << "Rectangle Perimeter: " << perimeter << endl;
    }

    void calculateArea() {
        double area = width * height;
        cout << "Rectangle Area: " << area << endl;
    }

private:
    double width;
    double height;
};

void* calculate_perimeter(void* arg) {
    Rectangle* rect = static_cast<Rectangle*>(arg);
    rect->calculatePerimeter();
    pthread_exit(NULL);
}

void* calculate_area(void* arg) {
    Rectangle* rect = static_cast<Rectangle*>(arg);
    rect->calculateArea();
    pthread_exit(NULL);
}

int main() {
    Rectangle rectangle(5.0, 10.0);

    pthread_t perimeter_tid, area_tid;

    int result_perimeter = pthread_create(&perimeter_tid, NULL, calculate_perimeter, &rectangle);
    int result_area = pthread_create(&area_tid, NULL, calculate_area, &rectangle);

    cout << "Main thread" << endl;

    pthread_join(perimeter_tid, NULL);
    pthread_join(area_tid, NULL);

    return 0;
}
