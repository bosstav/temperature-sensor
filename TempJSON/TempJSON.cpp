// TempJSON.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream> 
#include <fstream>
#include <string> 
#include <vector> 
#include <array>                    
#include <algorithm>                //For min_element, std::max_element
#include <numeric>                  //For Accumulate
#include <chrono>                   //For Sleep
#include <thread>                   //For Sleep
#include <iomanip>                  //For UTC time
#include <sstream>                  //For UTC time
#include <deque>                    
#include <cpprest/http_client.h>    //JSON
#include <cpprest/filestream.h>     //JSON
#include <cpprest/json.h>           //JSON


std::deque<int> orderOfReadings;




int tempFromText = 1;
int allTempValuesCounter = 0;
int measurementInterval = 0;
double rawTemp;
double compTemp = 50; //Accounting for reading starting at -50
double tempStep = 0.0244200244200244; // 100/4095
double scale = 0.01; //For rounding to two decimal places
std::vector<double> allTempValues;
std::vector<std::string> timeBefore(10);
std::vector<std::string> timeAfter(10);

std::vector<double> minTempValues(10);
std::vector<double> maxTempValues(10);
std::vector<double> avgTempValues(10);
bool sentToAlternative = false;


void OrderOfTempReadings(int a) {
    orderOfReadings.push_front(a);
    if (orderOfReadings.size() > 10) {
        orderOfReadings.pop_back();
    }
    
    /*
    std::cout << "number at pos 0 in deque" << "\n";
    std::cout << orderOfReadings.at(0) << "\n";
    std::cout << "Size of queue" << "\n";
    std::cout << orderOfReadings.size() << "\n";
    */
 
    
}

std::string iso8601() {
    struct tm tmNow;                //Create time struct

    time_t now = time(NULL);        // Get the current time
    _localtime64_s(&tmNow, &now);   //Converts a time_t time value to a tm structure, and corrects for the local time zone

    

    
    char bufferTime[26];
    char bufferTimezoneOffset[6];
    size_t tsizTime = strftime(bufferTime, 26, "%Y-%m-%dT%H:%M:%S", &tmNow);   
    size_t tsizOffset = strftime(bufferTimezoneOffset, 6, "%z", &tmNow);       

    std::string timeStamp = std::string() + bufferTime + bufferTimezoneOffset;  //Adds time and timezoneoffset together
    timeStamp.insert(22, ":");                                                  
    //std::cout << timeStamp;
    return timeStamp;
}

