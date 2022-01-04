#ifndef LINEDRAWX_H
#define LINEDRAWX_H

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "dstructx.h"

/*
* @brief Handles special cases of lines
*
*    Vertical line: occurs when x2-x1=0 thus m=INF -> thus we calculate the points between
*    the y1 and y2. If y1 > y2 swap values, in order to make the calculations all along
*    the Y-Axis in a positive direction.
*
*    Horizontal line: occurs when y2-y1=0 thus m=0 -> thus we calculate the points between
*    the x1 and x2. If x1 > x2 swap values, in order to make the calculations all along 
*    the Y-Axis in a positive direction.
*
*    Diagonal 45° line: occurs when m=1
*/
void special_case(line_t * line, point2d_stack ** coords);

/*
 *@brief Implementation of naive approach of line drawing algorithm
*/
void slope_intercept(line_t * line, point2d_list ** coords);
/*
 *@brief Implementation of DDA (Digital Differental Analyzer) line drawing algorithm
*/
void dda(line_t * line, point2d_list ** coords);
/*
 *@brief Implementation of Bresenham line drawing algorithm
*/
void bresenham(line_t * line, point2d_stack ** coords);

#endif