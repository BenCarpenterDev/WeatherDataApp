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
    string locationName;
    string latitude;
    string longitude;

    //hourly variables
    string time;
    string temperature;
    string relativeHumidity;
    string precipitation;

    string sealevelPressure;
    string surfacePressure;
    string cloudCover;

    string windSpeed10;
    /*
    std::string windSpeed80;
    std::string windSpeed120;
    std::string windSpeed180;*/
};

struct geoData {
    int geoID;
    string geoName;
    string geoLat;
    string geoLon;
};

void printWeatherData(const std::vector<WeatherData>& dataVector, int index) {
    cout << "\nLocationID: " << dataVector[index].locationID;
    cout << "\nLocation: " << dataVector[index].locationName;
    cout << "\nLatitude: " << dataVector[index].latitude;
    cout << "\nLongitude: " << dataVector[index].longitude;

    cout << "\nTime by Hour: " << dataVector[index].time;
    cout << "\nTemperature: " << dataVector[index].temperature;
    cout << "\nRelative Humidity: " << dataVector[index].relativeHumidity;
    cout << "\nPrecipitation: " << dataVector[index].precipitation;

    cout << "\nSea Level Pressure: " << dataVector[index].sealevelPressure;
    cout << "\nSurface Pressure: " << dataVector[index].surfacePressure;
    cout << "\nCloud Cover: " << dataVector[index].cloudCover;
    cout << "\nWind Speed at 10 metres: " << dataVector[index].windSpeed10;
    cout << "\n \n";
};

vector<string> commands{ "quit", "help", "add", "remove", "modify", "search", "saved"};

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<geoData> getGeoData(string searchStr) {
    CURL* curl;
    CURLcode res;
    string responseBuffer;

    curl = curl_easy_init();
    if (curl) {
        // Set the URL you want to request
        string apiKey = "YOUR_API_KEY"; // Replace with your Open-Meteo API key
        string url = "https://geocoding-api.open-meteo.com/v1/search?name=" + searchStr + "&count=10&language=en&format=json";
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

                string geoID = root["results"][0]["id"].asString();
                int id = stoi(geoID);
                string name = root["results"][0]["name"].asString();
                string lat = root["results"][0]["latitude"].asString();
                string lon = root["results"][0]["longitude"].asString();


                std::vector<geoData> TempVector;

                TempVector.push_back({ id, name, lat, lon });

                return TempVector;
            }
        }
        curl_easy_cleanup(curl);
    }
}

std::vector<WeatherData> getWeatherData(string name) {
    std::vector<geoData> geoVector;
    geoVector = getGeoData(name);

    string latitude = geoVector[0].geoLat;
    string longitude = geoVector[0].geoLon;
    string locationName = geoVector[0].geoName;
    int id = geoVector[0].geoID;


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

                string hourlyTime = root["hourly"]["time"][0].asString();
                string hourlyTemperature = root["hourly"]["temperature_2m"][0].asString();
                string hourlyHumidity = root["hourly"]["relative_humidity_2m"][0].asString();
                string hourlyPrecipitation = root["hourly"]["precipitation"][0].asString();
                string hourlySeaPressure = root["hourly"]["pressure_msl"][0].asString();
                string hourlySurfacePressure = root["hourly"]["surface_pressure"][0].asString();
                string hourlyCloudCover = root["hourly"]["cloud_cover"][0].asString();
                string hourlyWindSpeed = root["hourly"]["wind_speed_10m"][0].asString();

                std::vector<WeatherData> TempVector;

                TempVector.push_back({ id, locationName, weatherLatitude, weatherLongitude, hourlyTime, hourlyTemperature,
                    hourlyHumidity, hourlyPrecipitation, hourlySeaPressure, hourlySurfacePressure, hourlyCloudCover, hourlyWindSpeed });

                return TempVector;
            }
        }
        curl_easy_cleanup(curl);
    }
}



int main() {
    cout << "| WELCOME TO BEN'S WEATHER DATA APPLICATION |";
    cout << "\n-----------------------------------------\n";
    cout << "Input 'help' for commands\n\n";
    

    std::vector<WeatherData> savedDataVector;

    savedDataVector.push_back({ 0, "TestLocation", "50", "76" });
    savedDataVector.push_back({ 1, "TestLocationAnother", "24", "32" });



    bool UserCommand = true;

    while(UserCommand == true)
    {
        string command, input;

        getline(cin, input);
        stringstream ss(input);
        vector<std::string> inputs;

        while (ss >> command) {
            inputs.push_back(command);
        }
        int id = savedDataVector.size() + 1;


        
        if (find(commands.begin(), commands.end(), inputs[0]) != commands.end())
            if (inputs[0] == "help") {
                cout << "Here are help commands\n";
            }

        // show all saved/favourited locations
            else if (inputs[0] == "saved") {
                cout << "\n| SHOWING FAVOURITED LOCATIONS |";
                cout << "\n-----------------------------------------\n";

                for (int i = 0; i < savedDataVector.size(); i++) {
                    printWeatherData(savedDataVector, i);
                }
            }
        // Search and print weather data by longitude and latitude
            else if (inputs[0] == "search") {
                cout << "\n| SEARCH RESULTS |";
                cout << "\n-----------------------------------------\n";
                printWeatherData(getWeatherData(inputs[1]), 0);//search Nottingham
            }

        // save locations
            else if (inputs[0] == "add") {//add Nottingham
                std::vector<WeatherData> temp = getWeatherData(inputs[1]);
                savedDataVector.insert(savedDataVector.end(), temp.begin(), temp.end());
                cout << "\n| "+ inputs[1] +" LOCATION FAVOURITED |";
                cout << "\n-----------------------------------------\n";
            }

        // remove saved locations
            else if (inputs[0] == "remove") {//remove Nottingham

                savedDataVector.erase(remove_if(savedDataVector.begin(), savedDataVector.end(),[&](const WeatherData& data) { return data.locationName == inputs[1]; }),savedDataVector.end());

                cout << "\n| " + inputs[1] + " LOCATION REMOVED |";
                cout << "\n-----------------------------------------\n";
            }

        // quit program
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


