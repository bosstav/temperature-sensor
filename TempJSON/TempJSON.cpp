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
#include <deque>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>


std::deque<int> orderOfReadings;




int tempFromText = 1;
int allTempValuesCounter = 0;
int measurementInterval = 0;
double rawTemp;
double compTemp = 50;
double tempStep = 0.0244200244200244;
double scale = 0.01; //For rounding to two decimal places
std::vector<double> allTempValues;
std::vector<std::string> timeBefore(10);
//std::vector<std::string> timeBefore{ "2017-04-28T15:07:37Z", "2017-04-28T15:09:37Z", "2017-04-28T15:11:37Z", "2017-04-28T15:13:37Z", "2017-04-28T15:15:37Z", "2017-04-28T15:17:37Z", "2017-04-28T15:19:37Z", "2017-04-28T15:21:37Z", "2017-04-28T15:23:37Z", "2017-04-28T15:25:37Z"};
std::vector<std::string> timeAfter(10);
//std::vector<std::string> timeAfter{ "2017-04-28T15:09:37Z", "2017-04-28T15:11:37Z", "2017-04-28T15:13:37Z", "2017-04-28T15:15:37Z", "2017-04-28T15:17:37Z", "2017-04-28T15:19:37Z", "2017-04-28T15:21:37Z", "2017-04-28T15:23:37Z", "2017-04-28T15:25:37Z", "2017-04-28T15:27:37Z" };

std::vector<double> minTempValues(10);
std::vector<double> maxTempValues(10);
std::vector<double> avgTempValues(10);

std::string now = "2017-04-28T15:07:37";
std::string after = "2017-04-28T15:09:37";

void OrderOfTempReadings(int a) {
    orderOfReadings.push_front(a);
    if (orderOfReadings.size() > 10) {
        orderOfReadings.pop_back();
    }
    
    
    std::cout << orderOfReadings.at(0) << "orderOfReadings position 0";
    std::cout << orderOfReadings.size() << "size of queue ";
 
    
}

std::string iso8601() {
    struct tm tmNow;

    time_t now = time(NULL);    // Get the current time
    _localtime64_s(&tmNow, &now);

    char bufferTime[26];

    char bufferTimezoneOffset[6];
    size_t tsizTime = strftime(bufferTime, 26, "%Y-%m-%dT%H:%M:%S", &tmNow);
    size_t tsizOffset = strftime(bufferTimezoneOffset, 6, "%z", &tmNow);
    std::string time = bufferTime;
    return time;
}

void CreateAndSend10LastJSON() {

}

std::string ConvertDoubleIntoString(double temp){
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << temp;
    std::string string = ss.str();
    return string;
}

