var nmService =  'org.freedesktop.NetworkManager';

// {
//   try {
//     var iconbase = 'gpm-battery-';
//     var icons = { 000 : 'gpm-battery-000'
//                 , 020 : 'gpm-battery-020'
//                 , 040 : 'gpm-battery-040'
//                 , 060 : 'gpm-battery-060'
//                 , 080 : 'gpm-battery-080'
//                 , 100 : 'gpm-battery-100'
//                 };
//
//     function strengthIcon(strength)
//     {
//       if (strength < 0) {
//         return icons[000];
//       } else if (strength >= 0  && strength < 20) {
//         return icons[020];
//       } else if (strength >= 20 && strength < 40) {
//         return icons[040];
//       } else if (strength >= 40 && strength < 60) {
//         return icons[060];
//       } else if (strength >= 60 && strength < 80) {
//         return icons[080];
//       } else if (strength >= 80) {
//         return icons[100];
//       }
//     }
//
//     function imagePath(img)
//     {
//       return 'file:///usr/share/icons/elementary/panel/48/' + img + '.svg';
//     }
//
//     var strength = obj['Strength'].charCodeAt(0);
//     var alt = 'Signal strength: ' + strength;
//     var img = '<img id=\"wlanimg\" src=\"' + imagePath(strengthIcon(strength)) + '\" alt=\"' + alt + '\"></img>';
//
//     document.getElementById('wifi').innerHTML = img;
//   } catch (e) {
//     document.getElementById('wifi').innerHTML = e;
//   }
// }

  // html += 'jsDBusBridge.name: ' + jsDBusBridge.name;
  // html += '<br>' + 'current network: ' + jsDBusBridge.currentNetwork;
  // html += '<br>' + 'connection status: ' + jsDBusBridge.connectionStatus;
  // html += '<br>' + 'connection status: ' + typeof jsDBusBridge.connectionStatus.toString();
  // html += '<br>' + 'connection status: ' + jsDBusBridge.connectionStatus.toJsonObject();
  // html += '<br>' + 'connection status: ' + typeof jsDBusBridge.connectionStatus.toJsonObject();
  // html += '<br>' + 'connection status: ' + jsDBusBridge.connectionStatus.toJsonObject;
  // html += '<br>' + 'connection status: ' + typeof jsDBusBridge.connectionStatus.toJsonObject;
  // html += '<br>' + 'typeof jsDBusBridge.connectionStatus: ' + typeof jsDBusBridge.connectionStatus;

  // for (var property in jsDBusBridge.connectionStatus) {
  //   html += '<br>' + 'property: ' + property;
  // }

  // html += '<br>' + 'connection status: ' + jsDBusBridge.connectionStatus.int32;
  // html += '<br>' + 'connection status: ' + jsDBusBridge.connectionStatus.array[0];
  // html += '<br>' + 'connection status: ' + jsDBusBridge.connectionStatus.array[0].string;

  // var obj = jsDBusBridge.variantMap;
  // var obj = jsDBusBridge.variantList;

  // try {
  //   html += showAllProperties(obj);
  //   jsDBusBridge.propertiesChanged.connect(updateWifiSignalStrength);
  //   html += '<br><br>ESSID: ' + obj[0]['Id'];
  // } catch (e) {
  //   html = e;
  // }

function updateInfoBox(obj)
{
  document.getElementById('infobox').innerHTML  = typeof obj;
  document.getElementById('infobox').innerHTML += '<br>';
  document.getElementById('infobox').innerHTML += obj;
  document.getElementById('infobox').innerHTML += '<br>';
  document.getElementById('infobox').innerHTML += showAllProperties(obj);
  document.getElementById('infobox').innerHTML += '<br>';
}

