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

class WeatherData {
public:
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

    //additional variables
    string uv;
    string uvClear;
    string isDay;

    //solar radiation variables
    string shortwave;
    string directwave;
    string diffusewave;

    //pressure level variables
    string pressTemp;
    string pressHumid;
    string pressCloud;

    //daily weather variables
    string sunrise;
    string sunset;

    //Settings
    string setTemp;
    string setWind;
    string setZone;

    WeatherData(int id, string name, string weatherLatitude, string weatherLongitude,
        string hourlyTime, string hourlyTemperature, string hourlyHumidity, string hourlyPrecipitation,
        string hourlySeaPressure, string hourlySurfacePressure, string hourlyCloudCover, string hourlyWindSpeed,
        string adUV, string adUVclear, string adisDay,
        string srShort, string srDirect, string srDiff,
        string prTemp, string prHumid, string prCloud,
        string daSunrise, string daSunset,
        string setTemp, string setWind, string setZone)

        : locationID(id), locationName(name), latitude(weatherLatitude), longitude(weatherLongitude),
        time(hourlyTime), temperature(hourlyTemperature), relativeHumidity(hourlyHumidity),
        precipitation(hourlyPrecipitation), sealevelPressure(hourlySeaPressure),
        surfacePressure(hourlySurfacePressure), cloudCover(hourlyCloudCover),
        windSpeed10(hourlyWindSpeed), uv(adUV), uvClear(adUVclear), isDay(adisDay),
        shortwave(srShort), directwave(srDirect), diffusewave(srDiff),
        pressTemp(prTemp), pressHumid(prHumid), pressCloud(prCloud),
        sunrise(daSunrise), sunset(daSunset),
        setTemp(setTemp), setWind(setWind), setZone(setZone) {}
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

    cout << "\n\n| Hourly Data |";
    cout << "\nTemperature: " << dataVector[index].temperature;
    cout << "\nRelative Humidity: " << dataVector[index].relativeHumidity;
    cout << "\nPrecipitation: " << dataVector[index].precipitation;
    cout << "\nSea Level Pressure: " << dataVector[index].sealevelPressure;
    cout << "\nSurface Pressure: " << dataVector[index].surfacePressure;
    cout << "\nCloud Cover: " << dataVector[index].cloudCover;
    cout << "\nWind Speed at 10 metres: " << dataVector[index].windSpeed10;

    cout << "\n\n| Additional Data |";
    cout << "\nUV Index: " << dataVector[index].uv;
    cout << "\nUV Index Clear Sky: " << dataVector[index].uvClear;
    cout << "\nDay or Night: " << dataVector[index].isDay;

    cout << "\n\n| Solar Radiation Data |";
    cout << "\nShortwave: " << dataVector[index].shortwave;
    cout << "\nDirect: " << dataVector[index].directwave;
    cout << "\nDiffuse: " << dataVector[index].diffusewave;

    cout << "\n\n| Pressure Level Data (500hPa)|";
    cout << "\nTemperature: " << dataVector[index].pressTemp;
    cout << "\nRelative Humidity: " << dataVector[index].pressHumid;
    cout << "\nCloud Cover: " << dataVector[index].pressCloud;

    cout << "\n\n| Daily Weather Data |";
    cout << "\nSunrise: " << dataVector[index].sunrise;
    cout << "\nSunset: " << dataVector[index].sunset;

    cout << "\n\n| Current Units |";
    cout << "\nTemperature: " << dataVector[index].setTemp;
    cout << "\nWind Speed: " << dataVector[index].setWind;
    cout << "\nTime Zone: " << dataVector[index].setZone;
    cout << "\n-----------------------------------------\n";

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
        string url = "https://api.open-meteo.com/v1/forecast?latitude="+latitude+"&longitude="+longitude+"&hourly=temperature_2m,relative_humidity_2m,precipitation,pressure_msl,surface_pressure,cloud_cover,wind_speed_10m,shortwave_radiation,direct_radiation,diffuse_radiation,uv_index,uv_index_clear_sky,is_day,temperature_500hPa,relative_humidity_500hPa,cloud_cover_500hPa&daily=sunrise,sunset,rain_sum&forecast_days=1&models=best_match,gfs_global";
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
                int timeIndex = 14;//2pm, can change time inteval with this integer

                string weatherLatitude = root["latitude"].asString();
                string weatherLongitude = root["longitude"].asString();
                string hourlyTime = root["hourly"]["time"][timeIndex].asString();

                //Hourly Data
                string hourlyTemperature = root["hourly"]["temperature_2m_best_match"][timeIndex].asString();
                string hourlyHumidity = root["hourly"]["relative_humidity_2m_best_match"][timeIndex].asString();
                string hourlyPrecipitation = root["hourly"]["precipitation_best_match"][timeIndex].asString();
                string hourlySeaPressure = root["hourly"]["pressure_msl_best_match"][timeIndex].asString();
                string hourlySurfacePressure = root["hourly"]["surface_pressure_best_match"][timeIndex].asString();
                string hourlyCloudCover = root["hourly"]["cloud_cover_best_match"][timeIndex].asString();
                string hourlyWindSpeed = root["hourly"]["wind_speed_10m_best_match"][timeIndex].asString();

                //Additional
                string adUV = root["hourly"]["uv_index_best_match"][timeIndex].asString();
                string adUVclear = root["hourly"]["uv_index_clear_sky_best_match"][timeIndex].asString();
                string adisDay = root["hourly"]["is_day_best_match"][timeIndex].asString();
                
