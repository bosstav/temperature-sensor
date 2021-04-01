// TempJSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream> 
#include <fstream>
#include <string> 
#include <vector> 
#include <array>        // std::array
#include <algorithm>//For min_element, std::max_element
#include <numeric>  //For Accumulate
#include <chrono>   //For Sleep
#include <thread>   //For Sleep
#include <iomanip>  //For UTC time
#include <sstream>  //For UTC time

int tempFromText = 1;
int allTempValuesCounter = 0;
int measurementInterval = 0;
double rawTemp;
double compTemp = 50;
double tempStep = 0.0244200244200244;
std::vector<double> allTempValues;
//std::vector<std::string> timeBefore(10);
std::vector<std::string> timeBefore{ "2017-04-28T15:07:37Z", "2017-04-28T15:09:37Z", "2017-04-28T15:11:37Z", "2017-04-28T15:13:37Z", "2017-04-28T15:15:37Z", "2017-04-28T15:17:37Z", "2017-04-28T15:19:37Z", "2017-04-28T15:21:37Z", "2017-04-28T15:23:37Z", "2017-04-28T15:25:37Z"};
//std::vector<std::string> timeAfter(10);
std::vector<std::string> timeAfter{ "2017-04-28T15:09:37Z", "2017-04-28T15:11:37Z", "2017-04-28T15:13:37Z", "2017-04-28T15:15:37Z", "2017-04-28T15:17:37Z", "2017-04-28T15:19:37Z", "2017-04-28T15:21:37Z", "2017-04-28T15:23:37Z", "2017-04-28T15:25:37Z", "2017-04-28T15:27:37Z" };
std::vector<double> minTempValues(10);
std::vector<double> maxTempValues(10);
std::vector<double> avgTempValues(10);

std::string now = "2017-04-28T15:07:37Z";
std::string after = "2017-04-28T15:09:37Z";




// TemperatureMeasurement
/*{
    "time": {
        "start": string, // Start date and time in ISO8601 format for the measurement
            "end" : string // End date and time in ISO8601 format for the measurement
    },
        "min" : number, // Minimum observed temperature
            "max" : number, // Maximum observed temperature
            "average" : number // Average temperature
}
*/

/*
std::string currentISO8601TimeUTC() {
    auto now = std::chrono::system_clock::now();
    auto itt = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(gmtime_s(&t, &buf), "%FT%TZ");
    return ss.str();
}
*/

double GetTemperature() {
    std::ifstream file("C:\\recruitment\\temperature-sensor\\temperature.txt");
    std::string str;
    for (int i = 0; i < tempFromText; ++i) {
        std::getline(file, str);
        rawTemp = std::stod(str);
        //std::cout << str << "\n";
        //std::this_thread::sleep_for(std::chrono::milliseconds(200000));
    }
    tempFromText++;
    if (tempFromText == 768) {
        tempFromText = 0;
    }
    double realTemp = (rawTemp * tempStep) - compTemp;
    return realTemp;
    //4095 is max temp at 3.3v max temp is 50C min is -50C
}


