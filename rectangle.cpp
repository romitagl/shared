// Copyright (C) 2016 Gian Luigi Romita. All rights reserved.

#include "rectangle.h"
#include <sstream>

rectangle::rectangle(origin_t& x, origin_t& y, distance_t& w, distance_t& h, const std::string& id) :
   origin_x(x), origin_y(y), distance_w(w), distance_h(h), string_coords(""), string_id(id)
{
  std::stringstream coordinates;
  //(100,100), w=250, h=80.
  coordinates << "(" << origin_x << "," <<  origin_y << "), w=" << distance_w << ", h=" << distance_h << ".";
  string_coords = coordinates.str();
}

bool rectangle::operator==(const rectangle& rect) const
{
  if( origin_x == rect.origin_x &&
      origin_y == rect.origin_y &&
      distance_w == rect.distance_w &&
      distance_h == rect.distance_h)
  {
    return true;
  }

  return false;
}

bool rectangle::operator !=(const rectangle& rect) const
{
  return !operator==(rect);
}

bool rectangle::operator<(const rectangle& rect) const
{
  return string_coords < rect.string_coords;
}
