#pragma once

#include <string>
#include "utils/json11.hpp"
#include <fstream>
#include <iostream>
namespace config
{

struct ConfigOption
{
	std::string targetHost;
	int port;
};

class ConfigParser
{
public:
	static ConfigOption parse(const char* configFile = DEFAULT_CONFIG_FILE)
	{
		const size_t configSize = 4096;
		char configStr[configSize] = {};
		ConfigOption option{};
		if (readConfigFile(configFile, configStr, configSize) <= 0)
		{
			return option;
		}
		
		std::string err{};
		auto js = json11::Json::parse(configStr, err);
		auto& optionMap = js.object_items();
		json11::Json host;
		json11::Json port;
		try
		{
			host = optionMap.at("target_host");
			port = optionMap.at("target_port");
		}
		catch (std::out_of_range & ex)
		{
			std::cout << "config error..." << ex.what() << std::endl;
			return option;
		}
		if (!host.is_string())
		{
			std::cout << "host config error..." << std::endl;
			return option;
		}
		option.targetHost = host.string_value();

		if (!port.is_number())
		{
			std::cout << "port config error..." << std::endl;
			return option;
		}
		option.port = port.int_value();
		return option;
	}

private:
	static int readConfigFile(const char* configFile, char* const outStr, size_t size)
	{
		std::ifstream ifs{};
		ifs.open(configFile, std::ios::in);
		std::streamsize sizeRead = 0;
		try
		{
			auto& s = ifs.read(outStr, size);
			sizeRead = strlen(outStr);
			if (!s && !s.eof())
			{
				std::cout << "config file read error..." << std::endl;
			}
		}
		catch (std::ios::failure& ex)
		{
			std::cout << "exception occured: " << ex.what() << std::endl;
			ifs.close();
			return -1;
		}
		ifs.close();
		return sizeRead;
	}

private:
	static const char* DEFAULT_CONFIG_FILE;
};

}

