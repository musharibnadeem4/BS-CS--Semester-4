//Musharib
//i201764
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

const int MIN_RANDOM = 10;
const int MAX_RANDOM = 99;

int generateRandomNumber() {
    return rand() % (MAX_RANDOM - MIN_RANDOM + 1) + MIN_RANDOM;
}


int Number_n() {
    int random = generateRandomNumber();
    int lastDigitOfRollNumber = 1764 % 10;
    int result = 1764 / random;

    result = result % 25;
    if (result < 10) {
        result += 15;
    }

    return result;
}
int** CreateMatrix(int n) {
    int** matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
        for (int j = 0; j < n; j++) {
            matrix[i][j] = Number_n();
        }
    }
    return matrix;
}
   
// Function to save a matrix to a text file
void saveMatrixToFile(int** matrix, int n, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                file << matrix[i][j] << ' ';
            }
            file << '\n';
        }
        file.close();
    } else {
        cerr << "Error: Unable to open file for writing." << endl;
    }
}

// Function to multiply two matrices and return the result
int** multiplyMatrices(int** matrix1, int** matrix2, int n) {
    int** result = new int*[n];
    for (int i = 0; i < n; i++) {
        result[i] = new int[n];
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int n = Number_n();

    int** randomMatrix = CreateMatrix(n);
    saveMatrixToFile(randomMatrix, n, "matrix.txt");

    // Fork n processes to perform matrix multiplication
    for (int i = 1; i <= n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Fork failed." << endl;
            return 1;
        } else if (pid == 0) {
            // Child process
            int** matrix1 = randomMatrix;
            int** matrix2 = CreateMatrix(n);
            int** result = multiplyMatrices(matrix1, matrix2,  n);

            // Save the result to a file
            string filename = "result_matrix" + to_string(i) + ".txt";
            saveMatrixToFile(result,   n, filename);

            exit(0);
        }
    }
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    // Read and display the resultant matrix
    int** finalResultMatrix = CreateMatrix(n);
    for (int i = 1; i <= n; i++) {
        string filename = "result_matrix" + to_string(i) + ".txt";
        ifstream file(filename);
        if (file.is_open()) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    int value;
                    file >> value;
                    finalResultMatrix[j][k] += value;
                }
            }
            file.close();
        }
    }

    cout << "Final Resultant Matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << finalResultMatrix[i][j] << ' ';
        }
        cout << endl;
    }

    // Clean up dynamically allocated memory
    for (int i = 0; i < n; i++) {
        delete[] randomMatrix[i];
        delete[] finalResultMatrix[i];
    }
    delete[] randomMatrix;
    delete[] finalResultMatrix;

    return 0;
}
