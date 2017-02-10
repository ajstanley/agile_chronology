<?php

function agile_show_can($year = 1866) {
  module_load_include('inc', 'agile_chronology', 'includes/timeline_shared');
  module_load_include('inc', 'agile_chronology', 'includes/search_query');
  module_load_include('inc', 'agile_chronology', 'includes/db_util');
  module_load_include('inc', 'agile_chronology', 'includes/record_class');
  module_load_include('inc', 'agile_chronology', 'includes/keyplay');
  module_load_include('inc', 'agile_chronology', 'includes/news_utilities');
  module_load_include('inc', 'agile_chronology', 'includes/authorship');
  module_load_include('inc', 'agile_chronology', 'includes/profiler');
  $module_path = drupal_get_path('module', 'agile_chronology');

  //$prof = new Profiler(false); // Output the profile information but no trace

  $timelineTable = CAN_TABLE;
  $recordID = IDByDate($timelineTable, $year);
  $factory = new RecordFactory('en');
  if (!$recordID) {
    // could not find ANY record to display
    // get dummy record instead
    $record = $factory->createStaticRecord($timelineTable, '', '', '', '', PLACEHOLDER_TEXT);
  }
  else {
    // get info from DB
    $record = $factory->createDBPrintRecord($timelineTable, $recordID);
  }
  if (empty($record))
    trigger_error('unable to load record for printing');

  $dateString = date('j M. Y');  // dd Mon. yyyy
// get authorship data
  $authorshipData = getAuthorshipLink($recordID, 'C');
  $authorship = $authorshipData['1']['text_en'];
//
// regular display
//
  if (!$recordID) {
    // could not find ANY record to display
    // get dummy record instead
    $record = $factory->createStaticRecord($timelineTable, '', '', '', '', PLACEHOLDER_TEXT);
  }
  else {
    $record = $factory->createRecordCan($recordID);
  }

  $_SESSION["year"] = $record->year;
  $_SESSION["colour"] = "chrono";

// build next/prev links (these don't always exist you know!)

  if ($record->nextDate) {
    // next
    $nextHref = 'chrono/' . $record->nextDate;
    $nextTop = <<<EOV
    <a href="$nextHref" onmouseover="changeImages('fwd_c', "$module_path/imgs/c_right_ovr.gif"); return true;" onmouseout="changeImages('fwd_c', "$module_path/imgs/c_right.gif"); return true;" title="next entry"><img name="fwd_c" src="$module_path/imgs/c_right.gif" alt="next entry" width="18" height="17" border="0" hspace="5" /></a>
EOV;
    $nextBottom = <<<EOV
    <a href="$nextHref" onmouseover="changeImages('fwd_c2', "$module_path/imgs/c_right_ovr.gif"); return true;" onmouseout="changeImages('fwd_c2', "$module_path/imgs/c_right.gif"); return true;" title="next entry"><img name="fwd_c2" src="$module_path/imgs/c_right.gif" alt="next entry" width="18" height="17" border="0" hspace="5" /></a>
EOV;
  }
  else {
    $nextTop = '&nbsp;';
    $nextBottom = '&nbsp;';
  }

  if ($record->prevDate) {
    $prevHref = 'chrono/' . $record->prevDate;
    $prevTop = <<<EOV
    <a href="$prevHref" onmouseover="changeImages('back_c', "$module_path/imgs/c_left_ovr.gif"); return true;" onmouseout="changeImages('back_c', "$module_path/imgs/c_left.gif"); return true;" title="previous entry"><img name="back_c" src="$module_path/imgs/c_left.gif" alt="previous entry" width="18" height="17" border="0" hspace="5" /></a>
EOV;
    $prevBottom = <<<EOV
    <a href="$prevHref" onmouseover="changeImages('back_c2', "$module_path/imgs/c_left_ovr.gif"); return true;" onmouseout="changeImages('back_c2', "$module_path/imgs/c_left.gif"); return true;" title="previous entry"><img name="back_c2" src="$module_path/imgs/c_left.gif" alt="previous entry" width="18" height="17" border="0" hspace="5" /></a>
EOV;
  }
  else {
    $prevTop = '&nbsp;';
    $prevBottom = '&nbsp;';
  }

// build 'printable' link
  $printableHref = $_SERVER['REQUEST_URI'] . '/print';

// find that image
// start with the exact record, then get more and more general until an image is found.
  $imageInfo = returnImageInfo('can', $record->year);

  if (!$imageInfo) {
    /*
      $imageSrc = PLACEHOLDER_IMAGE_NAME;
      $imageAlt = '';
      $imageDimentions = PLACEHOLDER_IMAGE_SIZE;
     */
    $imageCode = '';
  }
  else {
    $imageSrc = $imageInfo['src'];
    $imageAlt = $imageInfo['alt'];
    $imageDimentions = 'width="' . $imageInfo['width'] . '" height="' . $imageInfo['height'] . '" '; // function was returning false info for some GIFs
    $imageCode = <<<EOS
    <img src="$imageSrc" alt="$imageAlt"  title="$imageAlt" $imageDimentions class="float_right" border="2" hspace="6" vspace="6" align="right" />
EOS;
  }

// Set page (HTML) title
  if (!$record->heading) {
    $titleDate = $record->heading;
  }
  else {
    $titleDate = '';
  }

  include("$module_path/theme/tpl.can_main.php");
}

