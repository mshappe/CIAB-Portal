<?php
require_once $BACKEND.'/asset.inc';
?>

<script>
  function myMenuBar() {
    var x = document.getElementById("main_nav_list");
    if (x.className.indexOf("UI-show") == -1) {
      x.className += " UI-show";
    } else {
      x.className = x.className.replace(" UI-show", "");
    }
  }
</script>
<div class="UI-main-bar-div">
  <div id="main_nav" class="UI-main-bar">
  <img class="UI-main-bar-icon" src='data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///yH5BAEAAAAALAAAAAABAAEAAAIBRAA7'
    alt="Event Icon" style="background-image:url('<?php echo \ciab\Asset::load('org-icon');?>')"
>

<?php

require_once $BASEDIR.'/kiosk/kiosk.inc';
require_once $FUNCTIONDIR.'/menus.inc';

if (!isset($base_styles)) {
    $base_styles = " UI-main-bar-item";
}
if (!isset($resp_styles)) {
    $resp_styles = " UI-main-responsive-bar-item";
}

$kiosk = get_kiosk();

if ($kiosk === null) {
    if (!isset($base_menus)) {
        $base_menus = array();
        $base_menus[] = array('base style' => '',
             'selected style' => 'event-color-primary',
             'function' => 'main',
             'title' => 'Main Page',
             'i class' => 'fas fa-home',
             'text' => 'Main',
             'responsive' => false);

        $base_menus[] = array('base style' => '',
             'selected style' => 'event-color-primary',
             'function' => 'profile',
             'title' => 'Update Profile Information',
             'i class' => 'fas fa-user',
             'text' => 'Profile',
             'responsive' => true);
    }

    if (!isset($admin_menus)) {
        $admin_menus = [
        ];
    }

    if (!isset($report_menus)) {
        $report_menus = [
        ];
    }

    $modules = scandir($MODULESDIR);
    foreach ($modules as $key => $value) {
        if (!in_array($value, array(".", ".."))) {
            if ($kiosk === null && in_array($value, $DISABLEDMODULES)) {
                continue;
            }
            if (is_dir($MODULESDIR.DIRECTORY_SEPARATOR.$value)) {
                if (is_file($MODULESDIR.DIRECTORY_SEPARATOR.$value.DIRECTORY_SEPARATOR.'pages/menubar.inc')) {
                    require_once($MODULESDIR.DIRECTORY_SEPARATOR.$value.DIRECTORY_SEPARATOR.'pages/menubar.inc');
                }
            }
        }
    }
} else {
    $base_menus = array();
    if (!isset($admin_menus)) {
        $admin_menus = array();
    }
    if (!isset($report_menus)) {
        $report_menus = array();
    }

    if (is_dir($MODULESDIR.DIRECTORY_SEPARATOR.$kiosk)) {
        if (is_file($MODULESDIR.DIRECTORY_SEPARATOR.$kiosk.DIRECTORY_SEPARATOR.'pages/menubar.inc')) {
            require_once($MODULESDIR.DIRECTORY_SEPARATOR.$kiosk.DIRECTORY_SEPARATOR.'pages/menubar.inc');
        }
    }
}
$base_menus[] = array(
     'base style' => 'UI-admin-menu',
     'function' => $admin_menus,
     'title' => 'Administrator Pages',
     'i class' => 'fas fa-key',
     'text' => 'Administer',
     'responsive' => true);

$base_menus[] = array(
     'base style' => 'UI-admin-menu',
     'function' => $report_menus,
     'title' => 'Administrative Reports',
     'i class' => 'fas fa-file-csv',
     'text' => 'Reports',
     'responsive' => true);

echo build_menus($base_menus, $base_styles, $resp_styles);

if ($kiosk === null) {
?>
    <a class="UI-main-bar-pancake"
      href="javascript:void(0)"
      onclick="myMenuBar()">
      <em class="fas fa-bars"></em>
    </a>
    <a class="UI-main-logout-bar-item"
      href="index.php?Function=public" title="Log out and Restart">
      <em class="fas fa-sign-out-alt"></em>
      Log Out
    </a>
<?php } ?>


    <!--Responsive menu for tables and moblie-->
    <div id="main_nav_list" class="UI-main-bar-pancake-div">

<?php
echo build_menus($base_menus, $base_styles);
?>
    </div><!--close main_nav_list div-->
  </div><!--close main_nav div-->
</div>
<br />
<br />
