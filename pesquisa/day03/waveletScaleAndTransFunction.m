function retVar = waveletScaleAndTransFunction(value, scale, translation, func)
  retVar = (2 ^ ( -scale / 2 )) * func( ( 2^(-scale) ) * value - translation );
endfunction