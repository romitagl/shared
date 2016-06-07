// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#include <iostream>
#include "jsonparser.h"
#include "rectanglebuilder.h"
#include "rectangleoverlaps.h"

int main(int argc, char* argv[])
{
  // -- Read input parameters --
  if (argc != 2)
  {
    std::cerr << "usage: " << argv[0] << " path to json file" << std::endl;
    return -1;
  }

  // -- STEP 1: Load input JSON file --
  rapidjson::Document document;
  if(!jsonparser::parse_from_path(argv[1], document))
  {
    return -1;
  }

  // -- STEP 2: Verify that the JSON file is valid and load rectangles --
  std::vector<rectangle> input_rectangle;
  if(!rectanglebuilder::parse_document(document, input_rectangle))
  {
    return -1;
  }

  // -- STEP 3: Output list of all input rectangles --
  std::cout << "Input:" << std::endl;
  for(size_t count = 0; count < input_rectangle.size(); ++count)
  {
    //1: Rectangle at (100,100), w=250, h=80.
    std::cout << count+1 << ": Rectangle at " << input_rectangle[count].to_string() << std::endl;
  }

  // -- STEP 4: Output list of all overlaps --
  std::set<rectangle> overlaps_rectangle;
  if(!rectangleoverlaps::find_overlaps(input_rectangle, overlaps_rectangle))
  {
    return -1;
  }

  std::cout << "Overlaps:" << std::endl;
  for(auto iter = overlaps_rectangle.begin(); iter != overlaps_rectangle.end(); ++iter)
  {
    std::cout << "Between rectangle " << iter->get_id() << " at: " << iter->to_string() << std::endl;
  }

  return 0;
}
