#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <assert.h>
#include <sys/stat.h>
#include "StringExt.h"

#ifdef _WIN32
    #include <io.h>
    #include <direct.h>
    #define access   _access_s
    #define stat64   _stat64
    #define mkdir    _mkdir
#else
    #include <unistd.h>
#endif

inline bool DirExists(const std::string& dirname)
{
    struct stat st;
    if (stat(dirname.c_str(), &st) == 0) {
        if (S_ISDIR(st.st_mode)) { return true; }
    }
    return false;
}

inline bool CreateDir(const std::string& dirname)
{
    return (mkdir(dirname.c_str()) == 0) ? true : false;
}

inline bool FileExists(const std::string& filename)
{
    return access(filename.c_str(), 0) == 0;
}

inline long long FileSize(const std::string& filename)
{
    struct stat64 stat_buf;
    int rc = stat64(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

inline void HandleFatalError(std::string emsg)
{
	std::cerr << "ERROR: " << emsg << std::endl;
	exit(EXIT_FAILURE);
}

inline std::vector<std::string> ListFiles(std::string dirname)
{
    std::vector<std::string> result;

    for (const auto & entry : std::filesystem::directory_iterator(dirname))
        result.push_back(entry.path().string());

    return result;
}

inline std::string ReadFileStr(const std::string filename)
{
	std::ifstream sourceFile(filename);
    std::string sourceCode(std::istreambuf_iterator<char>(sourceFile),
                          (std::istreambuf_iterator<char>()));
	return sourceCode;
}

inline std::vector<std::string> ReadFileLines(const std::string filename)
{
    std::ifstream ifs(filename);
    std::vector<std::string> result;

	if (ifs.is_open()) {
		while (!ifs.eof()) {
            std::string line;
            std::getline(ifs, line);
            result.push_back(line);
		}

		ifs.close();
	}

	return result;
}

inline bool LoadConfigFile(const std::string filename, std::unordered_map<std::string,std::string>& str_map)
{
	std::ifstream configFile(filename);
	std::string line, key, data;
	size_t bpos;

	if (configFile.is_open()) {

		while (!configFile.eof()) {
			std::getline(configFile, line);
			if (line.empty() || line[0] == '#') continue;
			bpos = line.find("=");
			if (bpos == std::string::npos) continue;
			key = line.substr(0, bpos);
			data = line.substr(bpos+1);
			str_map[key] = data;
		}

		configFile.close();
		return true;
	}

	return false;
}
