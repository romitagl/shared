// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#ifndef __JSONPARSER_H__
#define __JSONPARSER_H__

#include <string>

// -- rapidjson --
#include "rapidjson/document.h"

namespace jsonparser
{
  //pase takes in input the path of the JSON file to parse
	bool parse_from_path(const std::string& path, rapidjson::Document& document);
}

#endif //__JSONPARSER_H__
