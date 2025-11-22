#include "UI/FileSystem.hpp"

#include "Exception.hpp"

#include <filesystem>

namespace Gecko
{
    bool FileSystem::FileExists(const ultralight::String16& path)
    {
        throw Exception("Not implemented."); 
    }

    bool FileSystem::GetFileSize(ultralight::FileHandle handle, int64_t& result)
    {
        result = file_handles[handle]->tellg();

        file_handles[handle]->seekg(0, std::ios::end);

        result = file_handles[handle]->tellg() - result;

        return true;
    }

    bool FileSystem::GetFileMimeType(const ultralight::String16& path, ultralight::String16& result)
    {
        static const std::map<std::string, std::string> types =
        {
            { ".css" , "text/css"        },
            { ".html", "text/html"       },
            { ".jpg" , "image/jpeg"      },
            { ".jpeg", "image/jpeg"      },
            { ".js"  , "text/javascript" },
            { ".png" , "image/png"       },
            { ".svg" , "image/svg+xml"   },
            { ".ts"  , "text/javascript" }
        };

        auto extension = std::filesystem::path(path.data()).extension();
        auto type = types.find(extension.string());

        if (type == types.end())
        {
            throw Exception("Unknown file type: " + extension.string());
        }

        result = type->second.c_str();

        return true;
    }

    ultralight::FileHandle FileSystem::OpenFile(const ultralight::String16& path, bool open_for_writing)
    {
        last_file_handle += 1;

        file_handles[last_file_handle] = std::make_shared<std::fstream>();
        file_handles[last_file_handle]->open(path.data());

        auto is_opened = file_handles[last_file_handle]->is_open();

        return last_file_handle;
    }

    void FileSystem::CloseFile(ultralight::FileHandle& handle)
    {
        file_handles.erase(handle);
    }

    int64_t FileSystem::ReadFromFile(ultralight::FileHandle handle, char* data, int64_t length)
    {
        auto stream = stream_to_buffer(file_handles[handle]);

        std::memcpy(data, stream.data(), length);

        return length;
    }
}