function updateWlan(obj)
{
  // document.getElementById('wlanbox').innerHTML = 'updateWlan';
  // document.getElementById('wlanbox').innerHTML = typeof obj;
  // document.getElementById('wlanbox').innerHTML += showAllProperties(obj);
  var strength = obj['Strength'].charCodeAt(0);
  document.getElementById('wlanbox').innerHTML = 'Strength: ' + strength;
  document.getElementById('wlanbox').innerHTML += '<br>';
  document.getElementById('wlanbox').innerHTML += wlanStrengthIcon(strength);

  if (obj['PrimaryConnection'] != null) {
    // dbus['propertiesChanged(QVariantMap)'].disconnect(updateWlan);

    dbus.systemConnectPropertyChanged('org.freedesktop.NetworkManager',
                                      nmGetActiveAccessPointPath(nmGetWlan0Path()),
                                      'org.freedesktop.NetworkManager.AccessPoint',
                                      'PropertiesChanged');
  }
}

function wlanStrengthIcon(strength)
{
  var iconbase = 'nm-signal-';
  var icons = { 000 : iconbase + '0'
              , 025 : iconbase + '25'
              , 050 : iconbase + '50'
              , 075 : iconbase + '75'
              , 100 : iconbase + '100'
              };

  function strengthIcon()
  {
    if (strength < 0) {
      return icons[000];
    } else if (strength >= 0  && strength < 25) {
      return icons[025];
    } else if (strength >= 25 && strength < 50) {
      return icons[050];
    } else if (strength >= 50 && strength < 75) {
      return icons[075];
    } else if (strength >= 75) {
      return icons[100];
    }
  }

  function imagePath(img)
  {
    return 'file:///usr/share/icons/elementary/panel/48/' + img + '.svg';
  }

  var imgsrc = imagePath(strengthIcon(strength));
  var img = '<img id=\"wlan-strength-icon\" src=\"' + imgsrc + '\"'
          + ' alt=\"' +  strength + '\"></img>';

  return img;
}

function showAllProperties(obj)
{
  var res = [];
  for (var property in obj) {
    var value = obj[property];
    res += ['<br>object[' + property + '] = \"' + value + '\" (type: ' + typeof value + ')'];
    if (typeof value == 'object' ) {
      res += '<br>{' + showAllProperties(value) + '<br>}';
    }
  }
  return res;
}

