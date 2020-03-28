function retVar = waveletMexicanFunction(x)
  retVar = (1 - x.^2).*e.^((-x.^2)/2);
endfunction