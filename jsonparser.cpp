// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#include "jsonparser.h"
#include <iostream>
#include <fstream>
// -- rapidjson --
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

using namespace rapidjson;

namespace jsonparser
{
  bool get_json_from_path(const std::string& path, std::string& json_out)
  {
    bool retval = true;

    std::ifstream inFile;
    std::string line;
    try
    {
      inFile.open(path.c_str(), std::ifstream::in);
      if (inFile.is_open())
      {
          while(std::getline(inFile, line))
          {
            json_out += line;
            //std::cout << line << std::endl;
          }
      }
      else
      {
        std::cerr << "error opening input json file with path: " << path << std::endl;
        return false;
      }
    }
    catch (std::exception const& e)
    {
      std::cerr << e.what() << std::endl;
      retval = false;
    }
    catch (...)
    {
      std::cerr << "error processing input json file with path: " << path << std::endl;
      retval = false;
    }
    inFile.close();

    return retval;
  }


  bool parse_from_path(const std::string& path, rapidjson::Document& document)
  {
    if(path.empty())
    {
      std::cerr << "error: empty input json path!" << std::endl;
      return false;
    }

    std::string json;
    if(!get_json_from_path(path, json))
    {
     return false;
    }

    // parse a JSON string into DOM.
    document.Parse(json.c_str());

    // whether a parse error has occured in the last parsing.
    if(document.HasParseError())
    {
        std::cerr << "parse error: " << document.GetParseError() << " - offset: " << document.GetErrorOffset() << " - error message: " << GetParseError_En(document.GetParseError()) <<  std::endl;
        return false;
    }

    assert(document.IsObject()); // RFC4627, only objects or arrays were allowed as root values. In this case, the root is an object.

    return true;
  }
}

