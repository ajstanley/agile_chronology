
<?php
$module_path = drupal_get_path('module', 'agile_chronology');
?>

<div id="leftnav">
    <img src="<?php echo "/$module_path"; ?>/imgs/decade_images/decade_1920.jpg" id="decade_img" alt="" width="130" height="140" />

    <form method="get" action="/agile/search" onsubmit="return checkSearchField(this);">
        <input type="hidden" name="query[1][type]" value="TXT" />
        <input type="hidden" name="order" value="ASC" />
        <input type="hidden" name="timeline[can]" value="true" />
        <input type="hidden" name="timeline[west]" value="true" />
        <input type="text" name="query[1][string]" size="12" maxlength="99" value="search for..." onfocus="fieldFocus(this);" onblur="fieldBlur(this);" class="txt_field" /><input type="submit" value="Go" />
    </form>
    <a href="/agile/search" title="advanced search">advanced&nbsp;search</a>
    <!-- saved results appear here -->
    <br />
    <?php
    if ((isset($_SESSION['saved']) && (!empty($_SESSION['saved'])))) {
      module_load_include('inc', 'agile_chronology', 'includes/search_query');
      foreach ($_SESSION['saved'] as $key => $savedSearch) {
        $savedSearch = unserialize(serialize($savedSearch));
        $links[] = sprintf('<a href="%s?action=load%d" target="main" class="global" title="Saved search: %s">%d</a>', '/agile/search/', $key + 1, $savedSearch->forString, $key + 1);
      }
      echo '<br /><span class="left">saved search results:</span><br />' . implode('&nbsp;|&nbsp;', $links) . "<br />";
    }
    else {
      echo '&nbsp;';
    }
    ?>
    <br />
    <a href="/agile/help" class="global" title="help resources">help</a>&nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp;<a href="/" class="global" title="link to ModernDrama.ca">home</a>
    <div class="divider"></div>
    <a href="http://www.moderndrama.ca/chronology/about-modern-theatre-in-context/" class="global" title="go to the introduction for Modern Theatre in Context">introduction</a><br />
    <a href="http://www.moderndrama.ca/chronology/modern-theatre-in-context-credits/" class="global" title="go to the credits page">credits</a>
    <!-- tab images below must change based on active theme; only difference is the variable in the filename i.e. west vs cdn -->
    <img src="<?php echo "/$module_path/imgs/tab_west.gif"; ?>" width="150" height="96" alt="" usemap="#tabs_west" />
    <!-- <img src="imgs/tab_cdn.gif" width="150" height="96" alt="" usemap="#tabs_cdn" />-->
    <map name="tabs_west" id="tabs_west">
        <area shape="poly" alt="activate British, American and Irish chronology" coords="150,95, 5,95, 3,92, 2,89, 3,83, 4,77, 7,68, 7,67, 9,66, 10,58, 12,55, 15,55, 133,55, 138,53, 143,48, 147,44, 148,39, 150,29" href="/chronowest/<?php echo $year; ?>" />
        <area shape="poly" alt="activate Canadian chronology" coords="149,0, 148,7, 147,13, 145,17, 141,23, 137,25, 133,26, 131,27, 14,27, 12,29, 10,34, 4,55, 3,59, 3,63, 4,66, 6,66, 8,66, 10,57, 11,55, 14,54, 132,54, 136,53, 140,50, 143,47, 146,43, 147,38, 148,34, 149,28, 150,28" href="/chrono/<?php echo $year; ?>" />
    </map>
    <map name="tabs_cdn" id="tabs_cdn">
        <area shape="poly" alt="activate British, American and Irish chronology" coords="149,95, 5,95, 3,93, 3,90, 2,87, 6,73, 8,67, 149,67" href="/chronowest/<?php echo $year; ?>" />
        <area shape="poly" alt="activate Canadian chronology" coords="149,67, 6,67, 5,67, 3,64, 3,61, 3,59, 4,53, 6,44, 8,39, 10,32, 11,29, 12,28, 14,27, 133,27, 137,25, 141,22, 145,19, 146,15, 147,12, 148,8, 149,6, 149,2, 149,0, 152,0" href="chrono/<?php echo $year; ?>" />
    </map>
</div><!-- end #leftnav -->