function loadJsDBusBridge()
{
  html = 'Here we go<br>';

  // try {
  //   html += showAllProperties(jsDBusBridge.variantList[0]);
  //   html += jsDBusBridge.variantList[0][0];
  // } catch (e) {
  //   html += e;
  // }

  try {
    html += 'typeof dbus: ' + typeof dbus;
    html += '<br>';
    // html += showAllProperties(dbus);
    // html += '<br>';
    html += 'typeof dbus.system: ' + typeof dbus.system;
    html += '<br>';
    // html += showAllProperties(dbus.system);
    // html += '<br>';
    html += 'typeof dbus.session: ' + typeof dbus.session;
    html += '<br>';
    // html += showAllProperties(dbus.session);
    // html += '<br>';

    {
      var path   = '/org/freedesktop/NetworkManager';
      var iface  = 'org.freedesktop.NetworkManager';
      var method = 'GetDeviceByIpIface';
      var arg    = 'wlan0';
      html += 'CALL: ' + dbus.system.call(nmService, path, iface, method, arg);
      html += '<br>';
    }

    var system  = dbus.system;
    var session = dbus.session;

    // dbus-send --session --print-reply --type=method_call
    //   --dest=org.gnome.Identity /org/gnome/Identity
    //   org.freedesktop.DBus.Introspectable.Introspect

    var managedObjects = session.call('org.gnome.Identity',
                                      '/org/gnome/Identity',
                                      'org.freedesktop.DBus.ObjectManager',
                                      'GetManagedObjects');

    html += typeof managedObjects;
    html += '<br>';
    html += 'showAllProperties(managedObjects):';
    html += '<br>';
    html += showAllProperties(managedObjects);
    html += '<br>';
    html += 'showAllProperties(managedObjects[\'/org/gnome/Identity/Manager\']):';
    html += '<br>';
    html += showAllProperties(managedObjects['/org/gnome/Identity/Manager']);
    html += '<br>';
    html += 'managedObjects[\'/org/gnome/Identity/Manager\']:';
    html += '<br>';
    html += managedObjects['/org/gnome/Identity/Manager'];
    html += '<br>';

    html += 'typeof system: ' + typeof system;
    html += '<br>';
    html += 'typeof session: ' + typeof session;
    html += '<br>';
    // html += 'showAllProperties session: ' + showAllProperties(session);
    // html += '<br>';

    // html += 'typeof dbus.signal: ' + typeof dbus.signal;
    // html += '<br>';
    // html += 'dbus.signal: ' + dbus.signal;
    // html += '<br>';

    var sessionNameOwnerSignal = session.connect('org.freedesktop.DBus',
                                                 '/org/freedesktop/DBus',
                                                 'org.freedesktop.DBus',
                                                 'NameOwnerChanged');

    // html += 'typeof sessionNameOwnerSignal: ' + typeof sessionNameOwnerSignal;
    // html += '<br>';
    // html += 'sessionNameOwnerSignal: ' + showAllProperties(sessionNameOwnerSignal);
    // html += '<br>';

    var systemNameOwnerSignal = system.connect('org.freedesktop.DBus',
                                               '/org/freedesktop/DBus',
                                               'org.freedesktop.DBus',
                                               'NameOwnerChanged');

    var nmApPropertiesSignal = system.connect('org.freedesktop.NetworkManager',
                                              nmGetActiveAccessPointPath(nmGetWlan0Path()),
                                              'org.freedesktop.NetworkManager.AccessPoint',
                                              'PropertiesChanged');

    systemNameOwnerSignal.notify.connect(updateInfoBox);
    nmApPropertiesSignal.notify.connect(updateWlan);

    // sessionNameOwnerSignal.notify.connect(function(obj) {
    //   systemNameOwnerSignal.notify.disconnect(updateInfoBox);
    // });


    // dbus-send --session --print-reply --type=method_call
    //   --dest=org.gnome.Identity /org/gnome/Identity
    //   org.freedesktop.DBus.ObjectManager.GetManagedObjects

    // var wlan0Path             = nmGetWlan0Path();
    // var activeAccessPointPath = nmGetActiveAccessPointPath(wlan0Path);
    // var strength              = nmGetStrength(activeAccessPointPath);

    // dbus.systemConnectPropertyChanged('org.freedesktop.NetworkManager',
    //                                   nmGetActiveAccessPointPath(nmGetWlan0Path()),
    //                                   'org.freedesktop.NetworkManager.AccessPoint',
    //                                   'PropertiesChanged');

    // dbus.systemConnectPropertyChanged('org.freedesktop.DBus',
    //                                   '/org/freedesktop/DBus',
    //                                   'org.freedesktop.DBus',
    //                                   'NameOwnerChanged');

    // dbus['propertiesChanged(QVariantMap)'].connect(updateWlan);
    // dbus['propertiesChanged(QVariant)'].connect(updateInfoBox);

    // html += 'Strength: ' + nmGetActiveAccessPointStrength();

  } catch (e) {
    html += e;
  }

  document.getElementById('main').innerHTML = html;
}

function dbusPropertiesGetAll(service, path, arg)
{
  var iface   = 'org.freedesktop.DBus.Properties';
  var method  = 'GetAll';
  return dbus.systemCall(service, path, iface, method, arg);
}

function nmGetDevices()
{
  var service = 'org.freedesktop.NetworkManager';
  var path    = '/org/freedesktop/NetworkManager';
  var iface   = 'org.freedesktop.NetworkManager';
  var method  = 'GetDevices';
  return dbus.systemCall(service, path, iface, method);
}

function nmGetDeviceProperties(devicePath)
{
  var service = 'org.freedesktop.NetworkManager';
  var path    = devicePath;
  var iface   = 'org.freedesktop.DBus.Properties';
  var method  = 'GetAll';
  var arg1    = 'org.freedesktop.NetworkManager.Device';
  return dbus.systemCall(service, path, iface, method, arg1);
}

