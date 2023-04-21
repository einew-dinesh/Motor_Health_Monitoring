var start =0;
var stop = 256;
var samplingfrequency= 492;
var samples = 512;

var step = samplingfrequency/samples;

var arre = Array.from(
    { length: (stop - start) / step + 1 },
    (value, index) => start + index * step
    );

    arre = arre.slice(0, 256);
console.log(arre[255]);