const firebaseConfig = {
    //   copy your firebase config informations
    apiKey: "AIzaSyByTWjYsnZQ4zSUH7ZEYJabD5XQxSppE3w",
    authDomain: "motor-health-analysis.firebaseapp.com",
    projectId: "motor-health-analysis",
    storageBucket: "motor-health-analysis.appspot.com",
    messagingSenderId: "869245690731",
    appId: "1:869245690731:web:89ceeac0a60df1d2df622e",
    databaseURL:"https://motor-health-analysis-default-rtdb.firebaseio.com/"
  
  };
  
// initialize firebase
firebase.initializeApp(firebaseConfig);
var start =0;
    var stop = 256;
    var samplingfrequency= 500;
    var samples = 512;

// reference your database
var fftarr = firebase.database().ref("fft/fft_arr");
var fftmeta = firebase.database().ref("fft/fft_meta");

fftmeta.on('value', (snapshot) =>{
    const meta = [];
    snapshot.forEach(childSnapshot => {

        meta.push(childSnapshot.val())
        
    });
    console.log(meta);
    var stop = meta[0]/2;
    var samplingfrequency= meta[1];
    var samples = meta[0];
});





fftarr.on('value', (snapshot) => {
    
    const data = [];
    snapshot.forEach(childSnapshot => {

        data.push(childSnapshot.val())
        
    });


    const temp = data[0].concat(data[1]);
    const amplitudefft = temp.concat(data[2]);
    ///////////////////////////

    

    var step = samplingfrequency/samples;

    var arre = Array.from(
        { length: (stop - start) / step + 1 },
        (value, index) => start + index * step
        );

    arre = arre.slice(0, stop);
    
    var frequency =arre.map(function(e){return e.toString()});
    

    console.log(amplitudefft);
    console.log(frequency);


    const ctx = document.getElementById('myChart');

    new Chart(ctx, {
        type: 'line',
        data: {
            labels: frequency,
        datasets: [{
            label: 'Amplitude',
            data: amplitudefft,
            borderWidth: 1
        }]
        },
        options: {
            scales: {
                y: {
                    title: {
                        display: true,
                        text: 'Amplitude',
                        color:'#1f40a3',
                        font: {
                          family: 'Comic Sans MS',
                          size: 20,
                          weight: 'bold',
                          lineHeight: 1.2,
                        },
                        padding: {top: 20, left: 0, right: 0, bottom: 0}
                    },
                    beginAtZero: true,
                    min: 0,
                    max: 20,
                },
                x: {
                    title: {
                        display: true,
                        text: 'Frequency',
                        color: '#1f40a3',
                        font: {
                          family: 'Comic Sans MS',
                          size: 20,
                          weight: 'bold',
                          lineHeight: 1.2,
                        },
                        padding: {top: 20, left: 0, right: 0, bottom: 0}
                    },
                    
                }
            }
        }
    });




});





  