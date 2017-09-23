#include "filesystem.hpp"
#include <sstream>
#include <cstring>

//TODO: remove
#include <iostream>

void fs::ensure_dir_exists(const std::string& path)
{
	int index = path.find_last_of(FS_DEFAULT_FILE_SEP);
	if (index > 0) {
		std::string dir(path.substr(0, index));
		if (!exists(dir)) {
			ensure_dir_exists(dir);
			create_dir(dir);
		}
	}
}

int inline len(const std::string& str)
{
	return str.size();
}

int inline len(const char* str)
{
	return strlen(str);
}

fs::path fs::path::append(const std::string& app)
{
	size_t index = app.find_first_of(FS_DEFAULT_FILE_SEP);
	bool single = (index == app.npos);

	std::string add = single ? app : app.substr(0, index);

	if (add == ".." && this->comp.size() > 0)
	{
		this->comp.pop_back();
	}
	else
	{
		this->comp.push_back(add);
	}

	if (single == false)
	{
		this->append(app.substr(index + len(FS_DEFAULT_FILE_SEP)));
	}

	return *this;
}

fs::path fs::path::append(const std::vector<std::string>& app)
{
	for (std::string app_part : app) this->append(app_part);
	return *this;
}

fs::path fs::path::append(const fs::path& app)
{
	this->append(app.comp);
	return *this;
}

fs::path::path(const std::vector<std::string>& val)
{
	this->append(val);
}

fs::path::path(const std::string& val)
{
	this->append(val);
}

fs::path::path() {}

std::string fs::path::to_string(const std::string& sep) const
{
	bool first = true;
	std::stringstream str;
	for (std::string c : this->comp)
	{
		if (first)
		{
			first = false;
		}
		else
		{
			str << sep;
		}
		str << c;
	}
	return str.str();
}

std::string fs::path::to_string() const
{
	return this->to_string(FS_DEFAULT_FILE_SEP);
}

bool fs::file_exists(const fs::path& path)
{
	return fs::file_exists(path.to_string());
}

bool fs::dir_exists(const fs::path& path)
{
	return fs::dir_exists(path.to_string());
}

bool fs::exists(const fs::path& path)
{
	return fs::exists(path.to_string());
}
