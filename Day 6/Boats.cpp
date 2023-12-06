#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>

int64_t waysToBeatTheRecord(int64_t time, int64_t distance) {
    double timeFloat = (double)time;
    double distFloat = (double)distance;
    double maxTime = timeFloat / 2.0; // hold time which maximizes distance
    double discriminant = 0.25 * timeFloat * timeFloat - distFloat;
    discriminant = sqrt(discriminant);
    double lowTimeFloat = maxTime - discriminant;
    double highTimeFloat = maxTime + discriminant;

    int64_t lowTime = lowTimeFloat;
    int64_t highTime = highTimeFloat;

    // test these values in case double result is near integer
    if (time * lowTime - lowTime * lowTime <= distance) {
        // lowTime doesn't actually beat the record
        lowTime++;
    }
    if (time * highTime - highTime * highTime <= distance) {
        // highTime doesn't actually beat the record
        highTime--;
    }

    return highTime - lowTime + (int64_t)1;
}

int main(int argc, char* argv[]) {
    std::ifstream ifs;
    if (argc > 1) {
        ifs.open(argv[1]);
    }
    else {
        ifs.open("input.txt");
    }
    if (ifs.fail()) {
        std::cout << "Could not open input file" << std::endl;
        return -1;
    }

    std::string buf;
    ifs >> buf; // "Time:"

    std::vector<int64_t> times;
    std::vector<int64_t> distances;
    int64_t intBuf;
    ifs >> intBuf;
    while (!ifs.eof() && !ifs.fail()) {
        std::cout << "Found time " << intBuf << std::endl;
        times.push_back(intBuf);
        ifs >> intBuf;
    }
    ifs.clear();

    ifs >> buf; // "Distance:"

    ifs >> intBuf;
    while (!ifs.eof() && !ifs.fail()) {
        std::cout << "Found distance " << intBuf << std::endl;
        distances.push_back(intBuf);
        ifs >> intBuf;
    }
    
    // prepare part 2 input
    std::stringstream timeStringStream;
    std::stringstream distanceStringStream;
    auto time = times.begin();
    auto distance = distances.begin();
    while (time != times.end() && distance != distances.end()) {
        timeStringStream << *time;
        distanceStringStream << *distance;

        time++;
        distance++;
    }
    int64_t bigTime, bigDistance;
    timeStringStream >> bigTime;
    distanceStringStream >> bigDistance;

    auto start = std::chrono::steady_clock::now();
    time = times.begin();
    distance = distances.begin();
    int64_t finalResult = 1;
    while (time != times.end() && distance != distances.end()) {
        int64_t numberOfWays = waysToBeatTheRecord(*time, *distance);
        finalResult *= numberOfWays;

        time++;
        distance++;
    }

    // Part 2
    int64_t bigResult = waysToBeatTheRecord(bigTime, bigDistance);

    auto end = std::chrono::steady_clock::now();

    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns" << std::endl;
    std::cout << "Final result: " << finalResult << std::endl;
    std::cout << "Big result: " << bigResult << std::endl;
}

