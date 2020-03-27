xaxis = [-100*pi : 100*pi];

scales = [ 1,2,4,8 ];
%translations = [ 1,2,3,4,5 ];
translations = [ 1:1:5 ];

for scale = scales
  for translation = translations
    plot(waveletScaleAndTransFunction(xaxis,scale,translation,@waveletMexicanFunction));
    title( sprintf('Scale: %d - Translation: %d', scale, translation));
    grid off;
    pause (.0001);
  endfor
endfor