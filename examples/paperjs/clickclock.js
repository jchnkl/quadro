var center = view.size / 2;

var cutOff = 40;

var hoursFontSize = 50;
var hoursFontColor = '#85b3c4';
var minuteFontSize = 40;
var minuteFontColor = '#b7ba52';
var minuteFontDimmedColor = '#cacd81';
var secondFontSize = 30;
var secondFontColor = '#eea551';
var secondFontDimmedColor = '#f5ca99';

// var hoursFontSize = 50;
// var hoursFontColor = '#ffffff';
// var minuteFontSize = 40;
// var minuteFontColor = '#ffffff';
// var minuteFontDimmedColor = '#000000';
// var secondFontSize = 30;
// var secondFontColor = '#ffffff';
// var secondFontDimmedColor = '#000000';

var minuteRadius = 1 / 3 * (view.size - 3 * minuteFontSize).width;
var secondRadius = 1 / 2 * (view.size - 3 * secondFontSize).width;

function toRad(angle)
{
  return angle / 180.0 * Math.PI;
}

function circlePoint(center, radius, angle)
{
  var rad         = toRad(angle);
      to_rel      = new Point(Math.sin(rad) * radius, -Math.cos(rad) * radius);
  return new Point(center) + to_rel;
}

function handText(pos, text, fsize, fcolor)
{
  var pointText = new PointText({
    point: pos,
    justification: 'center',
    fontWeight: 'normal',
    fontSize: fsize,
    fillColor: fcolor
  });

  pointText.content = text;

  return pointText;
}

function makeHand(center, radius, angle, text, fsize, fcolor)
{
  var pos = circlePoint(center, radius, angle);
  return handText(pos, text, fsize, fcolor);
}

function drawHand(layer, center, radius, angle, text, fsize, fcolor)
{
  // remove last item
  layer.removeChildren(0, 1);

  console.log('layer.children.length: ' + layer.children.length);
  for (var i = 0; i < layer.children.length; ++i) {
    var item = layer.children[i];
    console.log('child.content: ' + item.content);
    item.opacity *= 0.95;
    item.fontSize *= 0.95;
  }
}

function hourHand(layer, hours)
{
  layer.removeChildren();

  var pointText = new PointText({
    point: center,
    justification: 'center',
    fontWeight: 'bold',
    fontSize: hoursFontSize,
    fillColor: hoursFontColor
  });

  pointText.content = hours;

  layer.addChildren(pointText);
}

var hoursLayer  = new Layer();
var minuteLayer = new Layer();
var secondLayer = new Layer();

function desaturate(c) {
  var intensity = 0.3 * c.r + 0.59 * c.g + 0.11 * c.b;
  var k = 1;
  // var r = Math.floor(intensity * k + c.r * (1 - k));
  // var g = Math.floor(intensity * k + c.g * (1 - k));
  // var b = Math.floor(intensity * k + c.b * (1 - k));
  return {
  red: Math.floor(intensity * k + c.r * (1 - k)),
  green: Math.floor(intensity * k + c.g * (1 - k)),
  blue: Math.floor(intensity * k + c.b * (1 - k))
  };
  // return new Color
  // return [r, g, b];
}

function makeHandLayer(center, radius, fsize, fcolor, dcolor, time)
{
  var start = time - cutOff;

  if (start < 0) {
    start = 60 + start;
  }

  var opaqfactor = 0.5;
  var sizefactor = 1.0;

  var children = [];

  // var color = new Color(dcolor);
  var color = new Color(fcolor).convert('hsb');
  // var color = desaturate(new Color(dcolor));
  // var color = new Color(dcolor).convert('gray');
  // color.gray = 0.5;
  color.saturation = 0.4;
  // color.brightness = 0.0;
  // console.log(color.type);

  for (var i = cutOff - 1; i > -1; --i) {
    var cur = (start + i) % 60;
    var angle = (cur / 60) * 360;
    var h = makeHand(center, radius, angle, cur, fsize, fcolor);
    h.fillColor = color;
    // h.fillColor.gray = 1.0;
    h.strokeColor = color;
    // h.fontColor = dcolor;
    h.opacity  *= opaqfactor;
    h.fontSize *= sizefactor;
    opaqfactor *= 0.95;
    sizefactor *= 0.97;
    children.push(h);
  }

  children[0].opacity = 1.0;
  children[0].fillColor = fcolor;
  children[0].strokeWidth *= 3.0;
  children[0].strokeColor = fcolor;
  children[0].fontSize = children[0].fontSize * 1.2;

  var layer = new Layer();

  layer.addChildren(children);
  layer.reverseChildren();

  return layer;
}

// function init(center, layer, radius, fsize, fcolor, minsec)
// {
//   var start = minsec - cutOff;
//
//   if (start < 0) {
//     start = 60 + start;
//   }
//
//   for (var i = 0; i < cutOff; ++i) {
//     var cur = (start + i) % 60;
//     var angle = (cur / 60) * 360;
//     var h = makeHand(center, radius, angle, cur, fsize, fcolor);
//     layer.addChild(h);
//   }
//
//   var opfactor = 1.0;
//   var fsfactor = 1.0;
//
//   for (var i = cutOff - 1; i >= 0; --i) {
//     var h = layer.children[i];
//     fsfactor -= 0.05;
//     opfactor -= 0.05;
//     h.opacity = opfactor * h.opacity;
//     h.fontSize = fsfactor * h.fontSize;
//   }
// }

