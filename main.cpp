#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include<ctime>

double currentBalance;

std::time_t getCurrentTime() {
    return std::time(0);
}

void printTryResult(int currentTry) {
    std::cout << "Try " << currentTry << ", final balance: " << currentBalance << std::endl;
}

void printTestResult(std::string testType, std::time_t timeStart, std::time_t timeEnd) {
    std::cout << testType << ", time elapsed: " << (timeEnd - timeStart) << "s" << std::endl;
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

        printTryResult(currentTry +1);
        currentBalance = 0;
    }
    printTestResult("Sequential Test", timeStart, getCurrentTime());
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

        printTryResult(currentTry +1);
        currentBalance = 0;
    }

    printTestResult("Unsafe Test", timeStart, getCurrentTime());
}

void parallelSafeTest(double amountToModify, int numOfBalanceActions, int numOfThreads, int numOfTries) { 
    // tbi
}

int main() {
    std::string input;

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

    std::cout << "Enter the num of the test you'd like to run. Sequential(1), Parallel Unsafe(2), Parallel Safe(3): ";
    std::getline(std::cin, input);
    int testType = input != "" ? std::stoi(input) : 1;

    if (testType == 1) sequentialTest(amountToModify, numOfBalanceActions, numOfThreads, numOfTries);
    if (testType == 2) parallelUnsafeTest(amountToModify, numOfBalanceActions, numOfThreads, numOfTries);
    if (testType == 3) parallelSafeTest(amountToModify, numOfBalanceActions, numOfThreads, numOfTries);
    
    return 0;
}
