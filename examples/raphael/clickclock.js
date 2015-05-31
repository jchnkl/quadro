
var size = { width: 400, height: 400 };

var center = { x: size.width / 2, y: size.height / 2 };

var fontFamily = 'Ubuntu Light';
var hourFontSize = 65;
var hourFontColor = '#85b3c4';
var hourFontFamily = fontFamily;

var minuteFontSize = 55;
var minuteTrailLength = 15;
var minuteFontColor = '#b7ba52';
var minuteFontFamily = fontFamily;

var secondFontSize = 40;
var secondTrailLength = 15;
var secondFontColor = '#eea551';
var secondFontFamily = fontFamily;

// var minuteRadius = 1 / 1.0 * size.width - 2 * minuteFontSize;
// var secondRadius = 1 / 1.5 * size.width - 2 * secondFontSize;

var secondRadius = (size.width - 2 * secondFontSize) / 2;
var minuteRadius = (2 / 3) * (size.width - 2 * minuteFontSize) / 2;

function toRad(angle)
{
  return angle / 180.0 * Math.PI;
}

function circlePoint(center, radius, angle)
{
  var rad = toRad(angle);
  return { x: center.x + Math.sin(rad) * radius,
           y: center.y - Math.cos(rad) * radius
         };
}

function timeText(paper, pos, time, size, color, family)
{
  return paper.text(pos.x, pos.y, time)
              .attr({ 'font-size': size
                    , 'fill': color
                    , 'font-family': family
                    });
}

function makeTimeSet(paper, time, center, radius, trail, size, color, family)
{
  var set = paper.set();

  var opacity  = 0.5;
  var fontsize = 0.8;

  for (var tl = 1; tl < trail + 1; ++tl) {
    var t = (time - tl) % 60;

    if (t < 0) {
      t += 60;
    }

    var a = (t / 60) * 360;
    var p = circlePoint(center, radius, a);

    var text = timeText(paper, p, t, size * fontsize, color, family);
    text.attr({ 'fill-opacity': text.attr('fill-opacity') * opacity });

    opacity  *= 0.90;
    fontsize *= 0.97;

    set.push(text);
  }

  var ang = (time / 60) * 360;
  var pos = circlePoint(center, radius, ang);
  var text = timeText(paper, pos, time, size * 1.00, color, family);
  text.attr({ 'font-weight': 'bold' });
  set.push(text);

  return set.hide();
}

function makeSeconds(paper, center)
{
  var seconds = [];

  for (var second = 0; second < 60; ++second) {
    seconds.push(makeTimeSet(paper,
                             second,
                             center,
                             secondRadius,
                             secondTrailLength,
                             secondFontSize,
                             secondFontColor,
                             secondFontFamily));
  }

  return seconds;
}

function makeMinutes(paper, center)
{
  var minutes = [];

  for (var minute = 0; minute < 60; ++minute) {
    minutes.push(makeTimeSet(paper,
                             minute,
                             center,
                             minuteRadius,
                             minuteTrailLength,
                             minuteFontSize,
                             minuteFontColor,
                             minuteFontFamily));
  }

  return minutes;
}

function makeHour(paper, center, hour)
{
  return timeText(paper, center, hour, hourFontSize * 1.00, hourFontColor, hourFontFamily)
              .attr({ 'font-weight': 'bold' });
}

function onLoad()
{
  console.log('onLoad();');

  var paper = new Raphael(0, 0, size.width, size.height);

  // var time = paper.text(center.x, center.y, '');
  // console.log(hour + ':' + minute + ':' + second);

  var seconds = makeSeconds(paper, center);
  var minutes = makeMinutes(paper, center);

  var currentSecondsSet = seconds[0];
  var currentMinutesSet = minutes[0];
  var currentHourText = makeHour(paper, center, 0);

  var currentMinute = null;
  var currentHour   = null;

  (function () {

    var now = new Date();
    var hour   = now.getHours();
    var minute = now.getMinutes();
    var second = now.getSeconds();

    currentSecondsSet.hide();
    currentSecondsSet = seconds[second];
    currentSecondsSet.show();

    if (minute != currentMinute) {
      currentMinute = minute;
      currentMinutesSet.hide();
      currentMinutesSet = minutes[minute];
      currentMinutesSet.show();
    }

    if (hour != currentHour) {
      currentHour = hour;
      currentHourText.remove();
      currentHourText = makeHour(paper, center, hour);
    }


    // time.remove();
    // time = paper.text(center.x, center.y, hour + ':' + minute + ':' + second);

    setTimeout(arguments.callee, 1000);
  })();

// setInterval(function() {
//
//   var now = new Date();
//   var hour   = now.getHours();
//   var minute = now.getMinutes();
//   var second = now.getSeconds();
//
//   console.log(hour + ':' + minute + ':' + second);
//
//   // for (var i = 0; i < 360; i += 10) {
//     var p = circlePoint({ x: 100, y: 100 }, secondRadius, second);
//     paper.text(p.x, p.y, second);
//   // }
//
// }, 1000);

}
