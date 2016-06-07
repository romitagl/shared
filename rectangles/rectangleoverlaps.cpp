// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#include <iostream>
#include <map>
#include <algorithm>
#include "rectangleoverlaps.h"

namespace rectangleoverlaps
{
  void check_x_coord(const std::vector<rectangle>& input_rectangle, std::multimap<origin_t, rectangle>& x_coordinates)
  {
    std::multimap<origin_t, rectangle>::iterator iter;
    for(size_t count = 0; count < input_rectangle.size() ; ++count)
    {
      bool duplicate = false;
      // search for existing key and check if duplicate
      iter = x_coordinates.find(input_rectangle[count].get_x());
      while((iter != x_coordinates.end())
          && (iter->first == input_rectangle[count].get_x()))
      {
        if(iter->second == input_rectangle[count])
        {
          //skip the duplicate
          duplicate = true;
          break;
        }
        ++iter; //in case of more elements with the same key
      }

      if(!duplicate)
      {
        //left x
        x_coordinates.insert(std::make_pair(input_rectangle[count].get_x(), input_rectangle[count]));
      }
    }
  }
  
  bool find_overlaps_single(const std::vector<rectangle>& input_rectangle, std::set<rectangle>& overlaps_rectangle)
  {
    //at least 2 rectangles are needed
    if(input_rectangle.size() <= 1)
    {
      std::cerr << "insufficient number of input rectangles: " << input_rectangle.size() << std::endl;
      return false;
    }
    
    // add all the x coordinates in a map
    std::multimap<origin_t, rectangle> x_coordinates;
    check_x_coord(input_rectangle, x_coordinates);
    
    if(x_coordinates.size() <= 1)
    {
      std::cerr << "insufficient number of coordinates available" << std::endl;
      return false;
    }
    
    //left iterator
    std::multimap<origin_t, rectangle>::iterator x_coordinates_iter = x_coordinates.begin();
    //next iter
    std::multimap<origin_t, rectangle>::iterator x_coordinates_iter_next = std::next(x_coordinates_iter, 1);
    
    //outer loop checking all coordinates
    while(x_coordinates_iter_next != x_coordinates.end())
    {
      //inner loop - check the current coordinate with all the others
      while(x_coordinates_iter_next != x_coordinates.end())
      {
        //not the same rectangle
        if(x_coordinates_iter->second != x_coordinates_iter_next->second)
        {
          origin_t x = 0;
          origin_t y = 0;
          int w = 0;
          int h = 0;
          
          //x are sorted so always the intersection start with next
          x = x_coordinates_iter_next->second.get_x();
          y = std::max(x_coordinates_iter->second.get_y(), x_coordinates_iter_next->second.get_y());
          
          distance_t y_max = std::min(x_coordinates_iter->second.get_y() + x_coordinates_iter->second.get_h(),
                                      x_coordinates_iter_next->second.get_y() + x_coordinates_iter_next->second.get_h());
          h = y_max - y;
          
          distance_t x_max = std::min(x_coordinates_iter->second.get_x() + x_coordinates_iter->second.get_w(),
                                      x_coordinates_iter_next->second.get_x() + x_coordinates_iter_next->second.get_w());
          w = x_max - x;
          
          if((w > 0) && (h > 0))
          {
            overlaps_rectangle.insert(rectangle(x, y, reinterpret_cast<distance_t&>(w), reinterpret_cast<distance_t&>(h),
                                                   x_coordinates_iter->second.get_id() + " and " + x_coordinates_iter_next->second.get_id()));
          }
        }
        //check against the following rectangle
        ++x_coordinates_iter_next;
      }
      ++x_coordinates_iter;
      x_coordinates_iter_next = std::next(x_coordinates_iter, 1);
    }
    
    return true;
  }

  bool find_overlaps(const std::vector<rectangle>& input_rectangle, std::set<rectangle>& overlaps_rectangle)
  {
    //match the single intersections
    if(!rectangleoverlaps::find_overlaps_single(input_rectangle, overlaps_rectangle))
    {
      return false;
    }
    
    std::vector<rectangle> input_rectangle_overlap(input_rectangle.begin(), input_rectangle.end());
    input_rectangle_overlap.insert( input_rectangle_overlap.end(), overlaps_rectangle.begin(), overlaps_rectangle.end() );
    //match the multiple intersections
    if(!rectangleoverlaps::find_overlaps_single(input_rectangle_overlap, overlaps_rectangle))
    {
      return false;
    }
    
    return true;
  }
}
