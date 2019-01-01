<?php

/*.
    require_module 'standard';
.*/

namespace registration\panes;

require_once(__DIR__.'/../functions/events.inc');
require_once($BACKEND.'/asset.inc');


function _htmlsafeify($array)
{
    $output = [];
    foreach ($array as $key => $value) {
        $output[$key] = htmlspecialchars($value);
    }
    return $output;

}


function badges()
{
    $badges = get_tickets($_SESSION['accountId']);
    $lines = [];
    if (!empty($badges)) {
        foreach ($badges as $badge) {
            $name = $badge['BadgeName'];
            if ($name == null) {
                $name = "<unselected>";
            }
            if (array_key_exists($badge['EventName'], $lines)) {
                $lines[$badge['EventName']][] = [
                'Id' => $badge['AccountID'],
                'Badge Name' => $name,
                'Badge Type' => $badge['Badge'],
                'Purchased' => $badge['RegistrationDate'],
                'Purchaser' => $badge['RegisteredByID']
                                                ];
            } else {
                $lines[$badge['EventName']] = [[
                'Id' => $badge['AccountID'],
                'Badge Name' => $name,
                'Badge Type' => $badge['Badge'],
                'Purchased' => $badge['RegistrationDate'],
                'Purchaser' => $badge['RegisteredByID']
                                               ]];
            }
        }
        ksort($lines);
?>
        <script src="sitesupport/common.js"></script>
        <script src="modules/registration/sitesupport/registration.js"></script>
        <div class='UI-container event-color-secondary'>

        <div class='UI-center event-color-secondary'>
            <strong style="margin-left: 100px;">Badges Purchased</strong>
            <div class='UI-right'>
                <input id='refresh' type='button' class='UI-orangebutton'
                       onclick='refreshBadgeData(<?php echo $_SESSION['accountId'];?>)' value='refresh'>
            </div>
        </div>

        <table class='UI-stripedtable'>
        <caption>Badges to the events</caption>
<?php
foreach ($lines as $con => $names) {
    $colspan = count($names[0]);
    echo "<table class='UI-stripedtable'>\n";
    echo "<tr><th class='UI-center event-color-primary' colspan='$colspan'>".$con;
    echo " (".count($names)." badges)</th></tr>\n";
    $keys = array_keys($names[0]);
    echo "<tr><th>";
    echo implode(_htmlsafeify($keys), '</th><th>');
    echo "</th></tr>\n";
    foreach ($names as $badge) {
        echo "<tr><td>";
        echo implode(_htmlsafeify($badge), '</td><td> ');
        echo "</td></tr>\n";
    }
    echo "</table>\n";
}
?>
        <tr class='event-color-secondary'><th id="placeholder" colspan=5 class='UI-center'></th></tr>
        </table>
        </div>
<?php
    } else {
    }

}


function upcoming()
{
    $events = get_upcoming();
    if (!empty($events)) {
?>
        <script src="sitesupport/common.js"></script>
        <script src="modules/event/sitesupport/events.js"></script>
        <script src="modules/registration/sitesupport/registration.js"></script>
        <div class='w3-container event-color-secondary'>
            <div class='w3-center event-color-secondary'>
                <span class='w3-large'><b>Upcoming Events</b></span>
            </div>
            <div>
                <table class='w3-table w3-striped w3-bordered w3-white w3-responsive'>
<?php
foreach ($events as $event) {
    echo "<tr>";
    echo "<td><span class='w3-cell-middle w3-large'><b>";
    echo htmlspecialchars($event['EventName'])."</b><span></td>\n";
    echo "<td>".$event['DateFrom']." <b>--</b> ".$event['DateTo'];
    echo "</td>\n";
    $tickets = get_tickets($_SESSION['accountId'], $event['EventID']);
    if (count($tickets) == 0) {
        $badges = get_badge_types();
        if (count($badges) > 0) {
            $param = [
            'account' => $_SESSION['accountId'],
            'event' => $event,
            'badges' => $badges,
            'banner' => \ciab\Asset::load('event_banner_'.$event['EventID'])
                     ];
            $param = json_encode($param);
            $param = base64_encode($param);
            echo "<td><input id='refresh' type='button' class='UI-eventbutton' onclick='registerEvent(\"".$param."\");' value='REGISTER NOW!'></td>\n";
        } else {
            echo "<td><span class='w3-padding w3-gray w3-round-xlarge'>No badges available</span></td>\n";
        }
    } else {
        echo "<td>".count($tickets)." badge";
        if (count($tickets) > 1) {
            echo "s";
        }
        echo " purchased </td>\n<td>";
        $badges = get_badge_types();
        if (count($badges) > 0) {
            $param = [
            'account' => $_SESSION['accountId'],
            'event' => $event,
            'badges' => $badges
                     ];
            $param = json_encode($param);
            $param = base64_encode($param);
            echo "<input id='refresh' type='button' class='UI-eventbutton' onclick='registerEvent(\"".$param."\");' value='Buy more'></td>\n";
        } else {
            echo "<span class='w3-padding w3-gray w3-round-xlarge'>Badges no longer avaliable</span></td>\n";
        }
    }
    echo "</tr>\n";
}
?>
                </table>
            </div>
        </div>
<?php
    } else {
    }

}


