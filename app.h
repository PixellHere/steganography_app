#pragma once
#include <string>

bool checkData(std::string const& option, std::string const& path);
bool checkData(std::string const& option, std::string const& path, std::string const& message);
bool isSupported(std::string const& path);
void info(std::string const& path);
void encrypt(std::string const& path, std::string const& message);
std::string decrypt(std::string const& path);
void check(std::string const& path, std::string const& message);
void help();