function init(root)
{
  var iconDiv = document.createElement('div');
  iconDiv.id = 'icon';
  root.appendChild(iconDiv);
}

// e.g.
// classes = 4 => 0/25/50/75/100
// value = 33 => class 25
// value = 38 => class 50
function percentClass(classes, percent)
{
  var n = 100 / classes;
  return Math.round(percent/n) * n;
}

function iconPath()
{
  return '/usr/share/icons/elementary/status/48/';
}

function iconClass(percent)
{
  var icons = {   0 : '000'
              ,  20 : '020'
              ,  40 : '040'
              ,  60 : '060'
              ,  80 : '080'
              , 100 : '100'
              };
  return icons[percentClass(5, percent)];
}

function regularIcon(percent)
{
  return iconPath() + 'battery-' + iconClass(percent) + '.svg';
}

function remainingRunningTime(bat)
{
  var rem = TpSmapi.remainingRunningTime(0);
  if (rem != null) {
    return { hours: Math.floor(rem / 60)
           , minutes: ('00' + rem % 60).slice(-2)
           };
  } else {
    return null;
  }
}

// '/org/freedesktop/UPower/devices/battery_BAT0'