$homepage_panes[] = 'registration\panes\upcoming';
$homepage_panes[] = 'registration\panes\badges';

echo "\n<script>\n";
echo "  var myself = {\n";
echo "      'fname' : '".$_SESSION['preferredFirstName']."',\n";
echo "      'lname' : '".$_SESSION['preferredLastName']."',\n";
echo "      'email' : '".$_SESSION['email']."'\n";
echo "      };\n";
echo "</script>\n";

?>
<div id="reg01" class="w3-modal" style="position: fixed;">
 <div class="w3-modal-content w3-card-4 w3-animate-zoom">
  <header class="w3-container w3-blue">
   <span onclick="document.getElementById('reg01').style.display='none'"
   class="UI-button w3-blue w3-xlarge w3-display-topright">&times;</span>
   <h2>Registering for <span id="reg01_con">CONVENTION</span></h2>
  </header>
  <div class="w3-container w3-center">
    <img id='reg01_img' style="max-width:600px;max-height:170px;" class="w3-center">
  </div>
  <div class=" w3-container w3-row w3-padding w3-responsive">
    <div class="w3-third">
     <input type="text" name="daterange" id="reg01_dates" value="                          " readonly />
    </div>
    <div class="UI-rest">
        <div id="reg01_tickets">
        </div>
    </div>
  </div>
  <div class="w3-container w3-light-grey w3-padding w3-center">
   <button class="UI-roundbutton w3-right w3-white w3-border"
   onclick="document.getElementById('reg01').style.display='none'">Close</button>
   <button class="UI-eventbutton w3-center w3-border"
   id="reg01_continue">Continue</button>
  </div>
 </div>
</div>

<div id="reg02" class="w3-modal" style="position: fixed;">
 <div class="w3-modal-content w3-card-4">
  <header class="w3-container w3-blue">
   <span onclick="document.getElementById('reg02').style.display='none'"
   class="UI-secondarybutton w3-xlarge w3-display-topright">&times;</span>
   <h2>Registering for <span id="reg02_con">CONVENTION</span></h2>
  </header>
  <div class="w3-container w3-center">
    <img id='reg02_img' style="max-width:600px;max-height:170px;" class="w3-center">
  </div>
  <div class="w3-container w3-padding">
    <label for='reg02_number'>Number Attending: *</label>
    <input id="reg02_number" type='number' class='w3-input w3-border' value=1
     onchange="reg02Req();" min=1>
    <input id="reg02_self" class="w3-check" type="checkbox" checked>
    <label>Include myself as an attendee</label>
    <hr>
    <label>EVENT WAIVER AGREEMENT</label>
    <div id="reg02_waiver" class="w3-container w3-border w3-padding">
    </div>
    <input id="reg02_agree" class="w3-check" type="checkbox" onclick='reg02Req();'>
    <label>Yes, I Agree: *</label>
  </div>
  <div class="w3-container w3-light-grey w3-padding w3-center">
   <button class="UI-roundbutton w3-right w3-white w3-border"
   onclick="document.getElementById('reg02').style.display='none'">Close</button>
   <button class="UI-eventbutton w3-center w3-border"
   id="reg02_continue" disabled>Continue</button>
  </div>
 </div>
</div>

<script src="sitesupport/dropdownsection.js"></script>

