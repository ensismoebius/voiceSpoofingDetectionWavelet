xaxis = [-1000*pi : 1000*pi];

scales = [ 1,2,4,8 ];
translations = [ 1,2,3,4,5 ];

%hold on;
for scale = scales
  for translation = translations
    plot(waveletScaleAndTransFunction(xaxis,scale,translation,@sin));
    title( sprintf('Scale: %d - Translation: %d', scale, translation));
    grid off;
    %print('figure.pdf','-dpdf')
  endfor
endfor

%hold off;