void CreateAndSendSingleJSON() {
    std::cout << "Creating JSON" << "\n";



    std::cout << "Sending JSON" << "\n";

    /*
    web::json::value TemperatureMeasurement;
    web::json::keep_object_element_order (true);
    TemperatureMeasurement[L"time"][L"start"] = web::json::value::string(utility::conversions::to_utf16string("2017-04-28T15:07:37Z"));
    TemperatureMeasurement[L"time"][L"end"] = web::json::value::string(utility::conversions::to_utf16string("2017-04-28T15:07:37Z"));
    TemperatureMeasurement[L"min"] = web::json::value::string(utility::conversions::to_utf16string("-10.54"));
    TemperatureMeasurement[L"max"] = web::json::value::string(utility::conversions::to_utf16string("10.53"));
    TemperatureMeasurement[L"average"] = web::json::value::string(utility::conversions::to_utf16string("0.01"));
    */
    
    utility::string_t timeBeforestring = utility::conversions::to_string_t(timeBefore[measurementInterval]);
    utility::string_t timeAfterstring = utility::conversions::to_string_t(timeAfter[measurementInterval]);
    
    std::string minTempstring = ConvertDoubleIntoString(minTempValues[measurementInterval]);
    std::string maxTempstring = ConvertDoubleIntoString(maxTempValues[measurementInterval]);
    std::string avgTempstring = ConvertDoubleIntoString(avgTempValues[measurementInterval]);
    utility::string_t minTempstring2 = utility::conversions::to_string_t(minTempstring);
    utility::string_t maxTempstring2 = utility::conversions::to_string_t(maxTempstring);
    utility::string_t avgTempstring2 = utility::conversions::to_string_t(avgTempstring);

    
    web::json::value TemperatureMeasurement;
    web::json::keep_object_element_order(true);

    TemperatureMeasurement[L"time"][L"start"] = web::json::value::string(timeBeforestring); //wont read string from vector
    TemperatureMeasurement[L"time"][L"end"] = web::json::value::string(timeAfterstring);
    TemperatureMeasurement[L"min"] = web::json::value::string(minTempstring2);
    TemperatureMeasurement[L"max"] = web::json::value::string(maxTempstring2);
    TemperatureMeasurement[L"average"] = web::json::value::string(avgTempstring2);
    


    /*
    float tempMin = (floor(minTempValues[measurementInterval] / scale + 0.5) * scale);
    float tempMax = (floor(maxTempValues[measurementInterval] / scale + 0.5) * scale);
    float tempAvg = (floor(avgTempValues[measurementInterval] / scale + 0.5) * scale);
    web::json::value TemperatureMeasurement;
    web::json::keep_object_element_order(true);
    TemperatureMeasurement[L"time"][L"start"] = web::json::value::string(U("2012-08-01T15:44:06Z"));
    TemperatureMeasurement[L"time"][L"end"] = web::json::value::string(U("2012-08-01T15:46:06Z"));
    TemperatureMeasurement[L"min"] = web::json::value::number(tempMin);
    TemperatureMeasurement[L"max"] = web::json::value::number(tempMax);
    TemperatureMeasurement[L"average"] = web::json::value::number(tempAvg);
    */


    //web::http::client::http_client client(L"http://localhost:3000/posts");
   web::http::client::http_client client(L"http://localhost:5000/api/temperature");


    //client.request(web::http::methods::POST, L"", TemperatureMeasurement.serialize().c_str(),
     //   L"application/json").then([](const web::http::http_response& response)

    std::cout << minTempstring << std::endl;
    std::wcout << minTempstring2 << std::endl;
    //std::cout << timeBefore[measurementInterval] << std::endl;
    //std::cout << timeAfter[measurementInterval] << std::endl;
    std::wcout << TemperatureMeasurement << std::endl;

    //client.request(web::http::methods::POST, L"", TemperatureMeasurement.serialize().c_str(),
    client.request(web::http::methods::POST, U("/"), TemperatureMeasurement).then([](const web::http::http_response& response)
            {
                std::wcout << response.status_code() << std::endl;

                if (response.status_code() == 500)
                {
                    printf("Success");
                }
                if (response.status_code() == web::http::status_codes::OK)
                {
                    std::cout << "ok" << "\n";
                    auto body = response.extract_string();

                    std::wcout << body.get().c_str();
                }
                /*
                
                else
                {
                    printf("Nothing");
                }
                */
            });
}


    

        
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

        timeBefore[measurementInterval] = iso8601();

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
        timeAfter[measurementInterval] = iso8601();
        //timeAfter.push_back(after);
        /*
        std::cout << "measurement nr " << measurementInterval << "\n";
        std::cout << "Time before " << timeBefore.at(measurementInterval);
        std::cout << "Time after " << timeAfter.at(measurementInterval);
        std::cout << "min is " << minTempValues.at(measurementInterval);
        std::cout << "max is " << maxTempValues.at(measurementInterval);
        std::cout << "average is " << avgTempValues.at(measurementInterval);
        */
        CreateAndSendSingleJSON();
        /*
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
        */
        allTempValues.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        //http send
        OrderOfTempReadings(measurementInterval);
        measurementInterval++;
        if (measurementInterval == 9) {
            measurementInterval = 0;
        }
        allTempValuesCounter = 0;
    }

}







int main() {
    while (1) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));    //Wait 100ms between temperature readings
        double latestTemperatureReading = GetTemperature();
        TemperatureBuffer(latestTemperatureReading);
        //std::cout << latestTemperatureReading << "\n";

    }
}

