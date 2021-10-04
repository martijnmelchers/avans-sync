#pragma once
#include <deque>
#include <filesystem>

enum State
{
	UPLOAD,
	DOWNLOAD,
	READ,
	WAIT_INPUT,
	SYNC
};

enum SyncState
{
	NONE,
	CREATE_DIRECTORIES,
	LIST_FILES,
	READ_FILES,
	UPLOAD_FILES,
	DELETE_FILES
};

struct DownloadSettings
{
	int totalBytes;
	std::filesystem::path path;
};

struct SyncSettings
{
	SyncState state;
	std::deque<std::filesystem::path> fileQueue;
	std::vector<std::string> serverFiles;
	std::deque<std::filesystem::path> directoryQueue;
	std::deque<std::string> serverOutput;
};

struct UploadSettings
{
	std::filesystem::path path;
};


struct Settings
{
	State state;
	int pendingLines;
	bool catchOutput;
	DownloadSettings downloadSettings;
	UploadSettings uploadSettings;
	SyncSettings syncSettings;

};


