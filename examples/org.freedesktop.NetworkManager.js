var nmService =  'org.freedesktop.NetworkManager';

// https://developer.gnome.org/NetworkManager/unstable/spec.html#type-NM_DEVICE_STATE
var NM_DEVICE_STATE_DISCONNECTED = 30;
var NM_DEVICE_STATE_PREPARE      = 40;
var NM_DEVICE_STATE_ACTIVATED    = 100;

function nmGetDevicePath(deviceName)
{
  var path   = '/org/freedesktop/NetworkManager';
  var iface  = 'org.freedesktop.NetworkManager';
  var method = 'GetDeviceByIpIface';
  return dbus.system.call(nmService, path, iface, method, deviceName);
}

function nmGetActiveAccessPointPath(wlanPath)
{
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.NetworkManager.Device.Wireless';
  var arg2   = 'ActiveAccessPoint';
  return dbus.system.call(nmService, wlanPath, iface, method, arg1, arg2);
}

function nmGetEssid(activeAccessPointPath)
{
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.NetworkManager.AccessPoint';
  var arg2   = 'Ssid';
  var essid = dbus.system.call(nmService, activeAccessPointPath, iface, method, arg1, arg2);

  var essidStr = '';
  for (var i = 0; i < essid.length; ++i) {
    essidStr += String.fromCharCode(essid[i]);
  }

  return essidStr;
}

function nmGetStrength(activeAccessPointPath)
{
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.NetworkManager.AccessPoint';
  var arg2   = 'Strength';
  return dbus.system.call(nmService, activeAccessPointPath, iface, method, arg1, arg2).charCodeAt(0);
}

function nmGetActiveConnectionId(deviceName)
{
  var devPath = nmGetDevicePath(deviceName);
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.NetworkManager.Device';
  var arg2   = 'ActiveConnection';

  var acPath = dbus.system.call(nmService, devPath, iface, method, arg1, arg2);

  arg1 = 'org.freedesktop.NetworkManager.Connection.Active';
  arg2 = 'Id';
  return dbus.system.call(nmService, acPath, iface, method, arg1, arg2);
}

function nmGetDeviceState(devPath)
{
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.NetworkManager.Device';
  var arg2   = 'State';
  return dbus.system.call(nmService, devPath, iface, method, arg1, arg2);
}
