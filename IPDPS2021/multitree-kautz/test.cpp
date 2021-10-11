#include <iostream>

int main() {

    //Hey, pointers have a finite size, no matter the indirection level!
    std::cout << "sizeof(int*): " << sizeof(int*) << std::endl;
    std::cout << "sizeof(int**): " << sizeof(int**) << std::endl;
    std::cout << "sizeof(int***): " << sizeof(int***) << std::endl;
    int d  = 5;
    //Create an array of pointers that points to more arrays
    int** matrix = new int*[d];
    for (int i = 0; i < 5; ++i) {
        matrix[i] = new int[d];
        for (int j = 0; j < 5; ++j) {
            matrix[i][j] = i*5 + j;
        }
    }

    //Print out the matrix to verify we have created the matrix
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            std::cout << matrix[j][i] << std::endl;
        }
    }

    //Free each sub-array
    for(int i = 0; i < 5; ++i) {
        delete matrix[i];   
    }
    //Free the array of pointers
    delete matrix;

    return 0;
}