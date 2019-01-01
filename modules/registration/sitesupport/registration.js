/*
 * Base functions for the registration module
 */

/* jshint browser: true */
/* jshint -W097 */
/* global confirmbox, showSidebar, basicBackendRequest, showSidebar, myself */
/* exported sidebarMainDiv, refreshBadgeData, printBadge, showUpdateBadge,
            updateBadge, workstationChange, registerEvent, reg02Req, reg03Req,
            reg04Previous, reg05Previous */

'use strict';

var sidebarMainDiv = 'main_content';

function basicRegistrationRequest(parameter, finish) {
  basicBackendRequest('POST', 'registration', parameter, finish);
}

function refreshBadgeData(badge) {
  basicRegistrationRequest('refreshData=' + badge, function() {
    location.reload();
  });
}

var _badgeData = null;

function printBadge(data) {
  var input = JSON.parse(atob(data));
  _badgeData = data;
  var msg;
  if (input['Print Count'] > 0) {
    msg =  'Reprint badge for \'';
  } else {
    msg =  'Print badge for \'';
  }
  confirmbox('Confirm Print Badge',
    msg + input['Badge Name'] + '\' ?').then(function() {
    window.location = 'index.php?Function=registration&printBadge=' +
                      _badgeData;
  });
}

function showUpdateBadge(data) {
  var input = JSON.parse(atob(data));
  _badgeData = data;
  showSidebar('update_badge_div');
  document.getElementById('badge_name').value = input['Badge Name'];
}

function updateBadge() {
  var data = JSON.parse(atob(_badgeData));
  data['Badge Name'] = document.getElementById('badge_name').value;
  var param = btoa(JSON.stringify(data));
  basicRegistrationRequest('updateBadge=' + param, function() {
    location.reload();
  });
}

function registerEvent(json) {
  var data = JSON.parse(atob(json));
  document.getElementById('reg01_con').innerHTML = data.event.EventName;
  document.getElementById('reg01_img').src = data.banner;
  document.getElementById('reg01_dates').value = data.event.DateFrom + '-' +
    data.event.DateTo;
  var badgeData = '<h3 class="event-color-primary">Tickets</h3>';
  var today = new Date();
  var row = 0;
  badgeData += '<ul class="w3-ul w3-border">';
  data.badges.forEach(function(element) {
    var t = element.AvailableFrom.split(/[-]/);
    var ticketStart = new Date(t[0], t[1] - 1, t[2]);
    t = element.AvailableTo.split(/[-]/);
    var ticketEnd = new Date(t[0], t[1] - 1, t[2] + 1);
    if (ticketStart <= today && ticketEnd >= today) {
      if (row % 2) {
        badgeData += '<li>';
      } else {
        badgeData += '<li class="w3-light-gray">';
      }
      badgeData += ' $' + element.Cost + ' - ' + element.Name + '</li>';
    }
    row += 1;
  });
  badgeData += '<ul>';
  document.getElementById('reg01_tickets').innerHTML = badgeData;
  document.getElementById('reg01_continue').onclick =
    function() { registrationStep2(json); };
  document.getElementById('reg01').style.display = 'block';
}

function registrationStep2(json) {
  document.getElementById('reg01').style.display = 'none';
  var data = JSON.parse(atob(json));
  document.getElementById('reg02_con').innerHTML = data.event.EventName;
  document.getElementById('reg02_img').src = data.banner;
  document.getElementById('reg02_number').value = 1;
  document.getElementById('reg02_self').checked = true;
  document.getElementById('reg02_waiver').innerHTML = '<embed src="' +
    '/resources/data/event/' + data.event.EventID + '_waiver.html" ' +
    'width="100%">';
  document.getElementById('reg02_agree').checked = false;
  document.getElementById('reg02_continue').disabled = true;

  document.getElementById('reg02_continue').onclick =
    function() { registrationStep3(json); };
  document.getElementById('reg02').style.display = 'block';
}

function reg02Req() {
  if (document.getElementById('reg02_number').value > 0 &&
    document.getElementById('reg02_agree').checked) {
    document.getElementById('reg02_continue').disabled = false;
  } else {
    document.getElementById('reg02_continue').disabled = true;
  }
}

