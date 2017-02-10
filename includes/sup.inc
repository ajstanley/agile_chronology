<?php

/*
  still to do:
  - french link type names
  - figure out what link to self should be / useless if redirect is enabled
 */

/////////////////////////////////////////////////////////
//
//  D E F I N E   T E M P L A T E   C O N S T A N T S 
//

define('CAN', 'CAN');
define('WEST', 'WEST');
define('BOTH', 'BOTH');

#
# LAYOUT CONSTANTS
#

/**
 * Link from sup record.
 * 	- 1 anchor href
 * 	- 2 anchor title (link title)
 * 	- 3 text (sup title)
 *  - 4 link type (eg. FTP, WWW, Supplemental, etc.)
 *  - 5 link icon graphic
 */
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



# read template constants from a config file
$template = parse_ini_file("sup.ini", true);

//
/////////////////////////////////////////////////////////
//ob_start('out');


include ('./class.sup.php');

module_load_include('inc', 'agile_chronology', 'includes/timeline_shared');
module_load_include('inc', 'agile_chronology', 'includes/search_query');
module_load_include('inc', 'agile_chronology', 'includes/db_util');
module_load_include('inc', 'agile_chronology', 'includes/record_class');
module_load_include('inc', 'agile_chronology', 'includes/keyplay');
module_load_include('inc', 'agile_chronology', 'includes/news_utilities');
module_load_include('inc', 'agile_chronology', 'includes/authorship');
module_loamd_include('inc', 'agile_chronology', 'includes/profiler');
module_loamd_include('inc', 'agile_chronology', 'includes/sup_class');


if (isset($_SESSION["year"])) {
  $year = $_SESSION["year"];
}
else {
  $year = "1800";
}

if (isset($_SESSION["colour"])) {
  if ($_SESSION["colour"] == "can") {
    $scheme = CAN;
  }
  else {
    $scheme = WEST;
  }
}
else {
  $scheme = BOTH;
}

$Sup = new supObj($_GET['id'], 'en');

if (isset($_GET['print'])) {
  $Sup->displayPrint();
}
else {
  $Sup->format_body();
  $Sup->display($scheme);
}