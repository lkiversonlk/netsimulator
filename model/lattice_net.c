#include "commons/type.h"
#include "model/lattice_net.h"

#include <math.h>

/***************************************************************************************
 *                    [col]
 * 0     1    2    3    4    5    ...   edge-2 edge-1
 * edge  ...                                   2edge-1
 * ...  ...                                     ...
 *           i-2  i-1   i   i+1  ...            ...    [row]
 * 
 ***************************************************************************************/

int lattice_foreach_within(oper_on_each oper, void *ctx, net_size_t i, double radius, double width, Lattice_Net *net){
  
  int edge = sqrt(net_size(net));
  int row = i / edge;     //starts from 0
  int col = i - edge * row;   //starts from 0
  
  int row_start = row - radius;                             //for example row is 4, radius 1.5, so the left row should be 2
  if(row_start < 0) row_start = 0;
  int row_end = row + radius + 1;                           //for example row is 4, radius 1.5, so the right row should be 6
  if(row_end >= edge) row_end = edge - 1;
  
  int col_start = col - radius;                             //see below
  if(col_start < 0) col_start = 0;
  int col_end = col + radius + 1;                           //se below
  if(col_end >= edge) col_end = edge - 1;
  
  int ret = 0;
  int cur_row, cur_col;
  for(cur_row = row_start; cur_row <= row_end; cur_row ++){
    for(cur_col = col_start; cur_col <= col_end; cur_col++){
      net_size_t j = cur_row * edge + cur_col;
      double distance = distance_direct(i, j, net, NULL);
      if(distance <= radius && distance > (radius - width)){
	ret += oper(j, ctx);
      }
    }
  }
  return ret;
}

double distance_to_center(net_size_t i, Net *net, void *ctx){
  net_size_t edge = sqrt(net_size(net));
  net_size_t x = i / edge;
  net_size_t y = i - x * edge;

  double xx = (double)x - (double)(edge - 1) / 2.0;
  xx *= xx;
  double yy = (double)y - (double)(edge - 1) / 2.0;
  yy *= yy;

  double distance = sqrt(xx + yy);
  return distance;
}
