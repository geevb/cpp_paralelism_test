#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>

std::mutex mtx;
double currentBalance;
void addToBalance(double amount, bool safeMode) {
    if (safeMode) mtx.lock();
    currentBalance += amount;
    if (safeMode) mtx.unlock();

    // Simulate other processes needed for the transaction to happen...
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
}

void removeFromBalance(double amount, bool safeMode) {
    if (safeMode) mtx.lock();
    currentBalance -= amount;
    if (safeMode) mtx.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

std::chrono::_V2::system_clock::time_point getCurrentTime() {
    return std::chrono::high_resolution_clock::now();
}

void printTryResult(int currentTry) {
    std::cout << "Try " << currentTry << ", final balance: " << currentBalance << std::endl;
}

void printTestResult(
    std::string testType, 
    std::chrono::_V2::system_clock::time_point timeStart, 
    std::chrono::_V2::system_clock::time_point timeEnd
) { 
    std::chrono::duration<double> diff = timeEnd - timeStart;
    std::cout << testType << ", time elapsed: " << diff.count() << "s" << std::endl;
}

void changeBalance(int numOfBalanceActions, double amountToModify, bool safeMode) {
    for (int i = 0; i < numOfBalanceActions; i++) {
        addToBalance(amountToModify, safeMode);
        removeFromBalance(amountToModify, safeMode);
    }
}

void sequentialTest(double amountToModify, int numOfBalanceActions, int numOfThreads, int numOfTries) {
    auto timeStart = getCurrentTime();
    for (int currentTry = 0; currentTry < numOfTries; currentTry++) {
        for (int i = 0; i < numOfThreads; ++i) {
            changeBalance(numOfBalanceActions, amountToModify, false);
        }

        printTryResult(currentTry +1);
        currentBalance = 0;
    }

    printTestResult("Sequential Test", timeStart, getCurrentTime());
}

void parallelTest(double amountToModify, int numOfBalanceActions, int numOfThreads, int numOfTries, bool safeMode) {
    auto timeStart = getCurrentTime();
    for (int currentTry = 0; currentTry < numOfTries; currentTry++) {
        
        std::vector<std::thread> threads;
        for (int i = 0; i < numOfThreads; ++i) {
            threads.emplace_back(changeBalance, numOfBalanceActions, amountToModify, safeMode);
        }

        for (auto& t : threads) {
            t.join();
        }

        printTryResult(currentTry +1);
        currentBalance = 0;
    }

    std::string testType = std::string(safeMode ? "Safe" : "Unsafe") + " Test";
    printTestResult(testType, timeStart, getCurrentTime());
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
    if (testType == 2) parallelTest(amountToModify, numOfBalanceActions, numOfThreads, numOfTries, false);
    if (testType == 3) parallelTest(amountToModify, numOfBalanceActions, numOfThreads, numOfTries, true);
    
    return 0;
}
