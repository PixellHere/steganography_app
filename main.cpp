#include <iostream>
#include <thread>
#include <string>

#include "app.h"

enum class flags {
    i,e,d,c,h,q,def
};

auto resolveFlag(std::string const& flag) -> flags {
    if(flag == "-i" || flag == "-info") {
        return flags::i;
    } else if(flag == "-e" || flag == "-encrypt") {
        return flags::e;
    } else if(flag == "-d" || flag == "-decrypt") {
        return flags::d;
    } else if(flag == "-c" || flag == "-check") {
        return flags::c;
    } else if(flag == "-q" || flag == "-quit") {
        return flags::q;
    } else if(flag == "-h" || flag == "-help"){
        return flags::h;
    } else {
        return flags::def;
    }

}

void clearConsole() {
    std::cout << "\033[2J\033[1;1H";
}

int main() {
    std::cout<<"Welcome to the steganography app\n";
    std::cout<<"Made by Kacper Płachetka\n";
    std::cout<<"The app will start in 3 seconds\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    clearConsole();

    // Color marks
    std::string successColor = "\033[32m";
    std::string failColor = "\033[31m";
    std::string normalColor = "\033[0m";

    bool exit = false;
    while(!exit) {
        std::cout<<"---- Type operation ----\n";
        std::string input;
        std::getline(std::cin, input);

        std::string operation;
        std::string path;
        std::string massage;

        int nextArgument = 0;
        bool isMassage = false;
        for(int i = 0; i < input.length(); i++) {
            if(nextArgument == 0) {
                if(input[i] == ' ') {
                    nextArgument++;
                } else
                    operation += input[i];
            } else if(nextArgument == 1) {
                if(input[i] == ' ') {
                    nextArgument++;
                } else
                    path += input[i];
            } else if(nextArgument == 2) {
                if(input[i] == '"') {
                    if(!isMassage) {
                        isMassage = true;
                    } else {
                        if(!input.substr(i + 1).empty())
                            std::cout << "Ignoring rest of input: '" << input.substr(i + 1) << "'" << std::endl;
                        break;
                    }
                }else
                    massage += input[i];
            }
        }

        if(operation.length() < 2) {
            operation = "def";
        }

        switch (resolveFlag(operation)) {
            case flags::i: {
                if(checkData(operation,path)) {
                    if(isSupported(path)) {
                        info(path);
                    }
                }

                break;
            }
            case flags::e: {
                if(checkData(operation,path,massage)) {
                    if(isSupported(path)) {
                        encrypt(path,massage);
                    }
                }
                break;
            }
            case flags::d: {
                if(checkData(operation,path)) {
                    if(isSupported(path)) {
                        if(decrypt(path) != "Failed to load [400]")
                            std::cout << successColor << "Success, decrypted message: " << normalColor << decrypt(path) << std::endl;
                    }
                }
                break;
            }
            case flags::c: {
                if(checkData(operation,path,massage)) {
                    if(isSupported(path)) {
                        check(path,massage);
                    }
                }
                break;
            }
            case flags::h: {
                help();
                break;
            }
            case flags::q: {
                std::cout<<"Closing program!";
                exit = true;
                break;
            }
            default: {
                std::cout<<failColor<<"Unrecognized flag"<<normalColor<<std::endl;
                std::cout<<"Opening help menu"<<std::endl;
                help();
            }
        }

    }

    return 0;
}
