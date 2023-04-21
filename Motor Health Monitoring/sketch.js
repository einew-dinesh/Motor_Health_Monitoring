var canvas;
var score;
var button;
var initialInput;
var submitButton;
var database;

function setup() {
  canvas = createCanvas(100, 100);
  score = 0;
  createP('Click the button to get points.');
  button = createButton('click');
  button.mousePressed(increaseScore);
  initialInput = createInput('initials');
  submitButton = createButton('submit');
  submitButton.mousePressed(submitScore);

  var config = {
    apiKey: "AIzaSyBnhHx1AjK6IuMpUX6PRpyp7sKpIDmRsY4",
    authDomain: "learning-b935c.firebaseapp.com",
    projectId: "learning-b935c",
    storageBucket: "learning-b935c.appspot.com",
    messagingSenderId: "407153653426",
    appId: "1:407153653426:web:67684eafdbd8c61881bbab"
  };
  const app = firebase.initializeApp(config);
  database = firebase.database();
}

function submitScore() {
  var data = {
    initials: initialInput.value(),
    score: score
  };
  console.log(data);
  var ref = database.ref('scores');
  ref.push(data);
}

function increaseScore() {
  score++;
}

function draw() {
  background(0);
  textAlign(CENTER);
  textSize(32);
  fill(255);
  text(score, width / 2, height / 2);
}