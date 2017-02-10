<?php

/*
  This file builds the timeline bar(at the top) for the western chronology.

  Really, this file populates one variable ($timeline) which is refered to in tpl.chronology.php

 */
module_load_include('inc', 'agile_chronology', 'includes/profiler');
module_load_include('inc', 'agile_chronology', 'includes/utilities');
$module_path = drupal_get_path('module', 'agile_chronology');
$prof = new Profiler(false); // Output the profile information but no trace

function returnPopulatedYears() {

  // shows only 'approved' records
  $query = db_select(CAN_TABLE, 'c');
  $result = $query
      ->fields('c', array('recordDate'))
      ->condition('status', 'DELETE', '!=')
      ->groupBy('recordDate')
      ->orderBy('recordDate', 'ASC')
      ->execute()
      ->fetchAllAssoc('recordDate');

  return array_keys($result);
}

$_SESSION["colour"] = "chrono";
$title = 'British, American and Irish Theatre: A Critical Timeline';
$timeline_letter = 'w';

$img_corner_graphic = "<img src='/$module_path/imgs/tl/tl_corner_cdn.gif' width='93' height='34' border='0' alt='Canadian timeline is active'>";
$img_brace_l = "<img src='/$module_path/imgs/tl/numbers/left_brace_c.gif' width='15' height='30' border='0' alt=''>";
$img_brace_r = "<img src='/$module_path/imgs/tl/numbers/right_brace_c.gif' width='14' height='30' border='0' alt=''>";

$body_format = 'bgcolor="#eeeeee" link="#000000" alink="#660000" vlink="#660000" style="background-image: url(imgs/tl_bg_cdn.gif)"';


$timeline = returnTimeLine_components();
include("$module_path/theme/tpl.chronology.php");
?>