// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#ifndef __RECTANGLEBUILDER_H__
#define __RECTANGLEBUILDER_H__

#include <vector>
#include "rectangle.h"
// -- rapidjson --
#include "rapidjson/document.h"

namespace rectanglebuilder
{
  //parse the rapidjson DOM and extract the rectangles
  bool parse_document(const rapidjson::Document& document, std::vector<rectangle>& input_rectangle);
}

#endif //__RECTANGLEBUILDER_H__