function addPersonPanel(data, section, index) {
  var prefix = 'reg03_attend_' + index;
  var html = '<div class="w3-padding">\n' +
               '<button onclick="expandSection(\'' + prefix +
               '\')" class="UI-rest w3-center event-color-primary ' +
               'UI-button w3-block w3-left-align">\n <span ' +
               'id="' + prefix + 'button_' + index + '">' +
               '<span id="' + prefix + '_' + index + '_check">*</span> ' +
               ' Attendee ' + index + '</span> ';
  if (index == 1) {
    html += '<i id="' + prefix + '_arrow" ' +
            'class="fa fa-caret-up"></i> \n </button> \n';
    html += '<div id="' + prefix + '" class="w3-container ' +
             'w3-margin w3-hide w3-responsive w3-show">';
  } else {
    html += '<i id="' + prefix + '_arrow" ' +
            'class="fa fa-caret-down"></i> \n </button> \n';
    html += '<div id="' + prefix + '" class="w3-container ' +
             'w3-margin w3-hide w3-responsive">';
  }

  html += '<label for="' + prefix + '_fname">First Name: *</label>' +
          '<input type="text" id="' + prefix + '_fname' + '"' +
          ' class="w3-input w3-border" onchange="reg03Req();"';
  if (document.getElementById('reg02_self').checked) {
    html += ' value="' + myself.fname + '"';
  } else {
    html += ' value=""';
  }
  html += '/>';
  html += '<label for="' + prefix + '_lname">Last Name: *</label>' +
          '<input type="text" id="' + prefix + '_lname' +
          '" class="w3-input w3-border" onchange="reg03Req();"';
  if (document.getElementById('reg02_self').checked) {
    html += ' value="' + myself.lname + '"';
  } else {
    html += ' value=""';
  }
  html += '/>';
  html += '<label for="' + prefix + '_email">Email: *</label>' +
          '<input type="text" id="' + prefix + '_email' +
          '" class="w3-input w3-border"';
  if (document.getElementById('reg02_self').checked) {
    html += ' value="' + myself.email + '"';
  } else {
    html += ' value=""';
  }
  html += '/>';
  html += '<label for="' + prefix + '_badgename">Badge Name: *</label>' +
          '<input type="text" id="' + prefix + '_badgename' +
          '" value="" class="w3-input w3-border" onchange="reg03Req();"/>';
  html += '<br><span>I am interested in volunteering at CONvergence!</span>';
  html += '<br><input id="' + prefix + '_vol" class="w3-check"' +
          ' type="checkbox"> <label>Please send me information on' +
          ' Volunteering</label><br><br>';
  html += '<label for="' + prefix + '_emergancy">In Case Of Emergency ' +
          '(Name and Phone) :</label>' +
          '<input type="text" id="' + prefix + '_emergancy' +
          '" value="" class="w3-input w3-border"/><br>';

  html += '<span>Event Admission: *</span>';

  var today = new Date();
  var row = 1;
  var badgeData = '<div class="w3-border w3-padding">';
  var first = null;
  data.badges.forEach(function(element, index) {
    var t = element.AvailableFrom.split(/[-]/);
    var ticketStart = new Date(t[0], t[1] - 1, t[2]);
    t = element.AvailableTo.split(/[-]/);
    var ticketEnd = new Date(t[0], t[1] - 1, t[2] + 1);
    if (ticketStart <= today && ticketEnd >= today) {
      if (row % 2) {
        badgeData += '<div>';
      } else {
        badgeData += '<div class="w3-light-gray">';
      }
      badgeData += '<input type="radio" id="' + prefix + '_'  + index + '" ' +
                   'name="' + prefix + '_badge' + '" value="' +
                   index + '"';
      badgeData += ' onclick="document.getElementById(\'' + prefix +
                   '_badgedata\').value = ' + index +
                   ';"';
      if (first === null) {
        badgeData += ' checked ';
        first = index;
      }
      badgeData += '>\n';
      badgeData += '<label for "' + prefix + '_' + index +
                   '"> $' + element.Cost + ' - ' + element.Name + '</label>';
      badgeData += '</div>';
    }
    row += 1;
  });
  badgeData += '<input type=hidden id="' + prefix + '_badgedata" value = ' +
                first + '>';
  badgeData += '</div>';
  html += badgeData;

  html += '</div></div>\n';
  section.innerHTML += html;
}

function reg03Req() {
  var count = document.getElementById('reg02_number').value;
  var disable = false;
  for (var i = 0; i < count; i++) {
    var prefix = 'reg03_attend_' + (i + 1);
    var check = prefix + '_' + (i + 1) + '_check';

    document.getElementById(check).innerHTML = '*';

    if (document.getElementById(prefix + '_fname').value === '') {
      disable = true;
      break;
    }
    if (document.getElementById(prefix + '_lname').value === '') {
      disable = true;
      break;
    }
    if (document.getElementById(prefix + '_email').value === '') {
      disable = true;
      break;
    }
    if (document.getElementById(prefix + '_badgename').value === '') {
      disable = true;
      break;
    }
    document.getElementById(check).innerHTML = '✓';
  }
  document.getElementById('reg03_continue').disabled = disable;
}

function registrationStep3(json) {
  var count = document.getElementById('reg02_number').value;
  document.getElementById('reg02').style.display = 'none';
  var data = JSON.parse(atob(json));
  document.getElementById('reg03_con').innerHTML = data.event.EventName;
  document.getElementById('reg03_img').src = data.banner;

  var section = document.getElementById('reg03_attend');
  section.innerHTML = '';
  for (var i = 0; i < count; i++) {
    addPersonPanel(data, section, i + 1);
  }

  document.getElementById('reg03_continue').onclick =
    function() { registrationStep4(json); };

  document.getElementById('reg03').style.display = 'block';
}

