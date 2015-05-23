function toRad(angle)
{
  return angle / 180.0 * Math.PI;
}

function arcPath(center, angle, radius)
{
  var rad         = toRad(angle);
      from_rel    = new Point(0, -radius);
      through_rel = new Point(Math.sin(rad / 2) * radius, -Math.cos(rad / 2) * radius);
      to_rel      = new Point(Math.sin(rad) * radius, -Math.cos(rad) * radius);

      from_abs    = new Point(center) + from_rel;
      through_abs = new Point(center) + through_rel;
      to_abs      = new Point(center) + to_rel;

  return { from: from_abs, through: through_abs, to: to_abs };
}

var center = new Point(250, 250);

var opacity = 0.25;

var hor_radius = 175;
var min_radius = 149;
var sec_radius = 133;

var hor_strokewidth = 30;
var min_strokewidth = 20;
var sec_strokewidth = 10;

var bg_circle = new Path.Circle(center, sec_radius + sec_strokewidth / 2 + 10);
bg_circle.fillColor = '#f0f0f0';
bg_circle.opacity = 0.5;

// TODO: hor layer, min layer, sec layer

var hor_circle = new Path.Arc(arcPath(center, 360, hor_radius));
hor_circle.opacity = opacity;
hor_circle.strokeWidth = hor_strokewidth;
hor_circle.strokeColor = '#8f8f8f';

var min_circle = new Path.Arc(arcPath(center, 360, min_radius));
min_circle.opacity = opacity;
min_circle.strokeWidth = min_strokewidth;
min_circle.strokeColor = '#8f8f8f';

var sec_circle = new Path.Arc(arcPath(center, 360, sec_radius));
sec_circle.opacity = opacity;
sec_circle.strokeWidth = sec_strokewidth;
sec_circle.strokeColor = '#8f8f8f';

var base_layer = new Layer([bg_circle, hor_circle, min_circle, sec_circle]);

var hands_layer = new Layer();

var hor_layer = new Layer();
var min_layer = new Layer();
var sec_layer = new Layer();

var hour = null;
var minute = 59;
var second = 57;

function hand(options)
{
  var center      = options.center;
      angle       = options.angle;
      radius      = options.radius;
      strokeWidth = options.strokeWidth;
      strokeColor = options.strokeColor;
      headColor   = options.headColor;
      arcColor    = options.arcColor;
      tailColor   = options.tailColor;
      textColor   = options.textColor;
      textContent = options.textContent;
      fontSize    = options.fontSize;
      fontFamily  = options.fontFamily;

  var path = arcPath(center, angle, radius);

  // var head = new Path.Circle(path.to, strokeWidth / 2);
  // head.fillColor = headColor;
  // head.strokeWidth = 2;
  // head.strokeColor = arcColor;

  var arc = new Path.Arc(path);
  arc.strokeWidth = strokeWidth;
  arc.strokeColor = arcColor;

  // var tail = new Path.Circle(path.from, strokeWidth / 2);
  // tail.fillColor = tailColor;

  // var text = new PointText(path.to);
  // text.content = textContent;
  // text.fillColor = textColor;
  // text.fontSize = fontSize;
  // text.fontFamily = fontFamily;
  // text.justification = 'center';
  // text.position = path.to;

  // return { head: head, arc: arc, tail: tail, text: text };
  // return { head: head, arc: arc, text: text };
  // return { head: head, arc: arc };
  return { arc: arc };
}

setInterval(function() {

  var date = new Date();

  // var cur_hour = 0;
  // var cur_minute = 0;
  // var cur_second = 0;

  var cur_hour = date.getHours();
  var cur_minute = date.getMinutes();
  // var cur_minute = (minute + 1) % 60;
  var cur_second = date.getSeconds();
  // var cur_second = (second + 1) % 60;

  if (cur_hour != hour) {
    console.log('cur_hour != hour');

    if (cur_hour == 0) {
      cur_hour = 24;
    }

    hour = cur_hour;
    var hours = ('00' + date.getHours()).slice(-2);
    var hor_angle = (cur_hour % 12 / 12) * 360;

    var hor_options =
        { center      : center
        , angle       : hor_angle
        , radius      : hor_radius
        , strokeWidth : hor_strokewidth
        , strokeColor : '#FFBA00'
        , headColor   : '#997000'
        , arcColor    : '#FFBA00'
        , tailColor   : '#FFBA00'
        , textColor   : '#FFBA00'
        , textContent : hours
        , fontSize    : '20'
        , fontFamily  : 'Ubuntu Light'
        };

    var hor_hand = hand(hor_options);

    hor_layer.removeChildren();
    hor_layer.addChildren(
        // [hor_hand.arc, hor_hand.tail, hor_hand.head, hor_hand.text]);
        // [hor_hand.arc, hor_hand.head, hor_hand.text]);
        // [hor_hand.arc, hor_hand.head]);
        [hor_hand.arc]);
  }

  if (cur_minute != minute) {
    console.log('cur_minute != minute');

    if (cur_minute == 0) {
      cur_minute = 60;
    }

    minute = cur_minute;
    var minutes = ('00' + date.getMinutes()).slice(-2);
    var min_angle = (cur_minute / 60) * 360;

    var min_options =
        { center      : center
        , angle       : min_angle
        , radius      : min_radius
        , strokeWidth : min_strokewidth
        , strokeColor : '#8EBB00'
        , headColor   : '#557000'
        , arcColor    : '#8EBB00'
        , tailColor   : '#8EBB00'
        , textColor   : '#8EBB00'
        , textContent : minutes
        , fontSize    : '20'
        , fontFamily  : 'Ubuntu Light'
        };

    var min_hand = hand(min_options);

    min_layer.removeChildren();
    min_layer.addChildren(
        // [min_hand.arc, min_hand.tail, min_hand.head, min_hand.text]);
        // [min_hand.arc, min_hand.head, min_hand.text]);
        // [min_hand.arc, min_hand.head]);
        [min_hand.arc]);
  }

  if (cur_second != second) {

    if (cur_second == 0) {
      cur_second = 60;
    }

    // console.log('cur_second != second: ' + seconds);

    // if (cur_second == 0) {
    //   second = null;
    // }

    second = cur_second;
    var seconds = ('00' + cur_second).slice(-2);
    var sec_angle = (second / 60) * 360;

    var sec_options =
        { center      : center
        , angle       : sec_angle
        , radius      : sec_radius
        , strokeWidth : sec_strokewidth
        , strokeColor : '#00A9EF'
        , headColor   : '#00658F'
        , arcColor    : '#00A9EF'
        , tailColor   : '#00A9EF'
        , textColor   : '#00A9EF'
        , textContent : seconds
        , fontSize    : '20'
        , fontFamily  : 'Ubuntu Light'
        };

    var sec_hand = hand(sec_options);

    sec_layer.removeChildren();
    sec_layer.addChildren(
        // [sec_hand.arc, sec_hand.tail, sec_hand.head, sec_hand.text]);
        // [sec_hand.arc, sec_hand.head, sec_hand.text]);
        // [sec_hand.arc, sec_hand.head]);
        [sec_hand.arc]);
  }

  // hands_layer.addChildren([ hor_hand.arc, hor_hand.tail, hor_hand.head, hor_hand.text
  //                         , min_hand.arc, min_hand.tail, min_hand.head, min_hand.text
  //                         , sec_hand.arc, sec_hand.tail, sec_hand.head, sec_hand.text
  //                         ]);

  paper.view.update();

}, 1000);