function agile_show_west($year = '1865', $monthD = 0) {
  module_load_include('inc', 'agile_chronology', 'includes/timeline_shared');
  module_load_include('inc', 'agile_chronology', 'includes/search_query');
  module_load_include('inc', 'agile_chronology', 'includes/db_util');
  module_load_include('inc', 'agile_chronology', 'includes/record_class');
  module_load_include('inc', 'agile_chronology', 'includes/keyplay');
  module_load_include('inc', 'agile_chronology', 'includes/news_utilities');
  module_load_include('inc', 'agile_chronology', 'includes/authorship');
  module_load_include('inc', 'agile_chronology', 'includes/profiler');
  $module_path = drupal_get_path('module', 'agile_chronology');
  $timelineTable = WEST_TABLE;
  $recordID = IDByDate($timelineTable, $year, $monthD);
  $factory = new RecordFactory('en');

//
// do we print ?
//
  $url = explode("/", strtolower($_SERVER['REQUEST_URI']));
  if (!$recordID) {
    // could not find ANY record to display
    // get dummy record instead
    $record = $factory->createStaticRecord($recordID, $timelineTable, 'en', '', '', '', '', 'No text yet available');
  }
  else {
    // get info from DB
    $record = $factory->createDBPrintRecord($timelineTable, $recordID);
  }
  if (empty($record))
    trigger_error('unable to load record for printing');

  $thisPage = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['PHP_SELF'];
  $dateString = date('j M. Y');  // dd Mon. yyyy
// get authorship data
  $authorshipData = getAuthorshipLink($recordID, 'W');
  $authorship = $authorshipData['1']['text_en'];

  if (array_pop($url) == 'print') {
    // include template.
    include('./tpl.west_print.php');
    exit;
  }


//
// regular display
//

  if (!$recordID) {
    // could not find ANY record to display
    // get dummy record instead
    $record = $factory->createStaticRecord($recordID, $timelineTable, 'en', '', '', '', '', 'No text yet available');
  }
  else {
    // query the database.
    $record = $factory->createRecordWest($recordID, 'en');
  }

  if (empty($record))
    trigger_error('unable to load record');

  $_SESSION["year"] = $record->year;
  $_SESSION["colour"] = "chronowest";

  traceVar('record body after format', $record->body);



//
//
// build next/prev links (these don't always exist you know!)
//
  if ($record->nextDate) {
    // next
    list($nextY, $nextM) = explode('-', $record->nextDate);
    $nextHref = sprintf('chronowest/%d/%d', $nextY, $nextM);

    $nextTop = <<<EOV
    <a href="$nextHref" onmouseover="changeImages('fwd_w', "/$module_path/imgs/w_right_ovr.gif"); return true;" onmouseout="changeImages('fwd_w', "/$module_path/imgs/w_right.gif"); return true;" title="next entry"><img name="fwd_w" src="/$module_path/imgs/w_right.gif" alt="next entry" width="18" height="17" border="0" /></a>
EOV;

    $nextBottom = <<<EOV
    <a href="$nextHref" onmouseover="changeImages('fwd_w2', "/$module_path/imgs/w_right_ovr.gif"); return true;" onmouseout="changeImages('fwd_w2', "/$module_path/imgs/w_right.gif"); return true;" title="next entry"><img name="fwd_w2" src="/$module_path/imgs/w_right.gif" alt="next entry" width="18" height="17" border="0" /></a>
EOV;
  }
  else {
    $nextTop = '&nbsp;';
    $nextBottom = '&nbsp;';
  }

  if ($record->prevDate) {
    // prev
    list($prevY, $prevM) = explode('-', $record->prevDate);
    $prevHref = sprintf('chronowest/%d/%d', $prevY, $prevM);
    $prevTop = <<<EOV
    <a href="$prevHref" onmouseover="changeImages('back_w', "/$module_path/imgs/w_left_ovr.gif"); return true;" onmouseout="changeImages('back_w', "/$module_path/imgs/w_left.gif"); return true;" title="previous entry"><img name="back_w" src="/$module_path/imgs/w_left.gif" alt="previous entry" width="18" height="17" border="0" /></a>
EOV;
    $prevBottom = <<<EOV
    <a href="$prevHref" onmouseover="changeImages('back_w2', "/$module_path/imgs/w_left_ovr.gif"); return true;" onmouseout="changeImages('back_w2', "/$module_path/imgs/w_left.gif"); return true;" title="previous entry"><img name="back_w2" src="/$module_path/imgs/w_left.gif" alt="previous entry" width="18" height="17" border="0" /></a>
EOV;
  }
  else {
    $prevTop = '&nbsp;';
    $prevBottom = '&nbsp;';
  }

// build 'printable' link
  $printableHref = $_SERVER['REQUEST_URI'] . '/print';


// find that image
// start with the exact record, then get more and more general until an image is found.
  $imageInfo = returnImageInfo('west', $record->year, $record->monthD);

  if (!$imageInfo) {
    $imageCode = '';
  }
  else {
    $imageSrc = $imageInfo['src'];
    $imageAlt = $imageInfo['alt'];
    //$imageDimentions = sprintf(' width="%d" height="%d"', $imageInfo['width'], $imageInfo['height'] );
    $imageDimentions = 'width="' . $imageInfo['width'] . '" height="' . $imageInfo['height'] . '" '; // function was returning false info for some GIFs
    $imageCode = <<<EOS
    <img src="$imageSrc" alt="$imageAlt"  title="$imageAlt" $imageDimentions class="float_right" border="2" hspace="6" vspace="6" align="right" />
EOS;
  }

  $titleDate = $record->heading;
  include("$module_path/theme/tpl.west_main.php");
}