                //Solar Radiation
                string srShort = root["hourly"]["shortwave_radiation_best_match"][timeIndex].asString();
                string srDirect = root["hourly"]["direct_radiation_best_match"][timeIndex].asString();
                string srDiff = root["hourly"]["diffuse_radiation_best_match"][timeIndex].asString();

                //Pressure Level
                string prTemp = root["hourly"]["temperature_500hPa_best_match"][timeIndex].asString();
                string prHumid = root["hourly"]["relative_humidity_500hPa_best_match"][timeIndex].asString();
                string prCloud = root["hourly"]["cloud_cover_500hPa_best_match"][timeIndex].asString();

                //Daily Weather
                string daSunrise = root["daily"]["sunrise_best_match"][0].asString();
                string daSunset = root["daily"]["sunset_best_match"][0].asString();

                //Settings
                string setTemp = root["hourly_units"]["temperature_2m_best_match"].asString();
                string setWind = root["hourly_units"]["wind_speed_10m_best_match"].asString();
                string setZone = root["timezone"].asString();

                std::vector<WeatherData> TempVector;

                TempVector.push_back({ 
                    id, locationName, weatherLatitude, weatherLongitude, hourlyTime, hourlyTemperature,
                    hourlyHumidity, hourlyPrecipitation, hourlySeaPressure, hourlySurfacePressure, hourlyCloudCover, hourlyWindSpeed,
                    adUV, adUVclear, adisDay, srShort, srDirect, srDiff, prTemp, prHumid, prCloud, daSunrise, daSunset,
                    setTemp, setWind, setZone
                    });

                return TempVector;
            }
        }
        curl_easy_cleanup(curl);
    }
}



int main() {
    cout << "| WELCOME TO BEN'S WEATHER DATA APPLICATION |";
    cout << "\n-----------------------------------------\n";
    cout << "\nWeather model is set to Best Match";
    cout << "\nEnter 'help' for commands\n\n";
    

    std::vector<WeatherData> savedDataVector;

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
                cout << "\n| SHOWING AVAILABLE COMMANDS |";
                cout << "\n-----------------------------------------\n";
                cout << "'help' - shows all commands\n";
                cout << "'saved' - shows saved locations\n";
                cout << "'search <name>' - searches for a location and displays result\n";
                cout << "example: search Nottingham\n";
                cout << "'add <name>' - saves chosen location\n";
                cout << "example: 'add Nottingham'\n";
                cout << "'remove <name>' - removes chosen location from saved\n";
                cout << "example: 'remove Nottingham'\n";
                cout << "'modify <locationID> <new value>' - modifies data of chosen location\n";
                cout << "example: 'modify 2641170 locationName ChangedNottingham'\n";
                cout << "'quit' - exits application\n";
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
                cout << "\n-----------------------------------------";
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

        // modify saved locations
            else if (inputs[0] == "modify") {//change Nottingham
                string originalName;
                for (auto& data : savedDataVector) {
                    if (data.locationID == stoi(inputs[1])) {
                        originalName = data.locationName;


                        if (inputs[2] == "locationName") {
                            data.locationName = inputs[3];
                        }
                        else if (inputs[2] == "latitude") {
                            data.latitude = inputs[3];
                        }
                        else if (inputs[2] == "longitude") {
                            data.longitude = inputs[3];
                        }
                        else if (inputs[2] == "time") {
                            data.time = inputs[3];
                        }
                        else if (inputs[2] == "temperature") {
                            data.temperature = inputs[3];
                        }
                        else if (inputs[2] == "relativeHumidity") {
                            data.relativeHumidity = inputs[3];
                        }
                        else if (inputs[2] == "precipitation") {
                            data.precipitation = inputs[3];
                        }
                        else if (inputs[2] == "sealevelPressure") {
                            data.sealevelPressure = inputs[3];
                        }
                        else if (inputs[2] == "surfacePressure") {
                            data.surfacePressure = inputs[3];
                        }
                        else if (inputs[2] == "cloudCover") {
                            data.cloudCover = inputs[3];
                        }
                        else if (inputs[2] == "windSpeed") {
                            data.windSpeed10 = inputs[3];
                        }
                        else if (inputs[2] == "uv") {
                            data.uv = inputs[3];
                        }
                        else if (inputs[2] == "uvClear") {
                            data.uvClear = inputs[3];
                        }
                        else if (inputs[2] == "isDay") {
                            data.isDay = inputs[3];
                        }
                        else if (inputs[2] == "shortwave") {
                            data.shortwave = inputs[3];
                        }
                        else if (inputs[2] == "directwave") {
                            data.directwave = inputs[3];
                        }
                        else if (inputs[2] == "diffusewave") {
                            data.diffusewave = inputs[3];
                        }
                        else if (inputs[2] == "pressTemp") {
                            data.pressTemp = inputs[3];
                        }
                        else if (inputs[2] == "pressHumid") {
                            data.pressHumid = inputs[3];
                        }
                        else if (inputs[2] == "pressCloud") {
                            data.pressCloud = inputs[3];
                        }
                        else if (inputs[2] == "sunrise") {
                            data.sunrise = inputs[3];
                        }
                        else if (inputs[2] == "sunset") {
                            data.sunset = inputs[3];
                        }
                        else if (inputs[2] == "setTemp") {
                            data.setTemp = inputs[3];
                        }
                        else if (inputs[2] == "setWind") {
                            data.setWind = inputs[3];
                        }
                        else if (inputs[2] == "setZone") {
                            data.setZone = inputs[3];
                        }
                        else {
                            std::cout << "Invalid member." << std::endl;
                        }


                        break;
                    }
                }
                
                cout << originalName+"'s "+inputs[2] + " changed to " + inputs[3];
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
}


