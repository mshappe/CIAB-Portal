<div id="dynamic_content" class="main_screen">
<?php require($PAGESDIR . '/base/leftnav.inc'); ?>
<div id="main_content">
<?php
require_once($FUNCTIONDIR . "/divisional.inc");

echo "<pre>\n";
ConComListDisplay();
echo "</pre>\n";

?>
</div><!--close main_content div-->