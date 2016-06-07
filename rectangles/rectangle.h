// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include <string>

// rectangle coordinates types
typedef int origin_t;
typedef unsigned int distance_t;

/*
 * Defines a rectangle in a 2D space using X, Y integer coordinates for one corner and a width and height value.
 * Width and height are always positive integers.
 */
class rectangle
{
public:
  //  constructor
  rectangle(origin_t& x, origin_t& y, distance_t& w, distance_t& h, const std::string& id);
  //return a printable string of the rectangle coordinates
  const std::string& to_string() const { return string_coords; }
  const std::string& get_id() const { return string_id; }
  // comparison operator
  bool operator == (const rectangle& rect) const;
  bool operator != (const rectangle& rect) const;
  bool operator < (const rectangle& rect) const;
  //get coordinates functions
  origin_t get_x() const { return origin_x; }
  origin_t get_y() const { return origin_y; }
  distance_t get_w() const { return distance_w; }
  distance_t get_h() const { return distance_h; }
private:
  origin_t origin_x;
  origin_t origin_y;
  distance_t distance_w;
  distance_t distance_h;
  std::string string_coords;
  std::string string_id;
};

#endif //__RECTANGLE_H__
