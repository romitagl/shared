// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#include <iostream>
#include "jsonparser.h"
#include "rectanglebuilder.h"
#include "rectangleoverlaps.h"

// this test executable implements a first set of robustness and correcteness tests
// specific unit tests should be implemented using Cppunit or GoogleTest
int main()
{
  // -- invalid input path --
  {
    // -- test wrong path --
    rapidjson::Document document;
    if(!jsonparser::parse_from_path("error", document))
    {
      std::cout << "- Wrong input path test: OK" << std::endl;
    }
    else
    {
      std::cout << "- Wrong input path test: NOK" << std::endl;
      return -1;
    }

    // -- test empty document --
    std::vector<rectangle> input_rectangle;
    if(!rectanglebuilder::parse_document(document, input_rectangle))
    {
      std::cout << "- Empty json document test: OK" << std::endl;
    }
    else
    {
      std::cout << "- Empty json document test: NOK" << std::endl;
      return -1;
    }

    // -- test invalid input rectangles array --
    std::set<rectangle> overlaps_rectangle;
    if(!rectangleoverlaps::find_overlaps(input_rectangle, overlaps_rectangle))
    {
      std::cout << "- Empty number of reclangles test: OK" << std::endl;
    }
    else
    {
      std::cout << "- Empty number of reclangles test: NOK" << std::endl;
      return -1;
    }
  }

  // -- rectangles_invalid_format.json --
  {
    // -- test rectangles invalid json format  --
    rapidjson::Document document;
    if(!jsonparser::parse_from_path("./json_input_files/rectangles_invalid_format.json", document))
    {
      std::cout << "- Invalid input json format test: OK" << std::endl;
    }
    else
    {
      std::cout << "- Invalid input json format test: NOK" << std::endl;
      return -1;
    }
  }

  // -- rectangles_invalid_rectangle_format.json --
  {
    // -- test rectangles invalid rectangle json format step 1 --
    rapidjson::Document document;
    if(!jsonparser::parse_from_path("./json_input_files/rectangles_invalid_rectangle_format.json", document))
    {
      std::cout << "- Invalid input rectangle json format test step 1: NOK" << std::endl;
      return -1;
    }
    else
    {
      std::cout << "- Invalid input rectangle json format test step 1: OK" << std::endl;

    }

    // -- test rectangles invalid rectangle json format step 2 --
    std::vector<rectangle> input_rectangle;
    if(!rectanglebuilder::parse_document(document, input_rectangle))
    {
      std::cout << "- Invalid input rectangle json format test step 2: OK" << std::endl;
    }
    else
    {
      std::cout << "- Invalid input rectangle json format test step 2: NOK" << std::endl;
      return -1;
    }
  }

  // -- rectangles_invalid_number_rectangles.json --
  {
  // -- test invalid number rectangles step 1 --
    rapidjson::Document document;
    if(!jsonparser::parse_from_path("./json_input_files/rectangles_invalid_number_rectangles.json", document))
    {
      std::cout << "- Test invalid number rectangles step 1: NOK" << std::endl;
      return -1;
    }
    else
    {
      std::cout << "- Test invalid number rectangles step 1: OK" << std::endl;
    }

    // -- test invalid number rectangles step 2 --
    std::vector<rectangle> input_rectangle;
    if(!rectanglebuilder::parse_document(document, input_rectangle))
    {
      std::cout << "- Test invalid number rectangles step 2: NOK" << std::endl;
      return -1;
    }
    else
    {
      std::cout << "- Test invalid number rectangles step 2: OK" << std::endl;
    }

    // -- test invalid number rectangles step 3 --
    std::set<rectangle> overlaps_rectangle;
    if(!rectangleoverlaps::find_overlaps(input_rectangle, overlaps_rectangle))
    {
      std::cout << "- Test invalid number rectangles step 3: OK" << std::endl;
    }
    else
    {
      std::cout << "- Test invalid number rectangles step 3: NOK" << std::endl;
      return -1;
    }
  }

  // -- rectangles_duplicates.json --
  {
  // -- test rectangles duplicates step 1 --
    rapidjson::Document document;
    if(!jsonparser::parse_from_path("./json_input_files/rectangles_duplicates.json", document))
    {
      std::cout << "- Test rectangles duplicates step 1: NOK" << std::endl;
      return -1;
    }
    else
    {
      std::cout << "- Test rectangles duplicates step 1: OK" << std::endl;
    }

    // -- test rectangles duplicates step 2 --
    std::vector<rectangle> input_rectangle;
    if(!rectanglebuilder::parse_document(document, input_rectangle))
    {
      std::cout << "- Test rectangles duplicates step 2: NOK" << std::endl;
      return -1;
    }
    else
    {
      std::cout << "- Test rectangles duplicates step 2: OK" << std::endl;
    }

    // -- test rectangles duplicates step 3 --
    std::set<rectangle> overlaps_rectangle;
    if(!rectangleoverlaps::find_overlaps(input_rectangle, overlaps_rectangle))
    {
      std::cout << "- Test rectangles duplicates step 3: NOK" << std::endl;
      return -1;
    }
    else
    {
      std::cout << "- Test rectangles duplicates step 3: OK" << std::endl;
    }

    // -- test rectangles duplicates step 4 --
    if(overlaps_rectangle.size() == 2)
    {
      //rectangle 1 and 5 at: (140,160), w=210, h=20.
      //rectangle 3 and 5 at: (140,200), w=230, h=60.
      auto first = overlaps_rectangle.begin();
      auto second = std::next(first, 1);
      if(first->get_id() == "1 and 5" &&
          first->to_string() == "(140,160), w=210, h=20." &&
          second->get_id() == "3 and 5" &&
          second->to_string() == "(140,200), w=230, h=60.")
      {
        std::cout << "- Test rectangles duplicates step 4: OK" << std::endl;
      }
      else
      {
        std::cout << "- Test rectangles duplicates step 4: NOK" << std::endl;
        return -1;
      }
    }
    else
    {
      std::cout << "- Test rectangles duplicates step 4: NOK" << std::endl;
      return -1;
    }
  }

  std::cout << "-- All tests PASSED -- " << std::endl;

  return 0;
}
