// SDI Coursework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <curl/curl.h>
#include <json/json.h> 
using namespace std;

// Callback function to handle response data
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    string responseBuffer;

    // Initialize libcurl
    curl = curl_easy_init();
    if (curl) {
        // Set the URL you want to request
        string apiKey = "YOUR_API_KEY"; // Replace with your Open-Meteo API key
        string url = "https://api.open-meteo.com/v1/forecast?latitude=52.52&longitude=13.41&current=temperature_2m,wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set callback function to handle the response
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
                string weatherTimezone = root["timezone"].asString();
                string weatherTemperature = root["temperature_2m"].asString();


                cout << "Latitude: " << weatherLatitude << std::endl;
                cout << "Timezone: " << weatherTimezone << std::endl;
                cout << "Temperature: " << weatherTemperature << std::endl;
            }
            else {
                cerr << "Failed to parse JSON response" << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return 1;
    }

    return 0;
}

