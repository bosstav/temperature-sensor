// TempJSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream> 
#include <fstream>
#include <string> 
#include <vector> 
#include <array>        // std::array
#include <algorithm> // std::min_element, std::max_element
#include <chrono>   //For Sleep
#include <thread>   //For Sleep

int tempFromText = 0;
int allTempValuesCounter = 0;
double rawTemp;
double startingTemp = 50;
double tempStep = 0.0244200244200244;

struct Temperature {
    double min;
    double max;
    double avg;
}Latest;

double GetTemperature() {
    std::ifstream file("C:\\recruitment\\temperature-sensor\\temperature.txt");
    std::string str;
    //double RawTemp;
    for (int i = 0; i < tempFromText; ++i) {
        std::getline(file, str);
        rawTemp = std::stod(str);
        //RawTemp = 3000;
        //std::cout << str << "\n";
    }

    double realTemp = (rawTemp * tempStep) - startingTemp;
    return realTemp;
    //4095 is max temp at 3.3v max temp is 50C min is -50C
}

void TemperatureBuffer(double latestTemperatureReading) {
    std::vector<float> allTempValues;
    allTempValues.push_back(latestTemperatureReading);
    allTempValuesCounter++;
    std::cout << allTempValuesCounter << "\n";
    if (allTempValuesCounter == 1199) {
        //Latest.Max = *max_element(AllTempValues.begin(), AllTempValues.end());
        //std::cout << "Maximum value is" << "\n";
        //std::cout << Latest.Max << "\n";
        auto minmax = minmax_element(allTempValues.begin(), allTempValues.end());
        std::cout << "min is " << *minmax.first;
        std::cout << "max is " << *minmax.second;
        std::this_thread::sleep_for(std::chrono::milliseconds(20000));
        //auto Max = std::max_element(std::begin(AllTempValues), std::end(AllTempValues));
        //Latest.Max = Max;
        //calc avg max min
        //http send
        allTempValuesCounter = 0;
    }

}





int main() {

    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));    //Wait 100ms between temperature readings
        double LatestTemperatureReading = GetTemperature();
        TemperatureBuffer(LatestTemperatureReading);
        std::cout << LatestTemperatureReading << "\n";
        tempFromText++;
        if (tempFromText == 768) {
            tempFromText = 0;
        }
    }
}