void CreateAndSend10LastJSON() {
    
    //Create JSON array
    web::json::keep_object_element_order(true);

    std::vector<web::json::value> arraytemperature;
    int counter = 0;
    for (int i = 0; i < 10; i++)
    {
        web::json::value tempReading;
        web::json::keep_object_element_order(true);
        tempReading[L"time"][L"start"] = web::json::value::string(utility::conversions::to_string_t(timeBefore[orderOfReadings.at(counter)])); //wont read string from vector
        tempReading[L"time"][L"end"] = web::json::value::string(utility::conversions::to_string_t(timeAfter[orderOfReadings.at(counter)]));
        tempReading[L"min"] = web::json::value::number((floor(minTempValues[orderOfReadings.at(counter)] / scale + 0.5) * scale));
        tempReading[L"max"] = web::json::value::number((floor(minTempValues[orderOfReadings.at(counter)] / scale + 0.5) * scale));
        tempReading[L"average"] = web::json::value::number((floor(minTempValues[orderOfReadings.at(counter)] / scale + 0.5) * scale));
        counter++;
        arraytemperature.push_back(tempReading);
    }

    web::json::value TemperatureMeasurement;
    TemperatureMeasurement = web::json::value::array(arraytemperature);
    std::cout << "Sending ten JSON tempreadings" << "\n";

    //web::http::client::http_client client(L"http://localhost:3000/posts");                    //Test server
    web::http::client::http_client client(L"http://localhost:5000/api/temperature/missing");



     //client.request(web::http::methods::POST, L"", TemperatureMeasurement.serialize().c_str(),
      //   L"application/json").then([](const web::http::http_response& response)

    //std::wcout << TemperatureMeasurement << std::endl;

    client.request(web::http::methods::POST, U("/"), TemperatureMeasurement).then([](const web::http::http_response& response)
            {
                std::wcout << response.status_code() << std::endl;

                if (response.status_code() == 500)
                {
                    sentToAlternative = true;
                    std::cout << "ok" << "\n";
                    auto body = response.extract_string();

                    std::wcout << body.get().c_str();
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

std::string ConvertDoubleIntoString(double temp){ //COnvert doubles to string to only send json with two decimal places
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << temp;
    std::string string = ss.str();
    return string;
}

void CreateAndSendSingleJSON() {
 
    //Creating JSON
    web::json::value TemperatureMeasurement;
    web::json::keep_object_element_order(true);

    TemperatureMeasurement[L"time"][L"start"] = web::json::value::string(utility::conversions::to_string_t(timeBefore[measurementInterval])); //wont read string from vector
    TemperatureMeasurement[L"time"][L"end"] = web::json::value::string(utility::conversions::to_string_t(timeAfter[measurementInterval]));
    TemperatureMeasurement[L"min"] = web::json::value::number((floor(minTempValues[measurementInterval] / scale + 0.5) * scale));
    TemperatureMeasurement[L"max"] = web::json::value::number((floor(minTempValues[measurementInterval] / scale + 0.5) * scale));
    TemperatureMeasurement[L"average"] = web::json::value::number((floor(minTempValues[measurementInterval] / scale + 0.5) * scale));


    std::cout << "Sending single JSON tempreading" << "\n";

    //Creates JSON with numbers sent as string to keep two decimal places
    /*
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
    */
    


    //::http::client::http_client client(L"http://localhost:3000/posts");              //Test server
    web::http::client::http_client client(L"http://localhost:5000/api/temperature");


    //client.request(web::http::methods::POST, L"", TemperatureMeasurement.serialize().c_str(),
     //   L"application/json").then([](const web::http::http_response& response)

    //std::wcout << TemperatureMeasurement << std::endl;

    client.request(web::http::methods::POST, U("/"), TemperatureMeasurement).then([](const web::http::http_response& response)  //Sends JSON
            {
                std::wcout << response.status_code() << std::endl;

                if (response.status_code() == 500)
                {
                    sentToAlternative = true;
                    std::cout << "ok" << "\n";
                    auto body = response.extract_string();

                    std::wcout << body.get().c_str();
                }
                if (response.status_code() == web::http::status_codes::OK)
                {
                    std::cout << "ok" << "\n";
                    auto body = response.extract_string();

                    std::wcout << body.get().c_str();
                }
            });
}


    

        
double GetTemperature() {   //Gets one reading from temperature text file and returns it value processed into real temp
    std::ifstream file("C:\\recruitment\\temperature-sensor\\temperature.txt");
    std::string str;
    for (int i = 0; i < tempFromText; ++i) {
        std::getline(file, str);
        rawTemp = std::stod(str);
        //std::cout << str << "\n";
        //std::this_thread::sleep_for(std::chrono::milliseconds(200000));
    }
    tempFromText++;
    if (tempFromText == 768) {      //loops readings
        tempFromText = 0;
    }
    double realTemp = (rawTemp * tempStep) - compTemp;  //Adjust temp reading to real value
    return realTemp;
    
}


void TemperatureBuffer(double latestTemperatureReading) {   //Keeps all readings to get values over two minute interval
    allTempValues.push_back(latestTemperatureReading);      //Adds latest reading into a vector
    if (allTempValuesCounter == 0) {                        //Stores start time for measurement interval
        

        timeBefore[measurementInterval] = iso8601();

        
    }
    allTempValuesCounter++;
    



    if (allTempValuesCounter == 1199) {                     //Calculates min, max and average temp over two minute interval
        auto minmax = minmax_element(allTempValues.begin(), allTempValues.end());
        minTempValues[measurementInterval] = *minmax.first;
        maxTempValues[measurementInterval] = *minmax.second;
        avgTempValues[measurementInterval] = accumulate(allTempValues.begin(), allTempValues.end(), 0.0) / allTempValues.size();
        timeAfter[measurementInterval] = iso8601();         //Stores end time for measurement interval
        if (sentToAlternative == false) {                   //Decides where to send readings based on latest http response
            CreateAndSendSingleJSON();
        }
        else {
            CreateAndSend10LastJSON();
            sentToAlternative = false;
        }
        
        allTempValues.clear();

        OrderOfTempReadings(measurementInterval);           //Stores order of reading for fallback JSON
        measurementInterval++;
        if (measurementInterval == 10) {                    //Goes back to start of list of measurement interval log kept for fallback JSON
            sentToAlternative = true; //For testing purposes
            //std::this_thread::sleep_for(std::chrono::milliseconds(2000)); //For testing purposes
            measurementInterval = 0;
        }
        allTempValuesCounter = 0;
    }

}







int main() {
    while (1) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));    //Wait 100ms between temperature readings
        double latestTemperatureReading = GetTemperature();                 //Gets temperaturereading                 
        TemperatureBuffer(latestTemperatureReading);                        //Sends temperaturereading to buffer
        //std::cout << latestTemperatureReading << "\n";

    }
}

