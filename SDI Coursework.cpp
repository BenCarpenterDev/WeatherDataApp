// SDI Coursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include<json/json.h>
#include<curl/curl.h>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

struct WeatherData {// for storing weatherdata in a vector
    int locationID;
    std::string locationName;
    std::string latitude;
    std::string longitude;

    //hourly variables
    std::string temperature;
    std::string relativeHumidity;
    std::string precipitation;

    std::string sealevelPressure;
    std::string surfacePressure;
    std::string cloudCover;

    std::string windSpeed10;
    /*
    std::string windSpeed80;
    std::string windSpeed120;
    std::string windSpeed180;*/
};

void printWeatherData(const std::vector<WeatherData>& dataVector, int index) {
    cout << "\nLocationID: " << dataVector[index].locationID;
    cout << "\nLocation: " << dataVector[index].locationName;
    cout << "\nLatitude: " << dataVector[index].latitude;
    cout << "\nLongitude: " << dataVector[index].longitude;

    cout << "\nTemperature: " << dataVector[index].temperature;
    cout << "\nRelative Humidity: " << dataVector[index].relativeHumidity;
    cout << "\nPrecipitation: " << dataVector[index].precipitation;

    cout << "\nSea Level Pressure: " << dataVector[index].sealevelPressure;
    cout << "\nSurface Pressure: " << dataVector[index].surfacePressure;
    cout << "\nCloud Cover: " << dataVector[index].cloudCover;
    cout << "\nWind Speed at 10 metres: " << dataVector[index].windSpeed10;
    cout << "\n";
};

vector<string> commands{ "quit", "help", "addLocation", "removeLocation", "modifyLocation", "searchLocation", "favouriteLocation", "ShowAll", "TestAPI"};

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<WeatherData> getWeatherData(string latitude, string longitude) {
    CURL* curl;
    CURLcode res;
    string responseBuffer;


    curl = curl_easy_init();
    if (curl) {
        // Set the URL you want to request
        string apiKey = "YOUR_API_KEY"; // Replace with your Open-Meteo API key
        string url = "https://api.open-meteo.com/v1/forecast?latitude="+latitude+"&longitude="+longitude+"&current=temperature_2m&hourly=temperature_2m,relative_humidity_2m,precipitation,pressure_msl,surface_pressure,cloud_cover,wind_speed_10m,wind_speed_80m,wind_speed_120m,wind_speed_180m&forecast_days=1";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBuffer);


        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {

            Json::Value root;
            Json::Reader reader;
            bool parsingSuccessful = reader.parse(responseBuffer, root);
            if (parsingSuccessful) {
                // Weather Attributes

                string weatherLatitude = root["latitude"].asString();
                string weatherLongitude = root["longitude"].asString();

                string hourlyTemperature = root["hourly"]["temperature_2m"][0].asString();
                string hourlyHumidity = root["hourly"]["relative_humidity_2m"][0].asString();
                string hourlyPrecipitation = root["hourly"]["precipitation"][0].asString();
                string hourlySeaPressure = root["hourly"]["pressure_msl"][0].asString();
                string hourlySurfacePressure = root["hourly"]["surface_pressure"][0].asString();
                string hourlyCloudCover = root["hourly"]["cloud_cover"][0].asString();
                string hourlyWindSpeed = root["hourly"]["wind_speed_10m"][0].asString();

                std::vector<WeatherData> TempVector;

                TempVector.push_back({ 2, "Nottingham", weatherLatitude, weatherLongitude, hourlyTemperature,
                    hourlyHumidity, hourlyPrecipitation, hourlySeaPressure, hourlySurfacePressure, hourlyCloudCover, hourlyWindSpeed });

                return TempVector;

            }
            else {
                cerr << "Failed to parse JSON response" << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Failed to initialize libcurl" << std::endl;
    }

}

int main() {
    cout << "WELCOME TO BEN'S WEATHER DATA APPLICATION ";
    cout << "\n-----------------------------------------\n";
    cout << "Input 'help' for commands\n";
    

    std::vector<WeatherData> weatherDataVector;

    weatherDataVector.push_back({ 0, "TestLocation", "50", "76" });
    weatherDataVector.push_back({ 1, "TestLocationAnother", "24", "32" });



    bool UserCommand = true;

    while(UserCommand == true)
    {
        string command;
        cin >> command;

        std::istringstream iss(command);
        string input;
        std::vector<std::string> inputs;

        while (iss >> input) {
            inputs.push_back(input);
        }




        
        if (find(commands.begin(), commands.end(), inputs[0]) != commands.end())
            if (inputs[0] == "help") {
                cout << "Here are help commands\n";
            }
            else if (inputs[0] == "ShowAll") {
                for (int i = 0; i < weatherDataVector.size(); i++) {
                    printWeatherData(weatherDataVector, i);
                }
            }
            else if (inputs[0] == "TestAPI") {
                printWeatherData(getWeatherData("52.52", "13.419998"), 0);//TestAPI 52.52 13.419998
            }
            else if (inputs[0] == "quit") {
                break;
            }
            else {
                cout << "other commands\n";
            }
        else
            cout << "Command invalid\n";

    
    };


    return 0;

    //location id, location name, latitude, longitude
    
    //Hourly ((temperature, precipitation, pressure, wind, humidity, and cloudiness)
    // solar radiation, pressure levels

}