void TemperatureBuffer(double latestTemperatureReading) {
    allTempValues.push_back(latestTemperatureReading);
    //std::cout << allTempValuesCounter << "\n";
    if (allTempValuesCounter == 0) {
        //timeBefore[measurementInterval] = std::put_time(std::localtime(&t), "%FT%T%z");

        //timeBefore[measurementInterval] = now;

        //timeBefore.push_back (now);
    }
    allTempValuesCounter++;
    //std::cout << latestTemperatureReading << "\n";



    if (allTempValuesCounter == 1199) {
        //Latest.Max = *std::max_element(AllTempValues.begin(), AllTempValues.end());
        //std::cout << "Maximum value is" << "\n";
        //std::cout << Latest.Max << "\n";
        auto minmax = minmax_element(allTempValues.begin(), allTempValues.end());
        minTempValues[measurementInterval] = *minmax.first;
        //minTempValues.push_back (*minmax.first);
        maxTempValues[measurementInterval] = *minmax.second;
        //maxTempValues.push_back (*minmax.second);
        avgTempValues[measurementInterval] = accumulate(allTempValues.begin(), allTempValues.end(), 0.0) / allTempValues.size();
        //avgTempValues.push_back (accumulate(allTempValues.begin(), allTempValues.end(), 0.0) / allTempValues.size());
        //timeAfter[measurementInterval] = after;
        //timeAfter.push_back(after);
        /*
        std::cout << "measurement nr " << measurementInterval << "\n";
        std::cout << "Time before " << timeBefore.at(measurementInterval);
        std::cout << "Time after " << timeAfter.at(measurementInterval);
        std::cout << "min is " << minTempValues.at(measurementInterval);
        std::cout << "max is " << maxTempValues.at(measurementInterval);
        std::cout << "average is " << avgTempValues.at(measurementInterval);
        */
        std::cout << "measurement nr " << "0" << "\n";
        std::cout << "Time before " << timeBefore.at(0);
        std::cout << "Time after " << timeAfter.at(0);
        std::cout << "min is " << minTempValues.at(0);
        std::cout << "max is " << maxTempValues.at(0);
        std::cout << "average is " << avgTempValues.at(0);
        //
        std::cout << "measurement nr " << "1" << "\n";
        std::cout << "Time before " << timeBefore.at(1);
        std::cout << "Time after " << timeAfter.at(1);
        std::cout << "min is " << minTempValues.at(1);
        std::cout << "max is " << maxTempValues.at(1);
        std::cout << "average is " << avgTempValues.at(1);
        //
        std::cout << "measurement nr " << "2" << "\n";
        std::cout << "Time before " << timeBefore.at(2);
        std::cout << "Time after " << timeAfter.at(2);
        std::cout << "min is " << minTempValues.at(2);
        std::cout << "max is " << maxTempValues.at(2);
        std::cout << "average is " << avgTempValues.at(2);
        //
        std::cout << "measurement nr " << "3" << "\n";
        std::cout << "Time before " << timeBefore.at(3);
        std::cout << "Time after " << timeAfter.at(3);
        std::cout << "min is " << minTempValues.at(3);
        std::cout << "max is " << maxTempValues.at(3);
        std::cout << "average is " << avgTempValues.at(3);
        //
        std::cout << "measurement nr " << "4" << "\n";
        std::cout << "Time before " << timeBefore.at(4);
        std::cout << "Time after " << timeAfter.at(4);
        std::cout << "min is " << minTempValues.at(4);
        std::cout << "max is " << maxTempValues.at(4);
        std::cout << "average is " << avgTempValues.at(4);
        //
        std::cout << "measurement nr " << "5" << "\n";
        std::cout << "Time before " << timeBefore.at(5);
        std::cout << "Time after " << timeAfter.at(5);
        std::cout << "min is " << minTempValues.at(5);
        std::cout << "max is " << maxTempValues.at(5);
        std::cout << "average is " << avgTempValues.at(5);
        //
        std::cout << "measurement nr " << "6" << "\n";
        std::cout << "Time before " << timeBefore.at(6);
        std::cout << "Time after " << timeAfter.at(6);
        std::cout << "min is " << minTempValues.at(6);
        std::cout << "max is " << maxTempValues.at(6);
        std::cout << "average is " << avgTempValues.at(6);
        //
        std::cout << "measurement nr " << "7" << "\n";
        std::cout << "Time before " << timeBefore.at(7);
        std::cout << "Time after " << timeAfter.at(7);
        std::cout << "min is " << minTempValues.at(7);
        std::cout << "max is " << maxTempValues.at(7);
        std::cout << "average is " << avgTempValues.at(7);
        //
        std::cout << "measurement nr " << "8" << "\n";
        std::cout << "Time before " << timeBefore.at(8);
        std::cout << "Time after " << timeAfter.at(8);
        std::cout << "min is " << minTempValues.at(8);
        std::cout << "max is " << maxTempValues.at(8);
        std::cout << "average is " << avgTempValues.at(8);
        //
        std::cout << "measurement nr " << "9" << "\n";
        std::cout << "Time before " << timeBefore.at(9);
        std::cout << "Time after " << timeAfter.at(9);
        std::cout << "min is " << minTempValues.at(9);
        std::cout << "max is " << maxTempValues.at(9);
        std::cout << "average is " << avgTempValues.at(9);
        //
        allTempValues.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(20000));

        //http send
        measurementInterval++;
        if (measurementInterval == 9) {
            measurementInterval = 0;
        }
        allTempValuesCounter = 0;
    }

}







int main() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));    //Wait 100ms between temperature readings
        double latestTemperatureReading = GetTemperature();
        TemperatureBuffer(latestTemperatureReading);
        //std::cout << latestTemperatureReading << "\n";

    }
}

