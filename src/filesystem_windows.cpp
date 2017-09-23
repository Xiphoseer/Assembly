#include "filesystem.hpp"

#ifdef _MSC_VER
#include <Windows.h>


std::vector<std::string>
fs::files_in_dir			( 	const std::string& dir,
								const std::string& filter 	)
{
	std::vector<std::string> files;
	WIN32_FIND_DATA fd;

	std::string search = dir + "\\" + filter;
	HANDLE h = FindFirstFile(search.c_str(), &fd);

	while (h != INVALID_HANDLE_VALUE)
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			files.push_back(fd.cFileName);
		}
		if (!FindNextFile(h, &fd))
		{
			FindClose(h);
			h = INVALID_HANDLE_VALUE;
		}
	}

	return files;
}

typedef bool (*fd_check) (WIN32_FIND_DATA);

bool is_dir(WIN32_FIND_DATA fd)
{
	return fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

bool is_not_dir(WIN32_FIND_DATA fd)
{
	return !is_dir(fd);
}

bool is_any(WIN32_FIND_DATA fd)
{
	return true;
}

bool
exists(const std::string& path, fd_check test)
{
	WIN32_FIND_DATA fd;
	HANDLE h = FindFirstFile(dir.c_str(), &fd);

	if (h != INVALID_HANDLE_VALUE)
	{
		if (fd_check(fd))
		{
			FindClose(h);
			return true;
		}
	}

	FindClose(h);
	return false;
}

bool
fs::dir_exists				(	const std::string& path 	)
{
	return exists(path, *is_dir);
}

bool
fs::file_exists 			( 	const std::string& path 	)
{
	return exists(path, *is_not_dir);
}

bool
fs::exists 			( 	const std::string& path 	)
{
	return exists(path, *is_any);
}

int
fs::create_dir 				(	const std::string& path 	)
{
	return CreateDirectory(dir.c_str(), NULL);
}

#endif // defined _MSC_VER
