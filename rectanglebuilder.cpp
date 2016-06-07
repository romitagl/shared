// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#include <iostream>
#include <string>
#include "rectanglebuilder.h"

// -- rapidjson --
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

const char RECTANGLES_KEY[] = "rects";
const char RECTANGLES_X[] = "x";
const char RECTANGLES_Y[] = "y";
const char RECTANGLES_W[] = "w";
const char RECTANGLES_H[] = "h";

//definition for compiler warning type
typedef long long unsigned int ullong_int_t;

using namespace rapidjson;

namespace rectanglebuilder
{
  void print_error(const std::string& key)
  {
    std::cerr << "error processing input json file - invalid key: " << key << std::endl;
  }

  bool parse_document(const Document& document, std::vector<rectangle>& input_rectangle)
  {
    if(!document.IsObject())
    {
      return false;
    }

    // check rects array
    if(document.HasMember(RECTANGLES_KEY) && document[RECTANGLES_KEY].IsArray())
    {
      //iterate and validate each element
      const Value& rects = document[RECTANGLES_KEY];
      for (SizeType count = 0; count < rects.Size(); count++)
      {
        //check x
        origin_t x = 0;
        if(rects[count].HasMember(RECTANGLES_X) && rects[count][RECTANGLES_X].IsInt())
        {
          x = rects[count][RECTANGLES_X].GetInt();
        }
        else
        {
          print_error(RECTANGLES_X);
          return false;
        }

        //check y
        origin_t y = 0;
        if(rects[count].HasMember(RECTANGLES_Y) && rects[count][RECTANGLES_Y].IsInt())
        {
          y = rects[count][RECTANGLES_Y].GetInt();
        }
        else
        {
          print_error(RECTANGLES_Y);
          return false;
        }

        //check w
        distance_t w = 0;
        if(rects[count].HasMember(RECTANGLES_W) && rects[count][RECTANGLES_W].IsUint())
        {
          w = rects[count][RECTANGLES_W].GetUint();
        }
        else
        {
          print_error(RECTANGLES_W);
          return false;
        }

        //check h
        distance_t h = 0;
        if(rects[count].HasMember(RECTANGLES_H) && rects[count][RECTANGLES_H].IsUint())
        {
          h = rects[count][RECTANGLES_H].GetUint();
        }
        else
        {
          print_error(RECTANGLES_H);
          return false;
        }

        input_rectangle.push_back(rectangle(x, y, w, h, std::to_string(static_cast<ullong_int_t>(count+1))));
      }
    }
    else
    {
      print_error(RECTANGLES_KEY);
      return false;
    }

    return true;
  }
}