function nmGetDeviceInterface(devicePath)
{
  var service = 'org.freedesktop.NetworkManager';
  var path    = devicePath;
  var iface   = 'org.freedesktop.DBus.Properties';
  var method  = 'Get';
  var arg1    = 'org.freedesktop.NetworkManager.Device';
  var arg2    = 'Interface';
  return dbus.systemCall(service, path, iface, method, arg1, arg2);
}

function nmGetWlan0Path()
{
  // dbus-send --system --print-reply --type=method_call
  // --dest=org.freedesktop.NetworkManager /org/freedesktop/NetworkManager
  // org.freedesktop.NetworkManager.GetDeviceByIpIface string:wlan0
  var path   = '/org/freedesktop/NetworkManager';
  var iface  = 'org.freedesktop.NetworkManager';
  var method = 'GetDeviceByIpIface';
  var arg    = 'wlan0';
  return dbus.system.call(nmService, path, iface, method, arg);
}

function nmGetActiveAccessPointPath(wlanPath)
{
  // dbus-send --system --print-reply --type=method_call
  // --dest=org.freedesktop.NetworkManager /org/freedesktop/NetworkManager/Devices/2
  // org.freedesktop.DBus.Properties.Get
  // string:org.freedesktop.NetworkManager.Device.Wireless string:ActiveAccessPoint
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.NetworkManager.Device.Wireless';
  var arg2   = 'ActiveAccessPoint';
  return dbus.system.call(nmService, wlanPath, iface, method, arg1, arg2);
}

function nmGetStrength(activeAccessPointPath)
{
  // dbus-send --system --print-reply --type=method_call
  // --dest=org.freedesktop.NetworkManager
  // /org/freedesktop/NetworkManager/AccessPoint/236
  // org.freedesktop.DBus.Properties.Get
  // string:org.freedesktop.NetworkManager.AccessPoint string:Strength
  var iface  = 'org.freedesktop.DBus.Properties';
  var method = 'Get';
  var arg1   = 'org.freedesktop.NetworkManager.AccessPoint';
  var arg2   = 'Strength';
  return dbus.system.call(nmService, activeAccessPointPath, iface, method, arg1, arg2).charCodeAt(0);
}

function nmGetActiveAccessPointStrength()
{
  return nmGetStrength(nmGetActiveAccessPointPath(nmGetWlan0Path()));
}

//   [!45612 $] dbus-send --system --print-reply --type=method_call
//     --dest= 
//     method return sender=:1.2713 -> dest=:1.2892 reply_serial=2
//        array [
//              object path "/org/freedesktop/NetworkManager/Devices/0"
//                    object path "/org/freedesktop/NetworkManager/Devices/1"
//                          object path "/org/freedesktop/NetworkManager/Devices/2"
//                                object path "/org/freedesktop/NetworkManager/Devices/3"
//                                   ]
//
//                                   [!45613 $] dbus-send --system --print-reply
//                                   --type=method_call
//                                   --dest=org.freedesktop.NetworkManager
//                                   /org/freedesktop/NetworkManager/Devices/0
//                                   org.freedesktop.DBus.Properties.Get
//                                   string:org.freedesktop.NetworkManager.Device
//                                   string:Interface
//                                   method return sender=:1.2713 -> dest=:1.2893 reply_serial=2
//                                      variant       string "lo"
//
//                                      [!45614 $] dbus-send --system --print-reply --type=method_call --dest=org.freedesktop.NetworkManager /org/freedesktop/NetworkManager/Devices/1 org.freedesktop.DBus.Properties.Get string:org.freedesktop.NetworkManager.Device string:Interface
//                                      method return sender=:1.2713 -> dest=:1.2894 reply_serial=2
//                                         variant       string "eth0"
//
//                                         [!45615 $] dbus-send --system --print-reply --type=method_call --dest=org.freedesktop.NetworkManager /org/freedesktop/NetworkManager/Devices/2 org.freedesktop.DBus.Properties.Get string:org.freedesktop.NetworkManager.Device string:Interface
//                                         method return sender=:1.2713 -> dest=:1.2895 reply_serial=2
//                                            variant       string "wlan0"