function addSummery(data, section, index) {
  var prefix = 'reg03_attend_' + index;

  var name = document.getElementById(prefix + '_fname').value + ' ' +
             document.getElementById(prefix + '_lname').value;

  var html = '<div class="w3-row">\n' +
             '<div class="w3-container w3-quarter w3-border">' +
             'Attendee ' + index + ':' + name;
  html += '</div>';

  var badgeIndex = document.getElementById(prefix + '_badgedata').value;

  html += '<div class="w3-container w3-half w3-border">' +
          'Ticket: ' + data.badges[badgeIndex].Name;
  html += '</div>';

  html += '<div class="w3-container w3-quarter w3-border">' +
          '$' + data.badges[badgeIndex].Cost;
  html += '</div>';

  html += '</div>';

  html += '<div class="w3-row">\n' +
          '<div class="w3-container w3-quarter w3-border">';
  html += '</div>';

  html += '<div class="w3-container w3-half w3-border w3-small">';
  html += 'Badge Name: ';
  html += document.getElementById(prefix + '_badgename').value;
  html += '<br>';
  html += 'Volunteering:';
  html += document.getElementById(prefix + '_vol').checked;
  html += '<br>';
  html += 'In Case Of Emergency (Name and Phone):';
  html += document.getElementById(prefix + '_emergancy').value;
  html += '</div>';

  html += '<div class="w3-container w3-quarter w3-border">';
  html += '</div>';

  html += '</div>';

  section.innerHTML += html;

  return parseFloat(data.badges[badgeIndex].Cost);
}

function registrationStep4(json) {
  document.getElementById('reg03').style.display = 'none';
  var data = JSON.parse(atob(json));
  document.getElementById('reg04_con').innerHTML = data.event.EventName;
  document.getElementById('reg04_con2').innerHTML = data.event.EventName;
  document.getElementById('reg04_img').src = data.banner;
  document.getElementById('reg04_dates').value = data.event.DateFrom + '-' +
    data.event.DateTo;

  var section = document.getElementById('reg04_summery');
  section.innerHTML = '';
  var count = document.getElementById('reg02_number').value;
  var total = 0;
  for (var i = 0; i < count; i++) {
    total += addSummery(data, section, i + 1);
  }

  var html = '<div class="w3-row">\n' +
             '<div class="w3-container w3-quarter w3-border">' +
             '<b>total</b></div>';
  html += '<div class="w3-container w3-half w3-border">-</div>';
  html += '<div class="w3-container w3-quarter w3-border"><b>$' + total +
          '</b></div>';
  html += '</div>';
  section.innerHTML += html;

  document.getElementById('reg04_continue').onclick =
    function() { registrationStep5(json); };

  document.getElementById('reg04').style.display = 'block';
}

function reg04Previous() {
  document.getElementById('reg04').style.display = 'none';
  document.getElementById('reg03').style.display = 'block';
}

function registrationStep5(json) {
  document.getElementById('reg04').style.display = 'none';
  var data = JSON.parse(atob(json));
  document.getElementById('reg05_con').innerHTML = data.event.EventName;
  document.getElementById('reg05_img').src = data.banner;

  document.getElementById('reg05_policy').innerHTML = '<embed src="' +
    '/resources/data/event/' + data.event.EventID + '_policy.html" ' +
    'width="100%">';

  document.getElementById('reg05_cc').innerHTML = '<h2>TODO: Take Money</h2>';

  document.getElementById('reg05_continue').disabled = false;

  document.getElementById('reg05_continue').onclick =
    function() { registrationStep6(json); };

  document.getElementById('reg05').style.display = 'block';
}

function registrationStep6(json) {
  var count = document.getElementById('reg02_number').value;
  for (var i = 0; i < count; i++) {
    addRegistration(json, i + 1);
  }
}

function reg05Previous() {
  document.getElementById('reg05').style.display = 'none';
  document.getElementById('reg04').style.display = 'block';
}

function addRegistration(json, index) {
  var input = JSON.parse(atob(json));
  var prefix = 'reg03_attend_' + index;
  var badgeIndex = document.getElementById(prefix + '_badgedata').value;
  var emergency = document.getElementById(prefix + '_emergancy').value;
  var badgeName = document.getElementById(prefix + '_badgename').value;

  var data = {
    'Purchaser': input.account,
    'Event': input.event.EventID,
    'Badge': input.badges[badgeIndex].BadgeTypeID,
    'Emergency': emergency,
    'BadgeName' : badgeName,
  };
  var param = btoa(JSON.stringify(data));
  basicRegistrationRequest('register=' + param, function() {
    location.reload();
  });
}

function workstationChange() {
  var value = document.getElementById('workstation_id').value;
  var d = new Date();
  var exdays = 5;
  d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
  var expires = ';expires=' + d.toUTCString();
  document.cookie = 'CIAB_REGISTRATIONWORKSTATION=' + value + expires +
                    ';path=/';
  var element = document.getElementById('kiosk_mode');
  var disabled = element.classList.contains('UI-disabled');
  var state = (value === '');
  if (state !== disabled) {
    if (state && !disabled)
    {element.classList.add('UI-disabled');}
    if (disabled && !state)
    {element.classList.remove('UI-disabled');}
  }
}
