#include "app.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <fmt/chrono.h>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <ctime>

// Color marks
std::string successColor = "\033[32m";
std::string failColor = "\033[31m";
std::string normalColor = "\033[0m";

bool checkData(std::string const& option, std::string const& path) {
    std::cout<<"Check if data is correct:"<<std::endl;
    std::cout<<"Flag: "<<option<<std::endl;
    std::cout<<"Path: "<<path<<std::endl;

    std::cout<<"If data is correct type 'correct' if not 'return': ";
    std::string answer;
    std::getline(std::cin, answer);

    if(answer == "correct") {
        return true;
    } else if(answer == "return") {
        return false;
    } else {
        std::cout<<failColor<<"Typo or wrong commend, please repeat if needed"<<normalColor<<std::endl;
        return false;
    }
}

bool checkData(std::string const& option, std::string const& path, std::string const& message) {
    std::cout<<"Check if data is correct:"<<std::endl;
    std::cout<<"Flag: "<<option<<std::endl;
    std::cout<<"Path: "<<path<<std::endl;
    std::cout<<"Massage: "<<message<<std::endl;

    std::cout<<"If data is correct type 'correct' if not 'return': ";
    std::string answer;
    std::getline(std::cin, answer);

    if(answer == "correct") {
        return true;
    } else if(answer == "return") {
        return false;
    } else {
        std::cout<<failColor<<"Typo or wrong commend, please repeat if needed"<<normalColor<<std::endl;
        return false;
    }
}

bool isSupported(std::string const& path) {
    // App supports only .png and .bmp extensions
    if(std::filesystem::exists(path)) {
        auto fileFormat = path.substr(path.length()-4,path.length()-1);
        if(fileFormat == ".png" || fileFormat == ".bmp") {
            return true;
        } else {
            std::cout<<failColor<<fileFormat<<" is not supported, only .png and .bmp"<<normalColor<<std::endl;
            return false;
        }
    } else {
        std::cout<<failColor<<"Wrong path or wrong path format (ex. file extension is missing)"<<normalColor<<std::endl;
        std::cout<<"Path format example: \n"<<std::endl;
        std::cout<<"-i /Users/John/Desktop/dog.png"<<std::endl;
        return false;
    }

}

void info(std::string const& path) {

    sf::Image image;
    if (!image.loadFromFile(path)) {
        std::cout<<failColor<<"Failed to load image: "<<path<<normalColor<<std::endl;
        return;
    }

    std::cout<<successColor<<"Successfully loaded image, info below:"<<normalColor<<std::endl;

    // Image size
    sf::Vector2u size = image.getSize();
    std::cout<<"Image width: "<<size.x<<" pixels\n";
    std::cout<<"Image height: "<<size.y<<" pixels\n";

    // File size
    auto fileSize = std::filesystem::file_size(path);
    std::cout<<"File size: "<<fileSize<<" bytes\n";

    // Last modification
    auto fileTime = std::chrono::file_clock::to_sys(std::filesystem::last_write_time(path));
    auto clockTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(fileTime);
    std::time_t calenderTime = std::chrono::system_clock::to_time_t(clockTime);
    auto local_time = *std::localtime(&calenderTime);
    fmt::println("Last modified: {:%Y-%m-%d %H:%M:%S}", local_time);

}

void encrypt(std::string const& path, std::string const& message) {
    sf::Image image;
    if (!image.loadFromFile(path)) {
        std::cout<<failColor<<"Failed to load image: "<<path<<normalColor<<std::endl;
        return;
    }

    std::string binaryMessage;
    for (char c : message) {
        binaryMessage += std::bitset<8>(c).to_string();
    }

    // End message mark
    binaryMessage += std::bitset<8>(';').to_string();

    // Size check
    size_t requiredPixels = binaryMessage.length() / 3;
    if (binaryMessage.length() % 3 != 0) {
        requiredPixels++;
    }

    if (image.getSize().x * image.getSize().y < requiredPixels) {
        std::cout << failColor <<"Image is too small to store the message." << normalColor <<std::endl;
        return;
    }

    size_t bitIndex = 0;
    for (unsigned int y = 0; y < image.getSize().y; ++y) {
        for (unsigned int x = 0; x < image.getSize().x; ++x) {
            if (bitIndex >= binaryMessage.length()) {
                break;
            }

            sf::Color pixelColor = image.getPixel(x, y);

            // Modification of the least significant bit of each color
            if (bitIndex < binaryMessage.length()) {
                pixelColor.r = (pixelColor.r & 0xFE) | (binaryMessage[bitIndex] - '0');
                bitIndex++;
            }
            if (bitIndex < binaryMessage.length()) {
                pixelColor.g = (pixelColor.g & 0xFE) | (binaryMessage[bitIndex] - '0');
                bitIndex++;
            }
            if (bitIndex < binaryMessage.length()) {
                pixelColor.b = (pixelColor.b & 0xFE) | (binaryMessage[bitIndex] - '0');
                bitIndex++;
            }

            image.setPixel(x, y, pixelColor);
        }
        if (bitIndex >= binaryMessage.length()) {
            break;
        }
    }

    // Checking if directory 'encryptedPictures' exists. If not create it
    std::filesystem::path dir = std::filesystem::current_path() / "encryptedPictures";
    if (!std::filesystem::exists(dir)) {
        std::cout << "Folder 'encryptedPictures' does not exist. Creating it..." << std::endl;
        std::filesystem::create_directory(dir);
    }

    // Creating path for encrypted picture
    std::filesystem::path filePath(path);
    std::string fileName = filePath.stem().string();
    std::string fileExtension = filePath.extension().string();
    std::string encryptedFileName = fileName + "_encrypted" + fileExtension;
    std::filesystem::path encryptedFilePath = dir / encryptedFileName;

    // Saving picture in 'encryptedPictures'
    if (!image.saveToFile(encryptedFilePath.string())) {
        std::cout << failColor <<"Failed to save image." << normalColor <<std::endl;
    } else {
        std::cout << successColor << "Message encrypted and saved to: " << normalColor <<encryptedFilePath << std::endl;
    }
}

