function toRad(angle)
{
  return angle / 180.0 * Math.PI;
}

function drawArc(center, angle, radius)
{

  var rad         = toRad(angle);
      from_rel    = new Point(0, -radius);
      through_rel = new Point(Math.sin(rad / 2) * radius, -Math.cos(rad / 2) * radius);
      to_rel      = new Point(Math.sin(rad) * radius, -Math.cos(rad) * radius);

      from_abs    = new Point(center) + from_rel;
      through_abs = new Point(center) + through_rel;
      to_abs      = new Point(center) + to_rel;

  return new Path.Arc(from_abs,
                      through_abs,
                      to_abs);
}

// // Create a Paper.js Path to draw a line into it:
// var path = new Path();
// // Give the stroke a color
// path.strokeColor = 'black';
// var start = new Point(100, 100);
// // Move to start and draw a line from there
// path.moveTo(start);
// // Note the plus operator on Point objects.
// // PaperScript does that for us, and much more!
// path.lineTo(start + [ 100, -50 ]);

// var path = new Path.Circle({
//   center: view.center,
//   radius: 30,
//   strokeColor: 'black'
// });

// var path = new Path.Arc({
//     from: [20, 20],
//     through: [60, 20],
//     to: [80, 80],
//     strokeColor: 'black'
// });

// function onFrame(event)
// {
//   // document.getElementById('test').innerHTML  = event.count;
//   // document.getElementById('test').innerHTML += '<br>';
//   // document.getElementById('test').innerHTML += event.time;
//   // document.getElementById('test').innerHTML += '<br>';
//   // document.getElementById('test').innerHTML += event.delta;
// }

/*
var path_obj = {
  from: [0, 0],
  through: [Math.sin(Math.PI / 4) * 100, 100 - Math.cos(Math.PI / 4) * 100],
  to: [100, 100],
  strokeColor: 'red'
};

var circle = new Path.Circle(path_obj.through, 3);
circle.strokeColor = 'green';
*/

// path = new Path.Arc(path_obj);

// path = new Path.Arc({
//   from: [0, 0],
//   through: [50,50],
//   to: [0, 100],
//   strokeColor: 'black'
// });

// path = new Path.Arc({
//   from: [0, 0],
//   through: [Math.sin(1) * 50, Math.cos(1) * 50],
//   to: [50, 50],
//   strokeColor: 'red'
// });

// console.log('from: ' + path_obj.from + '; to: ' + path_obj.to + '; through: ' + path_obj.through);
// console.log('sin(Math.PI / 4): ' + Math.sin(Math.PI / 4) + '; cos(Math.PI / 4): ' + Math.cos(Math.PI / 4));

// drawArc(new Point(100, 100), 90, 100);
// drawArc(new Point(157, 157), 90, 123);
// // drawArc(new Point(57, 57), 123, 44);
// // drawArc(new Point(50, 70), 180, 60);
// drawArc(new Point(70, 70), 350, 60);
// // path.strokeColor = 'red';

var center = new Point(175, 200);

var hor_radius = 100;
var min_radius = 120;
var sec_radius = 140;

var hor_strokewidth = 20;
var min_strokewidth = 20;
var sec_strokewidth = 20;

// var hor_path = drawArc(center, 0, hor_radius);
// var min_path = drawArc(center, 0, min_radius);
// var sec_path = drawArc(center, 0, sec_radius);

// view.viewSize = new Size(400, 400);

setInterval(function() {

  var date = new Date();
  var hours   = ('00' + date.getHours()).slice(-2);
  var minutes = ('00' + date.getMinutes()).slice(-2);
  var seconds = ('00' + date.getSeconds()).slice(-2);

  // document.getElementById('test').innerHTML = hours + ':' + minutes + ':' + seconds;

  project.clear();

  /*
  var hor_circle = new Path.Circle(center, hor_radius - hor_strokewidth / 2);
  hor_circle.strokeColor = 'black';

  var min_circle = new Path.Circle(center, min_radius - min_strokewidth / 2);
  min_circle.strokeColor = 'black';

  var sec_circle = new Path.Circle(center, sec_radius - sec_strokewidth / 2);
  sec_circle.strokeColor = 'black';
  */

  var hor_angle = (date.getHours() / 24) * 360;
      min_angle = (date.getMinutes() / 60) * 360;
      sec_angle = (date.getSeconds() / 60) * 360;

  if (hor_angle == 0) {
    hor_angle = 360;
  }

  if (min_angle == 0) {
    min_angle = 360;
  }

  if (sec_angle == 0) {
    sec_angle = 360;
  }

//   var hor_path = drawArc(center, hor_angle, hor_radius);
//   hor_path.strokeWidth = hor_strokewidth;
//   hor_path.strokeColor = 'red';
//
//   var min_path = drawArc(center, min_angle, min_radius);
//   min_path.strokeWidth = min_strokewidth;
//   min_path.strokeColor = 'green';
//
//   var sec_path = drawArc(center, sec_angle, sec_radius);
//   sec_path.strokeWidth = sec_strokewidth;
//   sec_path.strokeColor = 'blue';

//   {
//   var radius_1 = 100;
//   var center_1 = new Point(300,200);
//
//   var circle_1 = new Path.Circle(center_1, radius_1);
//   circle_1.strokeWidth = 30;
//   circle_1.strokeColor = 'red';
//
//   var circle_2 = drawArc(center_1, 270, radius_1);
//   circle_2.strokeWidth = 30;
//   circle_2.strokeColor = 'green';
//   }

//   {
//     var radius_1 = 200;
//     var center_1 = new Point(300,400);
//
//     var arc = drawArc(center_1, 90, radius_1);
//     arc.strokeWidth = 40;
//     arc.strokeColor = 'green';
//   }

//   {
//     var radius_1 = 240;
//     var center_1 = new Point(300,400);
//
//     var circle = new Path.Circle(center_1, radius_1);
//     circle.strokeWidth = 40;
//     circle.strokeColor = 'pink';
//   }

  /*
  var ct = new Point(300, 400);
  var rd = 230;
  var sw = 100;

  {
    var circle = new Path.Circle(ct, rd);
    circle.strokeWidth = sw;
    circle.strokeColor = 'green';
  }

  {
    rd += sw;

    var circle = new Path.Circle(ct, rd);
    circle.strokeWidth = sw;
    circle.strokeColor = 'pink';
  }
  */

  // var curvePath = new Path('M300,200 h-150 a150,150 0 1,0 150,-150 z');
  // var curvePath = new Path('M300,200 h-150 a150,150 0 1,0 150,-150 z');
  var cp1 = new Path('M150,200 a150,150 0 1,0 150,-150');
  cp1.strokeWidth = 40;
  cp1.strokeColor = 'red';

  var cp2 = new Path('M110,200 a190,190 0 1,0 190,-190');
  cp2.strokeWidth = 40;
  cp2.strokeColor = 'blue';

  paper.view.update();

}, 1000);
