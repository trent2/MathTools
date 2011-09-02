/*  MathTools 
    Copyright (C) 2010, 2011, 2012 Robert Spillner <Robert.Spillner@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*  file: --- Calc.hpp --- */

#ifndef __CALC_HPP_
#define __CALC_HPP_

#include <cmath>

namespace calc {
  inline int roundi(double d) {
    return d<0 ? d-0.5 : d+0.5;
  }

  inline double round(double d) {
    return d<0 ? std::ceil(d-0.5) : std::floor(d+0.5);
  }
}
#endif