function show_sup($id = 49) {

  define('CAN', 'CAN');
  define('WEST', 'WEST');
  define('BOTH', 'BOTH');

#
# LAYOUT CONSTANTS
#

  define('FMT_SUP_LINK_LIST_ITEM', <<<EOB

					<li>
						<a href="%1\$s" title="%4\$s: %2\$s" class="%4\$s">
							<img src="imgs/%5\$s" alt="%4\$s" width="15" height="15" border="0" />
							%3\$s
						</a>
					</li>
EOB
  );

  /**
   * table that contains list of links from sup record.
   * 	- 1 collection of rows (formatted by FMT_SUP_LINK_LIST_ITEM above)
   * 	- 2 name of "back" button IMG tag
   * 	- 3 src of "back" button
   *  - 4 src of "back" button over state
   *  - 5 image src for decorative rule
   */
  define('FMT_SUP_LINK_TABLE', <<<EOT

	<!-- begin links table -->
				<p style="text-align: left;">The following entries may also be of interest:</p>
				<!-- begin nested link icon table -->
				<ul id="chrono-links">
					%1\$s
				</ul>
				<!--
				<ul id="chrono-links">
				<li><a href="#" title="">Naturalism</a></li>
				<li><a href="#" title="">Cup-and-Saucer Drama</a></li>
				<li><a href="#" title="">Realism</a></li>
				</ul>

				<ul id="feature-links">
				<li id="keyplay"><a href="#">Key European plays of 1867</a></li>
				<li id="headline"><a href="#">Headlines of the 1860s</a></li>
				<li id="news"><a href="#">Expanded news: 1865-1869</a></li>
				</ul>
				-->
				<!-- end nested icon link table -->
	<!-- end nested links table -->

EOT
  );

  /**
   * table to show when there are no links form a sup record. Used in lieu of FMT_SUP_LINK_TABLE 
   * 	- 1 name of "back" button IMG tag
   * 	- 2 src of "back" button
   *  - 3 src of "back" button over state
   *  - 4 image src for decorative rule
   */
  define('FMT_SUP_LINK_EMPTY_TABLE', <<<EOT
	<!-- begin (empty)links table -->
	<!-- end links table -->

EOT
  );


  module_load_include('inc', 'agile_chronology', 'includes/timeline_shared');
  module_load_include('inc', 'agile_chronology', 'includes/search_query');
  module_load_include('inc', 'agile_chronology', 'includes/db_util');
  module_load_include('inc', 'agile_chronology', 'includes/record_class');
  module_load_include('inc', 'agile_chronology', 'includes/keyplay');
  module_load_include('inc', 'agile_chronology', 'includes/news_utilities');
  module_load_include('inc', 'agile_chronology', 'includes/authorship');
  module_load_include('inc', 'agile_chronology', 'includes/profiler');
  module_load_include('inc', 'agile_chronology', 'includes/sup_class');

  if (isset($_SESSION["year"])) {
    $year = $_SESSION["year"];
  }
  else {
    $year = "1800";
  }

  if (isset($_SESSION["colour"])) {
    if ($_SESSION["colour"] == "chrono") {
      $scheme = CAN;
    }
    else {
      $scheme = WEST;
    }
  }
  else {
    $scheme = BOTH;
  }

  $Sup = new supObj($id, 'en');

  if (isset($_GET['print'])) {
    $Sup->displayPrint();
  }
  else {
    $Sup->format_body();
    $Sup->display($scheme);
  }
}

function agile_show_help() {

  define('CONTENT_PAGE_HELP', 'CONTENT_PAGE_HELP');
  module_load_include('inc', 'agile_chronology', 'includes/content_page_manager_class');
  module_load_include('inc', 'agile_chronology', 'includes/profiler');
  $prof = new Profiler_lee(false);

  $prof->startTimer('include_required_files');
  //include('config.php');

  //include('inc.common.php');
  $prof->stopTimer('include_required_files');

  /**
   * have to call session_start if I want to use the _SESSION variables
   * must happen after the code for the objects have been included or we get the
   * dreaded '__PHP_Incomplete_Class', which, it turns out, is pretty useless really.
   * */
  $prof->startTimer('send_cookie');
  $prof->stopTimer('send_cookie');

  $manager = new ContentPageManager('EN', $prof);
  $manager->display(CONTENT_PAGE_HELP, $prof);

  $prof->printTimers(false);
}
?>

