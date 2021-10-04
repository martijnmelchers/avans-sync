#pragma once
#include <filesystem>

enum State
{
	UPLOAD,
	DOWNLOAD,
	READ,
};

struct DownloadSettings
{
	int totalBytes;
	std::filesystem::path path;
};


struct Settings
{
	State state;
	DownloadSettings downloadSettings;
	std::filesystem::path uploadPath;
};


