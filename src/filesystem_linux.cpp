#include "filesystem.hpp"

#ifndef _MSC_VER
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

/* Adapted from :
 * https://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c#answer-306917
 * 
 * Question by	DShook 				(https://stackoverflow.com/users/370/dshook)
 * Answer 	by 	Chris Kloberdanz 	(https://stackoverflow.com/users/28714/chris-kloberdanz)
 *			and	RiaD				(https://stackoverflow.com/users/768110/riad)
 *
 * The relevant code segment is licensed under CC-BY-SA
 */

std::vector<std::string>
fs::files_in_dir			(	const std::string& path,
								const std::string& filter 	)
{
	DIR *dpdf;
	struct dirent *epdf;
	std::vector<std::string> files;

	dpdf = opendir(path.c_str());
	if (dpdf != NULL)
	{
		while (epdf = readdir(dpdf))
		{
			// TODO: implement filter
			if (epdf->d_type == DT_REG)
			{
				files.push_back(std::string(epdf->d_name));
			}
		}
	}

	closedir(dpdf);
	return files;
}

int fs::create_dir(const std::string& path)
{
	return mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

typedef bool (*st_func) (const struct stat&);

bool is_dir(const struct stat& sts)
{
	return S_ISDIR(sts.st_mode);
}

bool is_file(const struct stat& sts)
{
	return S_ISREG(sts.st_mode);
}

bool is_any(const struct stat& sts)
{
	return true;
}

bool exists(const std::string& path, st_func test)
{
	struct stat sts;
	if (stat(path.c_str(), &sts) == -1 && errno == ENOENT)
	{
	    return false;
	}
	return test(sts);
}

bool fs::exists(const std::string& path)
{
	return exists(path, *is_any);
}

bool fs::dir_exists(const std::string& path)
{
	return exists(path, *is_dir);
}

bool fs::file_exists(const std::string& path)
{
	return exists(path, *is_file);
}

#endif // not defined _MSC_VER
