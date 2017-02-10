<?php

function decadeHeader($year, $width) {
  $module_path = '/' . drupal_get_path('module', 'agile_chronology');
  $format = <<<EOF
     <h2 id="d%1\$dh2"><a href="%3\$s" onmouseover="swap_on_timeline(%1\$d); return true;" onmouseout="swap_off_timeline(%1\$d); return true;"><img src="$module_path/imgs/tl/%1\$d.gif" name="id%1\$d" width="%2\$d" height="15" alt="%1\$d"></a></h2>
EOF;
  $tmp = trim(sprintf($format, $year, $width, returnHref($year)));
  return $tmp;
}

//  decadeHeader DONE
///////////////////////////////////////////////////////////////////////////

function returnLiveImageTag($year) {
  $module_path = drupal_get_path('module', 'agile_chronology');
  $iName = "b_" . $year;
  if ($year % 10 == 0) { // decade
    $src = "/$module_path/imgs/tl/xf.gif";
  }
  else if ($year % 5 == 0) { // fiver
    $src = "/$module_path/imgs/tl/vf.gif";
  }
  else { // all others
    $src = "/$module_path/imgs/tl/if.gif";
  }
  /*
    EXAMPLE
    <img src="timeline_images/1s_off.gif" width=4 height=73 name="b_1867" border=0 alt="" />
   */
  $imageTag = '<img src="' . $src . '" name="' . $iName . '" alt="' . $year . '">' . "";
  return $imageTag;
}

///////////////////////////////////////////////////////////////////////////

function returnDeadImageTag($year) {
  $module_path = drupal_get_path('module', 'agile_chronology');

  if ($year % 10 == 0) {            // decade -- tallest
    $src = "/$module_path/imgs/tl/xfx.gif";
  }
  else if ($year % 5 == 0) {       // fiver -- med.
    $src = "/$module_path/imgs/tl/vfx.gif";
  }
  else {                     // all others -- spud
    $src = "/$module_path/imgs/tl/ifx.gif";
  }
  //$imageTag = '<img src="' . $src . '" alt="' . $year . '">' . "";  // don't need name and border if no anchor.
  $imageTag = '<a href="#" onclick="return false;" onmouseover="showdate(' . $year . ');" class="dead"><img src="' . $src . '" alt="' . $year . '" border="0"></a>';  // don't need name and border if no anchor.
  return $imageTag;
}

///////////////////////////////////////////////////////////////////////////

function returnAnchorTag($year) {
  //$href = "west.php?year=$year";
  //global $page;

  $href = '/' . $_SESSION["colour"] . '/' . $year;
  $mouseover = ' onmouseover="swap_on_timeline(' . $year . ');"'; ## swap_on_timeline(1867)
  $mouseout = ' onmouseout="swap_off_timeline(' . $year . ');"'; ## onmouseout="swap_off_timeline(1867);">
  $anchorTag = '<a href="' . $href . '" ' . $mouseover . $mouseout . ">";
  return $anchorTag;
}

///////////////////////////////////////////////////////////////////////////

function timelineBar($year) {
  $populatedYears = returnPopulatedYears();
  if (!in_array($year, $populatedYears)) {
    return returnDeadImageTag($year);
  }
  return returnAnchorTag($year) . returnImageTag($year) . '</a>';
}

///////////////////////////////////////////////////////////////////////////


function returnTimeLine_components() {
  $populatedYears = returnPopulatedYears();
  $tmp = array();

  $tmp[] = "\n<div id=\"tl-bars\">";

  $tmp[] = "\n<!-- begin timeline bars -->";

  // in case there was a problem loading info from db, show dead line.
  if (!is_array($populatedYears)) {
    $populatedYears = array();
  }

  for ($i = 1865; $i <= 2020; $i++) { // TODO move 2020 to configuration
    // if there is a record for that year
    if (in_array($i, $populatedYears)) {
      // create the anchor tag and image tag
      $tmp[] = returnAnchorTag($i) . returnLiveImageTag($i) . '</a>';
    }
    else {
      // otherwise, just the image
      $tmp[] = returnDeadImageTag($i);
    }
  }

  $tmp[] = "</div>\n<!-- done timeline bars -->";
  Return implode("", $tmp);
}

function returnHref($year) { // build the correct href to be used in the various anchor tags
  // !! dummy value -- fix later
  //global $page;
  return $_SESSION["colour"] . '/' . $year;
}

function agile_chronology_get_content_by_year($selected_year) {
  $content_table = 'live_can';
  $query = db_select($content_table, 'c');
  $content = $query
      ->fields('c', array('recordBodyEN'))
      ->condition('recordDate', $selected_year)
      ->execute()
      ->fetch();
  return $content->recordBodyEN;
}

function buid_year_selector() {
  $all_years = array();
  $current = date('Y');
  for ($year = 1866; $year <= date('Y'); $year++) {
    $all_years[$year] = $year;
  }
  return $all_years;
}

function agile_chronology_upsert($recordDate, $recordBody) {
  $content_table = 'live_can';
  $query = db_select($content_table, 'c');
  $content = $query
      ->fields('c', array('recordBodyFR'))
      ->condition('recordDate', $recordDate)
      ->execute()
      ->fetch();
  $french = $content ? $content->recordBodyFR : '';

  db_merge('live_can')
      ->key(array('recordDate' => $recordDate))
      ->fields(array(
        'recordDate' => $recordDate,
        'recordBodyEN' => $recordBody,
        'recordBodyFR' => $french,
        'status' => 'PUBLISHED',
        'recordEditedOn' => date("Y-m-d H:i:s"),
        'recordApprovedOn' => date("Y-m-d H:i:s")
      ))
      ->execute();
}
