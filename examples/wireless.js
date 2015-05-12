// IMPORTANT:
// Requires 'org.freedesktop.NetworkManager.js'
// <script src="org.freedesktop.NetworkManager.js" type='text/javascript'></script>

var Wireless = function (device)
{
  // public
  this.device = null;

  // private
  this.essidDiv    = null;
  this.strengthDiv = null;
  this.apPath      = null;
  this.apSignal    = null;

  this.run                   = run;
  this.showEssid             = showEssid;
  this.showStrength          = showStrength;
  this.switchState           = switchState;

  this.onState               = onState;
  this.onDisconnected        = onDisconnected;
  this.onPrepare             = onPrepare;
  this.onActivated           = onActivated;

  this.installStrengthSignal = installStrengthSignal;
  this.removeStrengthSignal  = removeStrengthSignal;
  this.onStrengthChange      = onStrengthChange;
};

function run(device)
{
  this.device = device;

  // create essid div; child container of wireless
  this.essidDiv = document.createElement('div');
  this.essidDiv.id = 'essid';

  // create strength div; child container of wireless
  this.strengthDiv = document.createElement('div');
  this.strengthDiv.id = 'strength';

  // get device path for this.device (e.g. 'wlan0')
  var devPath = nmGetDevicePath(this.device);

  // get state and initialize view
  this.switchState(nmGetDeviceState(devPath));

  // get signal object for StateChanged signal for this.device
  var signal = dbus.system.signal(
      'org.freedesktop.NetworkManager',
      devPath,
      'org.freedesktop.NetworkManager.Device',
      'StateChanged');

  // connect to StateChanged signals with callback
  signal.notify.connect(this, onState);
}

function showEssid(essid)
{
  this.essidDiv.innerHTML = essid;
}

function showStrength(strength)
{
  this.strengthDiv.innerHTML = wlanIcon(strength);
}

function onState(msg)
{
  if (msg.signal == 'StateChanged') {
    this.switchState(msg.contents[0]);
  }
}

function switchState(state)
{
  if (state == NM_DEVICE_STATE_DISCONNECTED) {
    this.onDisconnected();
  } else if (state == NM_DEVICE_STATE_PREPARE) {
    this.onPrepare();
  } else if (state == NM_DEVICE_STATE_ACTIVATED) {
    this.onActivated();
  }
}

function onDisconnected()
{
  this.essidDiv.innerHTML = 'Disconnected';
  this.strengthDiv.innerHTML = '';
  this.removeStrengthSignal();
}

function onPrepare()
{
  this.essidDiv.innerHTML = 'Connecting to ' + nmGetActiveConnectionId(this.device);
}

function onActivated()
{
  this.apPath = nmGetActiveAccessPointPath(nmGetDevicePath(this.device));
  this.showEssid(nmGetEssid(this.apPath));
  this.showStrength(nmGetStrength(this.apPath));
  this.installStrengthSignal();
}

function removeStrengthSignal()
{
  if (this.apSignal != null) {
    this.apSignal.notify.disconnect(this, onStrengthChange);
  }
  this.apSignal = null;
}

function installStrengthSignal()
{
  this.apSignal = dbus.system.signal(
      'org.freedesktop.NetworkManager',
      this.apPath,
      'org.freedesktop.NetworkManager.AccessPoint',
      'PropertiesChanged');
  this.apSignal.notify.connect(this, onStrengthChange);
}

function onStrengthChange(obj)
{
  var strength = obj['Strength'].charCodeAt(0);
  if (strength != null) {
    this.showStrength(strength);
  }
}

function wlanIcon(strength)
{
  var iconbase = 'nm-signal-';

  var icons = {   0 : iconbase + '0'
              ,  25 : iconbase + '25'
              ,  50 : iconbase + '50'
              ,  75 : iconbase + '75'
              , 100 : iconbase + '100'
              };

  function imagePath(img)
  {
    return 'file:///usr/share/icons/elementary/panel/48/' + img + '.svg';
  }

  var icon = icons[percentClass(4, strength)];
  var src = imagePath(icon);
  var img = '<img id=\"icon\" src=\"' + src + '\"'
          + ' alt=\"' +  strength + '\"></img>';

  return img;
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
