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

int tempFromText = 1;
int allTempValuesCounter = 0;
double rawTemp;
double compTemp = 50;
double tempStep = 0.0244200244200244;
std::vector<double> allTempValues;


struct Temperature {
    double min;
    double max;
    double avg;
}Latest;

double GetTemperature() {
    std::ifstream file("C:\\recruitment\\temperature-sensor\\temperature.txt");
    std::string str;
    for (int i = 0; i < tempFromText; ++i) {
        std::getline(file, str);
        rawTemp = std::stod(str);
        //std::cout << str << "\n";

    }
    tempFromText++;
    if (tempFromText == 768) {
        tempFromText = 0;
    }
    double realTemp = (rawTemp * tempStep) - compTemp;
    return realTemp;
    //4095 is max temp at 3.3v max temp is 50C min is -50C
}

double CompTemperature(double rawTemp) {
    double realTemp = (rawTemp * tempStep) - compTemp;
    return realTemp;
}

void TemperatureBuffer(double latestTemperatureReading) {
    allTempValues.push_back(latestTemperatureReading);
    std::cout << allTempValuesCounter << "\n";
    allTempValuesCounter++;
    std::cout << latestTemperatureReading << "\n";



    if (allTempValuesCounter == 1199) {
        //Latest.Max = *std::max_element(AllTempValues.begin(), AllTempValues.end());
        //std::cout << "Maximum value is" << "\n";
        //std::cout << Latest.Max << "\n";
        auto minmax = minmax_element(allTempValues.begin(), allTempValues.end());
        Latest.min = *minmax.first;
        Latest.max = *minmax.second;
        Latest.avg = accumulate(allTempValues.begin(), allTempValues.end(), 0.0) / allTempValues.size();
        std::cout << "min is " << Latest.min;
        std::cout << "max is " << Latest.max;
        std::cout << "average is " << Latest.avg;
        allTempValues.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(200000));

        //http send
        allTempValuesCounter = 0;
    }

}







int main() {
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));    //Wait 100ms between temperature readings
        double latestTemperatureReading = GetTemperature();
        TemperatureBuffer(latestTemperatureReading);
        //std::cout << LatestTemperatureReading << "\n";

    }
}



