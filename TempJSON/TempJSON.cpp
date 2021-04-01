// TempJSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> 
#include <fstream>
#include <string> 
#include <chrono>   //For Sleep
#include <thread>   //For Sleep

int tempFromText = 0;
double rawTemp;
double startingTemp = 50;
double tempStep = 0.0244200244200244;

struct Temperature {
    double min;
    double max;
    double avg;
};

double GetTemperature() {
    std::ifstream file("C:\\recruitment\\temperature-sensor\\temperature.txt");
    std::string str;

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




int main() {

    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));    //Wait 100ms between temperature readings
        double latestTemperatureReading = GetTemperature();
        std::cout << latestTemperatureReading << "\n";
        tempFromText++;
        if (tempFromText == 768) {
            tempFromText = 0;
        }
    }
}


