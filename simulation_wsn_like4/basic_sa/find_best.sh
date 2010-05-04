#!/bin/bash

awk '
BEGIN{x_1 = 0;o = 0 ; best=""}
{ x = $8;
  if (x != x_1) {
    x_1=x
    o = 0
  }else{
    if (o++ < '$1'){
       print $0
    }
  }
}'