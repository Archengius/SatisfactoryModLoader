#pragma once
#include <string>
#include <sstream>
#include "Json.h"
#include "SatisfactoryModLoader.h"

namespace SML {

	path getModConfigFilePath(std::wstring modid);

	/**
	 * Parses given json string into a valid json object
	 * Strips json syntax extensions like commentaries prior to parsing
	 */
	SML_API TSharedPtr<FJsonObject> parseJsonLenient(const std::wstring& input);

	/**
	 * Parses mod configuration file and returns json object
	 * returns json.null() if config file is missing, unreadable or corrupted
	 * It also supports comments in mod configs
	 */
	SML_API TSharedRef<FJsonObject> readModConfig(std::wstring modid, const TSharedRef<FJsonObject>& defaultValues);

	/*
	 * Dumps the given mod configuration json to the mod config file with the given modid.
	 * Completely overwrites the file with the given json.
	 *
	 * @param[in]	modid	the id of the mod you want to save the config from
	 * @param[in]	config	the coniguration you want to overwrite the file with
	 */
	SML_API void writeModConfig(std::wstring modid, const TSharedRef<FJsonObject>& config);
	
	bool setDefaultValues(const TSharedPtr<FJsonObject>& j, const TSharedPtr<FJsonObject>& defaultValues);
	
	template<typename First, typename ...Args>
	std::wstring formatStr(First &&arg0, Args &&...args) {
		std::wostringstream stringStream;
		formatInternal(stringStream, arg0, args...);
		return stringStream.str();
	}

	inline void formatInternal(std::wostringstream& stream) {}

	template<typename First, typename ...Args>
	void formatInternal(std::wostringstream& stream, First &&arg0, Args &&...args) {
		stream << std::forward<First>(arg0);
		formatInternal(stream, std::forward<Args>(args)...);
	}
};