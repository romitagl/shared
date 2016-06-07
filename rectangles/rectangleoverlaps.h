// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#ifndef __RECTANGLEOVERLAPS_H__
#define __RECTANGLEOVERLAPS_H__

#include <vector>
#include <set>
#include "rectangle.h"

namespace rectangleoverlaps
{
  //takes in input the array of rectangles and returns the output array containing the overlaps
  bool find_overlaps(const std::vector<rectangle>& input_rectangle, std::set<rectangle>& overlaps_rectangle);
}

#endif //__RECTANGLEOVERLAPS_H__
