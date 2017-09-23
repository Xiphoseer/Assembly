#pragma once

#include <string>
#include <vector>

#ifdef _MSC_VER
#define FS_DEFAULT_FILE_SEP "\\"
#else
#define FS_DEFAULT_FILE_SEP "/"
#endif

namespace fs
{
	typedef struct path
	{
		std::vector<std::string> comp;

		path append(const std::vector<std::string>& app);
		path append(const std::string& app);
		path append(const fs::path& app);

		path(const std::string& filename);
		path(const std::vector<std::string>& val);
		path();

		std::string
		to_string 		( 	const std::string&
							separator 			) const;
		std::string
		to_string() const;
	}
	path_t;

	int
	create_dir	 		(	const std::string& path 	);

	bool
	exists				(	const std::string& path 	);

	bool
	exists  	      	(	const fs::path& path 		);

	bool
	file_exists			(	const std::string& path 	);

	bool
	file_exists         (	const fs::path& path 		);

	bool
	dir_exists			(	const std::string& path 	);

	bool
	dir_exists         (	const fs::path& path 		);

	void
	ensure_dir_exists 	(	const std::string& path 	);

	std::vector<std::string>
	files_in_dir		(	const std::string& path,
							const std::string& filter 	);
}