<div id="reg03" class="w3-modal" style="position: fixed;">
 <div class="w3-modal-content w3-card-4">
  <header class="w3-container w3-blue">
   <span onclick="document.getElementById('reg03').style.display='none'"
   class="UI-button w3-blue w3-xlarge w3-display-topright">&times;</span>
   <h2>Registering for <span id="reg03_con">CONVENTION</span></h2>
  </header>
  <div class="w3-container w3-center">
    <img id='reg03_img' style="max-width:600px;max-height:170px;" class="w3-center">
  </div>
  <div class=" w3-container w3-padding" id="reg03_attend">
  </div>
  <div class="w3-container w3-light-grey w3-padding w3-center">
   <button class="UI-roundbutton w3-right w3-white w3-border"
   onclick="document.getElementById('reg03').style.display='none'">Close</button>
   <button class="UI-eventbutton w3-center w3-border"
   id="reg03_continue" disabled>Continue</button>
  </div>
 </div>
</div>

<div id="reg04" class="w3-modal" style="position: fixed;">
 <div class="w3-modal-content w3-card-4">
  <header class="w3-container w3-blue">
   <span onclick="document.getElementById('reg04').style.display='none'"
   class="UI-button w3-blue w3-xlarge w3-display-topright">&times;</span>
   <h2>Registering for <span id="reg04_con">CONVENTION</span></h2>
  </header>
  <div class="w3-container w3-center">
    <img id='reg04_img' style="max-width:600px;max-height:170px;" class="w3-center">
  </div>
  <div class=" w3-container w3-padding" id="reg04_review">
    <div>
        <h2>Event Registration Summary</h2>
        <span>Please review your registration information before proceeding to the payment page.</span>
    </div>
    <br>
    <div>
        <h2>Event: <span id="reg04_con2">CONVENTION</span></h2>
        <input type="text" name="daterange" id="reg04_dates" value="                        " readonly />
    </div>
    <br>
    <div>
        <div class="w3-row">
            <div class="w3-container w3-quarter w3-border">
            Attendee
            </div>
            <div class="w3-container w3-half w3-border">
            Admission
            </div>
            <div class="w3-container w3-quarter w3-border">
            Amount
            </div>
        </div>
        <div id="reg04_summery">
        </div>
    </div>
  </div>
  <div class="w3-container w3-light-grey w3-padding w3-center">
   <button class="UI-eventbutton w3-left w3-border"
   id="reg04_back" onclick="reg04Previous();">Previous</button>
   <button class="UI-roundbutton w3-right w3-white w3-border"
   onclick="document.getElementById('reg04').style.display='none'">Close</button>
   <button class="UI-eventbutton w3-center w3-border"
   id="reg04_continue">Complete Registration</button>
  </div>
 </div>
</div>

<div id="reg05" class="w3-modal" style="position: fixed;">
 <div class="w3-modal-content w3-card-4">
  <header class="w3-container w3-blue">
   <span onclick="document.getElementById('reg05').style.display='none'"
   class="UI-button w3-blue w3-xlarge w3-display-topright">&times;</span>
   <h2>Registering for <span id="reg05_con">CONVENTION</span></h2>
  </header>
  <div class="w3-container w3-center">
    <img id='reg05_img' style="max-width:600px;max-height:170px;" class="w3-center">
  </div>
  <div class=" w3-container w3-padding" id="reg05_review">
    <div>
        <h2>Event Registration Payment</h2>
        <span>Please review your registration information before proceeding to the payment page.</span>
    </div>
  </div>

  <div class='w3-container'>
    <div id='reg05_cc' class='w3-container w3-padding w3-border w3-margin'>
    </div>
    <div id='reg05_policy' class='w3-container w3-padding w3-border'>
    </div>
  </div>

  <div class="w3-container w3-light-grey w3-padding w3-center">
   <button class="UI-eventbutton w3-left w3-border"
   id="reg05_back" onclick="reg05Previous();">Previous</button>
   <button class="UI-roundbutton w3-right w3-white w3-border"
   onclick="document.getElementById('reg05').style.display='none'">Close</button>
   <button class="UI-eventbutton w3-center w3-border"
   id="reg05_continue">Submit Payment</button>
   <p class='w3-red'> NOTE: Please only click the 'Submit Payment' button once. Your payment may take time to process.</p>
  </div>
 </div>
</div>
