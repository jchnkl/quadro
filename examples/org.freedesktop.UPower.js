var upowerService =  'org.freedesktop.UPower';

function upowerOnBattery()
{
  var path   = '/org/freedesktop/UPower';
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.UPower';
  var arg2   = 'OnBattery';
  return dbus.system.call(upowerService, path, iface, method, arg1, arg2);
}

function upowerGetDeviceProperty(devicePath, property)
{
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.UPower.Device';
  return dbus.system.call(upowerService, devicePath, iface, method, arg1, property);
}

function upowerGetDeviceProperties(devicePath)
{
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'GetAll';
  var arg1   = 'org.freedesktop.UPower.Device';
  return dbus.system.call(upowerService, devicePath, iface, method, arg1);
}
