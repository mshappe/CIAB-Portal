<?php

/*.
    require_module 'standard';
    require_module 'json';
.*/

require_once($FUNCTIONDIR."/database.inc");
require_once($BASEDIR."/tools/neon_event_lib.inc");
require_once(__DIR__."/../functions/events.inc");

/* This should only be accessable from registration staff */
$access = in_registration();

if (!isset($_SESSION['IS_ADMIN']) && !$access) {
    goSite();
}

if (!empty($_POST)) {
    $arguments = [
    'refreshData'  => FILTER_SANITIZE_SPECIAL_CHARS,
    'updateBadge'  => FILTER_SANITIZE_SPECIAL_CHARS,
    'register'  => FILTER_SANITIZE_SPECIAL_CHARS,
    ];

    $updateData = filter_input_array(INPUT_POST, $arguments);

    if (isset($updateData['refreshData'])) {
        refresh_data($updateData['refreshData']);
        exit();
    }

    if (isset($updateData['updateBadge'])) {
        $data = json_decode(base64_decode($updateData['updateBadge']));
        print update_badge($data);
        exit();
    }

    if (isset($updateData['register'])) {
        $data = json_decode(base64_decode($updateData['register']));
        add_registration($data);
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
