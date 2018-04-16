#include "manifest.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>


namespace assembly::manifest
{
    enum manifest_section
    {
        MF_GENERAL,
        MF_VERSION,
        MF_FILES,
        MF_UNKNOWN
    };

    bool lexical_compare(std::string A, std::string B)
    {
        return A.compare(B) < 0;
    }

    bool manifest_compare(manifest_entry A, manifest_entry B)
    {
        return lexical_compare(A.path, B.path);
    }

    int read_from_file(const std::string& filename, manifest_file& manifest)
    {
        std::ifstream infile(filename);
        if (infile.is_open())
        {
            read_from_stream(infile, manifest);
            infile.close();
            return 0;
        }
        else
        {
            std::cerr << "Failed to load '" << filename << "' as manifest!" << std::endl;
            return 1;
        }
    }

    int read_from_stream(std::istream& infile, manifest_file& manifest)
    {
        std::string line;
        manifest_section section = MF_GENERAL;

        while(!infile.eof())
        {
            std::getline(infile, line);

            // Ignore empty lines
            if (line.length() == 0) continue;

            // Find INI-like headers "[...]"
            if (line.at(0) == '[' && line.at(line.length() - 1) == ']')
            {
                std::string sectionName = line.substr(1,line.length() - 2);
                if (sectionName == "version")
                {
                    section = MF_VERSION;
                }
                else if (sectionName == "files")
                {
                    section = MF_FILES;
                }
                else
                {
                    std::cout << "Unknown Manifest section: [" << sectionName << "]" << std::endl;
                    section == MF_UNKNOWN;
                }
            }
            else if (section == MF_VERSION) // Interpret a version line
            {
                std::istringstream linestream(line);
                std::string field;

                std::getline(linestream, field, ',');
                manifest.fileVersion = std::stoul(field);
                std::getline(linestream, manifest.checksum, ',');
                std::getline(linestream, manifest.version, ',');
            }
            else if (section == MF_FILES ) // Interpret a line for a file
            {
                std::istringstream linestream(line);
                std::string field;
                manifest_entry entry;
                std::getline(linestream, entry.path, ',');
                std::getline(linestream, field, ',');
                entry.sizeA = std::stoul(field);
                std::getline(linestream, entry.checkA, ',');
                std::getline(linestream, field, ',');
                entry.sizeB = std::stoul(field);
                std::getline(linestream, entry.checkB, ',');
                std::getline(linestream, entry.checkC, ',');
                manifest.files.push_back(entry);
            }
        }
    }

    manifest_entry manifest_file::find(std::string path)
    {
        manifest_entry test;
        test.path = path;
        std::vector<manifest_entry>::iterator it = std::lower_bound(this->files.begin(), this->files.end(), test, manifest_compare);
        if (it->path == test.path)
        {
            return *it;
        }
        else
        {
            return test;
        }
    }
}

