#pragma once

#include <chrono>
#include <ctime>
#include <string>
#include <filesystem>
#include <iostream>

const std::string basePath = "data/";

template <typename TP>
std::time_t to_time_t(TP tp)
{
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
		+ system_clock::now());
	return system_clock::to_time_t(sctp);
}

static bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

static bool is_valid_path(const std::string& s)
{
	if (s.find('?') != std::string::npos ||
		s.find(':') != std::string::npos ||
		s.find('*') != std::string::npos ||
		s.find('\"') != std::string::npos ||
		s.find('|') != std::string::npos ||
		s.find('>') != std::string::npos ||
		s.find('<') != std::string::npos ||
		s.find('\\') != std::string::npos ||
		s.find("../") != std::string::npos
		)
		return false;

	return true;
}

static bool hasReadPermissions(std::filesystem::perms p)
{
	return (p & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
}

static bool hasWritePermissions(std::filesystem::perms p)
{
	return (p & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
}
static bool hasReadAndWritePermissions(std::filesystem::perms p)
{
	return hasReadPermissions(p) && hasWritePermissions(p);
}

static bool parent_directory_exists(const std::filesystem::path& p)
{
	const std::string pathString = p.string();
	const std::string directory = pathString.substr(0, pathString.find_last_of("\\/"));

	if (!std::filesystem::exists(directory))
		return false;


	return true;
}

static void writeClient(const std::string content)
{
	std::cout << "CLIENT >> " << content << std::endl;
}

static void writeServer(const std::string content)
{
	std::cout << "SERVER >> " << content << std::endl;
}

static std::string normalize_path(const std::filesystem::path& p)
{
	std::string pathString = p.string();
	const size_t pos = pathString.find(basePath);

	// If found then erase it from string
	if (pos != std::string::npos)
	{
		pathString.erase(pos, basePath.length());
	}

	std::replace(pathString.begin(), pathString.end(), '\\', '/');
	return pathString;
}