// var lastHour = null;
// var lastMinute = null;
// // var lastMinute = null;

// var d_minutes = 0;
// var d_seconds = 0;

// initMinutes();
// initSeconds();

// var now = new Date();

// init(center, minuteLayer, minuteRadius, minuteFontSize, minuteFontColor, now.getMinutes());
// init(center, secondLayer, secondRadius, secondFontSize, secondFontColor, now.getSeconds());

var seconds = [];
var minutes = [];

for (var i = 0; i < 60; ++i) {
  seconds.push(makeHandLayer(center, secondRadius, secondFontSize, secondFontColor, secondFontDimmedColor, i));
  // seconds[i].visible = false;
  seconds[i].remove();
}

for (var i = 0; i < 60; ++i) {
  minutes.push(makeHandLayer(center, minuteRadius, minuteFontSize, minuteFontColor, minuteFontDimmedColor, i));
  // minutes[i].visible = false;
  minutes[i].remove();
}

// var now = new Date();
// var currentHour = now.getHours();
// var currentSecond = now.getSeconds();
// var currentMinute = now.getMinutes();

var currentHour   = null;
var currentSecond = null;
var currentMinute = null;

// var currentLayer = layers[currentSecond];
// currentLayer.activate();

var secondLayer = new Layer();
var minuteLayer = new Layer();
var hourLayer   = new Layer();

// project.activeLayer.addChild(hourLayer);
// project.activeLayer.addChild(minuteLayer);
// project.activeLayer.addChild(secondLayer);

setInterval(function() {

  // seconds[currentSecond].visible = false;
  // currentSecond = ++currentSecond % 60;
  // seconds[currentSecond].visible = true;

  var date = new Date();
  var hour   = date.getHours();
  var second = date.getSeconds();
  var minute = date.getMinutes();

  // project.activeLayer.removeChildren();

  currentSecond = second;
  // project.activeLayer.addChild(seconds[currentSecond]);
  secondLayer.removeChildren();
  secondLayer.addChild(seconds[currentSecond]);

  if (minute != currentMinute) {
    console.log('minute: ' + minute);
    currentMinute = minute;
    // project.activeLayer.addChild(minutes[currentMinute]);
    minuteLayer.removeChildren();
    minuteLayer.addChild(minutes[currentMinute]);
  }

  if (hour != currentHour) {
    console.log('hour: ' + hour);
    currentHour = hour;

    var hourText = new PointText({
      point: center,
      justification: 'center',
      fontWeight: 'bold',
      fontSize: hoursFontSize * 1.2,
      fillColor: hoursFontColor,
      strokeColor: hoursFontColor
    });

    hourText.strokeWidth *= 2.0,
    hourText.content = hour;

    // hourLayer.removeChildren();
    // hourLayer.addChildren(hourText);

    // hourLayer.activate();
  }

  paper.view.update();

}, 1000);

// setInterval(function() {
//
//   var date = new Date();
//
//   var hours = date.getHours();
//   var minutes = date.getMinutes();
//   var seconds = date.getSeconds();
//   // var minutes = (d_minutes + 1) % 60;
//   // d_minutes += 1;
//   // var seconds = (d_seconds + 1) % 60;
//   // d_seconds += 1;
//
//   // var hor_angle = (hours % 12 / 12) * 360;
//   var minuteAngle = (minutes / 60) * 360;
//   var secondsAngle = (seconds / 60) * 360;
//
//   if (hours != lastHour) {
//     console.log('hours');
//     lastHour = hours;
//     hourHand(hoursLayer, hours);
//   }
//
//   if (minutes != lastMinute) {
//     console.log('minutes');
//     lastMinute = minutes;
//     hand(minuteLayer, center, minuteRadius, minuteAngle, minutes,
//          minuteFontSize, minuteFontColor);
//   }
//
//     hand(secondLayer, center, secondRadius, secondsAngle, seconds,
//          secondFontSize, secondFontColor);
//
// //   var secondsPosition = arcPath(center, sec_angle, secondRadius).to;
// //
// //   if (secondLayer.lastChild) {
// //     var item = secondLayer.lastChild;
// //     item.fontSize = 2 * item.fontSize / 3;
// //     secondLayer.lastChild.fontWeight = 'normal';
// //   }
// //
// //   for (var i = 0; i < secondLayer.children.length; ++i) {
// //     var item = secondLayer.children[i];
// //     item.opacity *= 0.95;
// //     item.fontSize *= 0.95;
// //   }
// //
// //   if (secondLayer.children.length > 45) {
// //     secondLayer.removeChildren(0, secondLayer.children.length - 45);
// //   }
// //
// //   var secondsText = new PointText({
// //     point: secondsPosition,
// //     justification: 'center',
// //     fontWeight: 'bold',
// //     fontSize: secondFontSize,
// //     fillColor: secondFontColor
// //   });
// //
// //   secondsText.content = seconds;
// //
// //   secondLayer.appendTop(secondsText);
//
//   paper.view.update();
//
//   // console.log('seconds: ' + seconds + '; secondsPosition: ' + secondsPosition);
//
// }, 1000);
