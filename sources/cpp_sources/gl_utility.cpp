#include <iostream>
#include <fstream>

#include "gl_utility.h"

void logger(std::string_view log_info_, std::string_view info_) {
	std::string_view logs_file_name("logs");
	std::ofstream logs(logs_file_name.data(), std::ios::binary | std::ios::app | std::ios::out);

	if (logs.is_open()) {
		logs << log_info_ << ". " << info_ << '\n';

		logs.close();
	}
	else {
		std::cerr << log_info_ << ". " << info_ << '\n';
	}
}