std::string decrypt(std::string const& path) {
    sf::Image image;
    if (!image.loadFromFile(path)) {
        std::cout<<failColor<<"Failed to load image: "<<path<<normalColor<<std::endl;
        return "Failed to load [400]";
    }

    std::string binaryMessage;
    for (unsigned int y = 0; y < image.getSize().y; ++y) {
        for (unsigned int x = 0; x < image.getSize().x; ++x) {
            sf::Color pixelColor = image.getPixel(x, y);

            // Reading least significant bits
            binaryMessage += (pixelColor.r & 0x01) ? '1' : '0';
            binaryMessage += (pixelColor.g & 0x01) ? '1' : '0';
            binaryMessage += (pixelColor.b & 0x01) ? '1' : '0';
        }
    }

    // Creating message
    std::string message;
    for (size_t i = 0; i < binaryMessage.length(); i += 8) {
        std::bitset<8> byte(binaryMessage.substr(i, 8));
        char c = static_cast<char>(byte.to_ulong());

        if (c == ';') {
            break;
        }

        message += c;
    }

    // std::cout << "Decrypted message: " << message << std::endl;
    return message;
}

void check(std::string const& path, std::string const& message) {
    bool messageFound = false;
    if(message == decrypt(path)) {
        messageFound = true;
        std::cout<<successColor<<"Message: '"<<message<<"', was found in the file"<<normalColor<<std::endl;
    } else {
        std::cout<< failColor <<"Message: '"<<message<<"', was not found in the file"<< normalColor <<std::endl;
    }

    if(!messageFound) {
        sf::Image image;
        if (!image.loadFromFile(path)) {
            std::cout<<failColor<<"Failed to load image: "<<path<<normalColor<<std::endl;
            return;
        }

        std::string binaryMessage;
        for (char c : message) {
            binaryMessage += std::bitset<8>(c).to_string();
        }

        // End message mark
        binaryMessage += std::bitset<8>(';').to_string();

        // Image size check
        size_t requiredPixels = binaryMessage.length() / 3;
        if (binaryMessage.length() % 3 != 0) {
            requiredPixels++;
        }

        bool bigEnough = true;
        if (image.getSize().x * image.getSize().y < requiredPixels) {
            std::cout << failColor <<"Image is too small to store the message." << normalColor <<std::endl;
            bigEnough = false;
        }

        if(bigEnough) {
            std::cout<<successColor<<"You can hide your massage in this file, would you like to do it?"<<normalColor<<std::endl;
            std::cout<<"Type 'yes' or 'no': ";
            std::string response;
            std::getline(std::cin, response);
            if(response == "yes") {
                encrypt(path,message);
            } else if(response == "no") {
                std::cout<<"If you would like to encrypt message use -e or -encrypt"<<std::endl;
            } else {
                std::cout<<failColor<<"Typo or wrong commend, use -c or -check one more time if needed"<<normalColor<<std::endl;
            }
        }
    }
}

void help() {
    std::cout<<"------ Help menu ------\n";
    std::cout<<"-i (-info): Information about a file\n";
    std::cout<<"Format: <flag> <path>\n";
    std::cout<<"\n";
    std::cout<<"-e (-encrypt): Encrypt message in a file\n";
    std::cout<<"Format: <flag> <path> <Massage>, note: massage must have quotation marks\n";
    std::cout<<"\n";
    std::cout<<"-d (-decrypt): Decrypt message in a file\n";
    std::cout<<"Format: <flag> <path>\n";
    std::cout<<"\n";
    std::cout<<"-c (-check): Check for hidden massage and possibility to hide a massage\n";
    std::cout<<"Format: <flag> <path> <Massage>, note: massage must have quotation marks\n";
    std::cout<<"\n";
    std::cout<<"-h (-help): Help menu\n";
    std::cout<<"Format: <flag>\n";
    std::cout<<"\n";
    std::cout<<"-q (-quit): Close app\n";
    std::cout<<"Format: <flag>\n";
}