#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include<ctime>

double currentBalance;

std::time_t getCurrentTime() {
    return std::time(0);
}

void addToBalance(double amount) {
    currentBalance += amount;
}

void removeFromBalance(double amount) {
    currentBalance -= amount;
}

void changeBalance(int numOfBalanceActions, double amountToModify) {
    for (int i = 0; i < numOfBalanceActions; i++) {
        addToBalance(amountToModify);
        removeFromBalance(amountToModify);
    }
}

void sequentialTest(double amountToModify, int numOfBalanceActions, int numOfThreads, int numOfTries) {
    std::time_t timeStart = getCurrentTime();
    for (int currentTry = 0; currentTry < numOfTries; currentTry++) {
        for (int i = 0; i < numOfThreads; ++i) {
            changeBalance(numOfBalanceActions, amountToModify);
        }

        std::cout << "Try " << (currentTry +1) << ", final balance: " << currentBalance << std::endl;
        currentBalance = 0;
    }
    std::time_t timeEnd = getCurrentTime();
    std::cout << "Sequential Test, time elapsed: " << (timeEnd - timeStart) << "s" << std::endl;
}

void parallelUnsafeTest(double amountToModify, int numOfBalanceActions, int numOfThreads, int numOfTries) {
    std::time_t timeStart = getCurrentTime();
    for (int currentTry = 0; currentTry < numOfTries; currentTry++) {
        
        std::vector<std::thread> threads;
        for (int i = 0; i < numOfThreads; ++i) {
            threads.emplace_back(changeBalance, numOfBalanceActions, amountToModify);
        }

        for (auto& t : threads) {
            t.join();
        }

        std::cout << "Try " << (currentTry +1) << ", final balance: " << currentBalance << std::endl;
        currentBalance = 0;
    }
    std::time_t timeEnd = getCurrentTime();
    std::cout << "Unsafe Test, time elapsed: " << (timeEnd - timeStart) << "s" << std::endl;
}

void parallelSafeTest() { 
    // tbi
}

int main() {

    std::cout << "Enter the balance's initial value (Default: 0): ";
    std::getline(std::cin, input);
    currentBalance = input != "" ? std::stoi(input) : 0;

    std::cout << "Enter the value that the balance should be modified with (Default: 1): ";
    std::getline(std::cin, input);
    double amountToModify = input != "" ? std::stod(input) : 1;

    std::cout << "Enter how many times should the balance be modifed, i.e. add/remove (Default: 5000): ";
    std::getline(std::cin, input);
    int numOfBalanceActions = input != "" ? std::stoi(input) : 5000;

    std::cout << "Enter the amount of threads to run the tests (Default: 5): ";
    std::getline(std::cin, input);
    int numOfThreads = input != "" ? std::stoi(input) : 5;

    std::cout << "Enter how many times do you want to run the tests (Default: 10): ";
    std::getline(std::cin, input);
    int numOfTries = input != "" ? std::stoi(input) : 10;

    // parallelUnsafeTest(amountToModify, numOfBalanceActions, numOfThreads, numOfTries);
    sequentialTest(amountToModify, numOfBalanceActions, numOfThreads, numOfTries);
    
    return 0;
}
