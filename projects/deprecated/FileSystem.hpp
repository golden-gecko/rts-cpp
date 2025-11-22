#pragma once

#include <Ultralight/platform/FileSystem.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Gecko
{
    inline std::string file_to_string(std::string const& file_path)
    {
        std::ifstream inFile;
        inFile.open(file_path);

        std::stringstream strStream;
        strStream << inFile.rdbuf();
        std::string str = strStream.str();

        return str;
    }

    inline std::string stream_to_string(std::shared_ptr<std::fstream> stream)
    {
        stream->seekg(0);
        std::stringstream strStream;
        strStream << stream->rdbuf();
        std::string str = strStream.str();

        return str;
    }

    inline std::vector<char> stream_to_buffer(std::shared_ptr<std::fstream> stream)
    {
        stream->seekg(0, std::ios::end);
        std::streamsize size = stream->tellg();
        stream->seekg(0, std::ios::beg);

        std::vector<char> buffer(size);

        if (stream->read(buffer.data(), size))
        {
        }

        return buffer;
    }

    class FileSystem : public ultralight::FileSystem
    {
    public:
        bool FileExists(const ultralight::String16& path) override;
        bool GetFileSize(ultralight::FileHandle handle, int64_t& result) override;
        bool GetFileMimeType(const ultralight::String16& path, ultralight::String16& result) override;
        ultralight::FileHandle OpenFile(const ultralight::String16& path, bool open_for_writing) override;
        void CloseFile(ultralight::FileHandle& handle) override;
        int64_t ReadFromFile(ultralight::FileHandle handle, char* data, int64_t length) override;

    private:
        ultralight::FileHandle last_file_handle = 0;

        std::map<ultralight::FileHandle, std::shared_ptr<std::fstream>> file_handles;
    };
}
