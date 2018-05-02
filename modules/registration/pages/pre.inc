<?php

/*.
    require_module 'standard';
    require_module 'json';
.*/

require_once($FUNCTIONDIR."/database.inc");
require_once(__DIR__."/../functions/events.inc");

/* This should only be accessable from registration staff */
$access = in_registration();

if (!isset($_SESSION['IS_ADMIN']) && !$access) {
    goSite();
}

if (!empty($_POST)) {
    $arguments = [
    'updateBadge'  => FILTER_SANITIZE_SPECIAL_CHARS,
    ];

    $updateData = filter_input_array(INPUT_POST, $arguments);

    if (isset($updateData['updateBadge'])) {
        $data = json_decode(base64_decode($updateData['updateBadge']));
        print update_badge($data);
        exit();
    }
}

if (!empty($_REQUEST)) {
    $arguments = [
    'printBadge'  => FILTER_SANITIZE_SPECIAL_CHARS,
    ];

    require_once($FUNCTIONDIR."/volunteer.inc");

    $updateData = filter_input_array(INPUT_GET, $arguments);

    if (isset($updateData['printBadge'])) {
        $data = json_decode(base64_decode($updateData['printBadge']));
        print_badge($data);
        exit();
    }
}
