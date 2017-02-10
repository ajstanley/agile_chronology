<?php

/* * ************************************
 *   T E R M S  F U N C T I O N S 
 * *********************************** */

/* output should look like
  <table>
  <tr>
  <td width="20" align="center"><img src="imgs/bullet.gif" alt="Chronology link" width="15" height="15" border="0" /></td>
  <td><a href="#" title="A Short History of African-American Drama: 1890-1991">African-American Drama</a></td>
  </tr>
  <tr>
  <td width="20" align="center"><img src="imgs/bullet.gif" alt="Chronology link" width="15" height="15" border="0" /></td>
  <td><a href="#" title="Theatre Genres: Naturalism">Naturalism</a></td>
  </tr>
  </table>

 */

/**
 * returns a list containing the terms that have suplementary 
 * info and are indexed to a particular record 
 *
 * @return (array) list of standard objects (MySql query results)
 *
 * @param $recordID		(int)		Record ID 
 * @param $lang			(string)	Language ('en'|'fr') 
 */
function returnIndexedSups($recordID, $lang = 'en') {
  // returns an array containing the terms that have suplementary info and are indexed to a particular record 
  /// remember show only terms that have supplemental info
  global $con;
  if (empty($recordID)) {
    return false;
  }
  $termsTable = TERMS_TABLE;
  $indexTable = INDEX_TABLE;
  $supMapTable = SUPLEMENTAL_MAP_TABLE;
  $supInfoTable = SUPLEMENTAL_TABLE;

  switch (strtolower($lang)) {
    case 'fr':
      $end = 'FR';
      break;
    case 'en':
    default:
      $end = 'EN';
      break;
  }
  // title
  $title = 'title' . $end;
  // link title
  $linkTitle = 'linktitle' . $end;
  $query = db_select($indexTable, 'it');
  $query->join($supMapTable, 'sm', 'it.mapTermID = sm.term_id');
  $query->join($supInfoTable, 'si', 'si.sup_id = sm.sup_id');
  $query->addField('it', 'mapTermID', 'termID');
  $query->addField('it', 'mapRecordID', 'recordID');
  $query->addField('sm', 'term_id', 'term_id');
  $query->addField('si', $title, 'title');
  $query->addField('si', 'sup_id', 'supID');
  $query->addField('si', $linkTitle, 'linkTitle');
  $supResults = $query
      ->condition('it.mapRecordID', $recordID)
      ->execute();
  $num_returned = $supResults->rowCount();
  $rows = $supResults->fetchAll();
  return $num_returned ? $rows : FALSE;
}

/**
 *  Remove all unused anchor tags (terms that don't have Sup info)
 */
function removeTermUrls($body_text) {
  return preg_replace('/<a href="term:\/\/\d+\/">(.*)<\/a>/siU', '\1', $body_text);
}

/**
 * return a relative link to a suplemental record 
 *
 * @return	(string)	URL of sup record
 *
 * @param	$supID		(int)		supplemental ID
 *
 */
function returnSupLink($supID) {
  global $path;
  return $path['root'] . 'sup.php?id=' . $supID;
}

/**
 * returns a list containing the terms that have suplementary 
 * info and are present in a string 
 *
 * @return	(array)		list of term IDs 
 *
 * @param $body			(string)	body copy containing term: URLs
 *
 */
function returnLinkedTerms($body) {
  $matches = array();
  $pattern = '/<a href="term:\/\/(\d+)\/">/siU';

  preg_match_all($pattern, $body, $matches);

  if (isset($matches[1])) {
    $terms = $matches[1];

    // remove duplicate entries
    $terms = array_unique($terms);
    traceVar('terms found in body', $terms);
    return $terms;
  }
  else {
    trace('no terms found in body');
    return false;
  }
}

/**
 * returns a list containing the suplementary records that match
 * a given list of terms. 
 *
 * @return	(array)		list of sup objects 
 *
 * @param	$termList	(array)		list of term ids
 * @param	$lang		(string)	Language ('en'|'fr') 
 *
 */
function returnExistingSups($linkedTermIDs, $lang = 'en') {
  global $con;
  if (empty($linkedTermIDs)) {
    return false;
  }

  $supInfoTable = SUPLEMENTAL_TABLE;
  $supMapTable = SUPLEMENTAL_MAP_TABLE;

  switch (strtolower($lang)) {
    case 'fr':
      $end = 'FR';
      break;
    case 'en':
    default:
      $end = 'EN';
      break;
  }
  // title
  $title = 'title' . $end;
  // link title
  $linkTitle = 'linktitle' . $end;

  $id_list = implode(',', $linkedTermIDs);

  $query = <<<OEQ
    SELECT
      $supInfoTable.$title AS title,
      $supInfoTable.sup_id AS supID,
      $supInfoTable.$linkTitle AS linkTitle,
	  $supMapTable.term_id AS termID
     FROM $supInfoTable, $supMapTable
     WHERE $supInfoTable.sup_id = $supMapTable.sup_id AND
		 $supMapTable.term_id IN ( $id_list )
     GROUP BY supID
OEQ;

  //$supResults = db_query($query, $con, __FILE__, __LINE__);
  $query = db_select($supInfoTable, 'I');
  $query->join($supMapTable, 'M', 'I.sup_id = M.sup_id');
  $query->addField('I', "$title", 'title');
  $query->addField('I', "sup_id", 'supID');
  $query->addField('I', "$linkTitle", 'linkTitle');
  $query->addField('M', "term_id)", 'termID');
  $results = $query
      ->condition('M.term_id', array($id_list), 'IN')
      ->execute()
      ->fetchAll();






  if ($results) {  // if there are any.
    $supArray = array();
    foreach ($results as $result) {
      $supArray[$result->supID] = $result;
    }
    return $supArray;                           // return the list
  }
  else {
    trace('no sup records found');
    return false;         // no sup records exist.
  }
}

/* * ************************************
 *   R E C O R D  F U N C T I O N S 
 * *********************************** */

/**
 * Return a recordID when given a timeline and date.
 *
 * @param $timeline_table	(string) 
 * @param $year				(int)
 * @param $month			(int)
 * @param $language			(string)
 *
 * @return $recordID		(int) 
 */
function IDByDate($timeline_table, $year, $month = 1, $language = 'en') {
  $recordDate = sprintf('%02d-%02d-01', $year, $month);
  $query = db_select($timeline_table, 'lc');
  $result = $query
      ->fields('lc', array('recordID'))
      ->condition('status', 'DELETE', '!=')
      ->condition('recordDate', $recordDate, '>=')
      ->orderBy('recordDate', 'ASC')
      ->execute()
      ->fetchAssoc();
// if a recordID is found return it.
  if (count($result) == 1) {
    return $result['recordID'];
  }
}

/* * ************************************
 *   C O N T E X T  F U N C T I O N S 
 * *********************************** */

/**
 *  Returns a list of context for a given year
 *
 * @param $year			(int)
 * @param $language		(string) 'EN'|'FR'
 *
 * @return an array of the context for the decade that contains year.
 */
function returnContext($year, $language = "en") {

  $year = intval($year);

  if (Validator::isSpecialYear($year)) {
    $lower_limit = $year - 1000;
    $upper_limit = $lower_limit + 99;
  }
  else {
    $lower_limit = floor($year / 10) * 10;
    $upper_limit = $lower_limit + 9;
  }

  if ($language == "en") {
    $contextField = 'contextBodyEN';
  }
  elseif ($language == "fr") {
    $contextField = 'contextBodyFR';
  }

  if (Validator::isSpecialYear($year)) {
    $date_year = "date_year + 1000 as date_year";
  }
  else {
    $date_year = 'date_year';
  }

  $select = "SELECT $contextField as context, $date_year, contextID";


  $query = db_select(CONTEXT_TABLE, 'c');
  $query->addField('c', $contextField, 'context');
  $result = $query
      ->fields('c', array('date_year', 'contextID'))
      ->condition('date_year', $lower_limit, '>=')
      ->condition('date_year', $upper_limit, '<=')
      ->orderBy('date_year', 'ASC')
      ->execute()
      ->fetchAllAssoc('contextID');

  $context = array();
  $contextUnordered = array();
  while ($row = (array) array_shift($result)) {
    $row['context'] = stripslashes($row['context']);
    $year = $row['date_year'];
    $thisID = $row['contextID'];

    // sort before returning
    if (!isset($orderArray[$year])) {
      $orderArray[$year] = returnContextOrder($year);
      traceVar("orderArray[$year]", $orderArray[$year]);
    }

    if (!isset($orderArray[$year][$thisID])) { // if its not ordered put it in a temp. holding array
      $contextUnordered[$year][] = array('context' => $row['context'], 'id' => $thisID);
    }
    else { // if it is ordered, then start our sorted array.
      $context[$year][$orderArray[$year][$thisID]] = array('context' => $row['context'], 'id' => $thisID);
      trace("context[$year][{$orderArray[$year][$thisID]}] = array('context'=>{$row['context']},'id'=>$thisID)");
      ksort($context[$year]);
    }
    traceVar('context-the sorted half', $context);
    traceVar('contextUnordered', $contextUnordered);
  }
  if (isset($contextUnordered)) {
    foreach ($contextUnordered as $year => $yearWithUnordered) {
      foreach ($yearWithUnordered as $unOrdered) {
        $context[$year][] = $unOrdered;
      }
    }
  }
  return $context;
}

/// returns an array on success, false otherwise 
function returnContextOrder($year) {

  $orderGetQuery = 'SELECT contextIDs from ' . CONTEXT_ORDER_TABLE . ' WHERE year=' . $year;
  $query = db_select(CONTEXT_ORDER_TABLE, 'c');
  $orderGetResult = $query
      ->fields('c', array('contextIDS'))
      ->condition('year', '$year')
      ->execute()
      ->fetchAllAssoc('contextIDS');
  // $orderGetResult = db_query($orderGetQuery, '', __FILE__, __LINE__);

  if (count($orderGetResult) < 1) {
    return false;
  }

  $row = array_shift($orderGetResult);
  $orderList = trim($row[0], '|');
  $orderArray = explode('|', $orderList);
  $orderArray = array_flip($orderArray);
  return $orderArray;
}

/**
 *  Returns an (escaped)html string for the context
 *  overlib. (omit any line breaks)
 *
 * @return (string)
 * @param $year			(int)
 * @param $language		(string)
 */
function returnFormattedContext($year, $language = "en") {
  // returns a table containing the associated context
  $contextArray = returnContext($year, $language);
  traceVar('contextArray', $contextArray);
  //print_r($contextArray);

  if (sizeof($contextArray) > 0) { // if there's anything this year
    $contextString = '<table border=0 width=100% cellspacing=0 cellpadding=5 bgcolor=#eeeeee>';
    foreach ($contextArray as $year => $subArray) {
      //traceVar('subArray', $subArray);
      $contexts_this_year = sizeof($subArray);
      $firstContext = array_shift($subArray);
      //traceVar('firstContext', $firstContext);
      $contextString .= "<tr><td valign=top class=headline>$year</td><td class=headline align=center>" . strtoupper($firstContext['context']) . "</td></tr>";
      if (!empty($subArray)) {
        foreach ($subArray as $otherContext) {
          if (!empty($otherContext['context'])) {
            $context = strtoupper($otherContext['context']);
            $contextString .= "<tr valign=top class=headline><td>&nbsp;</td>";
            $contextString .= "<td class=headline align=center>" . $context . "</td></tr>";
          }
        }
      }
    }
    $contextString .="</table>";
  }
  else {
    $contextString = '';
  }

  if (strip_tags($contextString) == '') {
    $contextString = "sorry no context for this record";
  }
  //print_r($contextString);
  Return addslashes($contextString);
}

/* * ************************************
 *   I M A G E   F U N C T I O N S 
 * *********************************** */

/**
 * Returns information about a record image 
 *
 * @return (array)	an array containing the image SRC, ALT and WIDTH and HEIGHT
 *
 * @param $chronology	(string)
 * @param $year			(int)
 * @param $month		(int)
 */
function returnImageInfo($chronology, $year, $month = '') {
  $month = (int) $month;
  $year = (int) $year;
  $chronology = strtolower($chronology);
  // find the image
  if (!Validator::isValidChronology($chronology)) {
    trace('bad chronology');
    return false;
  }

  if (!Validator::isValidYear($year)) { // is year within bounds?
    trace("could not return image info -- year out of range($year)");
    return false;
  }

  $imageName = returnClosestImage($chronology, $year, $month);
  if (!$imageName) {
    return FALSE;
  }

  // figure out image sizes
  traceVar('imageName', $imageName);
  $sizeInfo = @getimagesize(addServerPath($imageName));
  if (!$sizeInfo) {
    trace("unable to get the size of that image ($imageName)");
    return false;
  }
  // find alt text
  /// check the DB
  $altText = getAltText($imageName);
  $image = array(
    'src' => addWebPath($imageName),
    'alt' => $altText,
    'width' => $sizeInfo[0],
    'height' => $sizeInfo[1]// getimagesize get bad height sometimes!! (some gifs report as 1px tall)
  );
  traceVar('image info', $image);
  Return $image;
}

function returnClosestImage($chronology, $year, $month = '') {
  $month = (int) $month;
  $year = (int) $year;
  $chronology = strtolower($chronology);
  // find the image
  //  check the file system
  //   check for image for year and month (west only)
  //   if not, then check for year
  //   if not, then check for decade
  //   if not, complain? ??
  if (!Validator::isValidChronology($chronology)) {
    trace('bad chronology');
    return false;
  }

  if (!Validator::isValidYear($year)) { // is year within bounds?
    trace('year out of range');
    return false;
  }

  if ($month) {
    switch ($chronology) {
      case 'west':
        $specificName = returnImageName($chronology, $year, $month);
        if (imageExists($specificName)) {
          Return imageExists($specificName);
        }
        break;
    }
  }
  // if still here, then check for the year.
  $yearName = returnImageName($chronology, $year);
  if (imageExists($yearName)) {
    Return imageExists($yearName);
  }

  // if still here, then check for the decade.
  $decade = floor($year / 10);
  $decadeName = returnImageName($chronology, $decade);
  if (imageExists($chronology, $decadeName)) {
    Return imageExists($decadeName);
  }
  // if still here, complain
  Return false;
}

/// returns an image filename (hopefully safe for fs use)
function returnImageName($chronology, $year, $month = '') {
  $month = (int) $month;
  $year = (int) $year;
  $chronology = strtolower($chronology);

  if (!Validator::isValidChronology($chronology)) {
    trace('bad chronology');
    return false;
  }

  if ($year < 1000) {
    // must be a decade number then
    // which also means that we can forget about even checking month.
    if (!Validator::isValidDecade($year)) { // is it reasonable
      trace("decade out of range ($year)");
      return false;
    }
    return sprintf('%s/%d', $chronology, $year);
  }

  if (!Validator::isValidYear($year)) {
    trace('year out of range');
    return false;
  }

  $nameEnd = ''; // default as empty
  if ($chronology == 'west' && Validator::isValidMonth($month)) {
    $nameEnd = sprintf('_%02d', $month); // zero-pad month number
  }
  $imageName = $chronology . '/' . $year . $nameEnd;
  traceVar('imageName', $imageName);
  Return $imageName;
}

function imageExists($imageName) {
  $module_path = drupal_get_path('module', 'agile_chronology');
  $file_path = "$module_path/images_live/$imageName.jpg";
  if (file_exists($file_path)) {
    return $imageName . '.jpg';
  }
  if (file_exists("$module_path/images_live/$imageName.gif")) {
    return $imageName . '.gif';
  }
  else {
    return FALSE;
  }
}

function addServerPath($imageName) {
  $module_path = drupal_get_path('module', 'agile_chronology');
  $file_path = "$module_path/images_live/$imageName";
  return $file_path;
}

function addWebPath($imageName) {
  return addServerPath($imageName);
}

function getAltText($filename) {
  // query
  //$query = "SELECT alt FROM " . REC_IMAGE_ALT . " WHERE filename ='$filename'";
  // $results = db_query($query, '', __FILE__, __LINE__);

  $query = db_select(REC_IMAGE_ALT, 'r');
  $results = $query
      ->fields('r', array('alt'))
      ->condition('filename', $filename)
      ->execute()
      ->fetchAssoc();

  // test results
  if (!$results) {
    //trace('trouble getting info from db');

    return FALSE;
  }
  elseif (count($results) < 1) {
    //trace('no alt text');
    return '';
  }
  else {
    return $results['alt'];
  }
}

////////////////////////////////////////////////////
//
//    K E Y   P L A Y    F U N C T I O N S 
//
////////////////////////////////////////////////////
/// returns an arary of all the keyplay entries for a given date 
function getKeyPlaysArray($year) {
  // array[keyplayID][EN|FR] =  keyplayBody values.
  // year MUST be a proper year(eg. 1998)
  $year = (int) $year; // force year to be an integer
  $keyplay = array(); // start with an empty array		
  // check that year is within bounds

  $query = "SELECT keyplayID, title, author from " . KEY_PLAY_TABLE . " WHERE date_year='$year'";
  $results = db_query($query, '', __FILE__, __LINE__);

  // sort before returning
  $orderArray = returnKeyPlayOrder($year);
  $keyplayUnordered = array();
  $keyplays = array();

  while ($row = mysqli_fetch_array($results)) {
    // trace('for each row returned.');
    $thisID = $row['keyplayID'];
    if (!isset($orderArray[$thisID])) { // if its not ordered put it in a temp. holding array
      $keyplayUnordered[] = array('title' => stripslashes($row['title']), 'author' => stripslashes($row['author']), 'id' => $thisID);
    }
    else { // if it is ordered, then start out sorted array.
      $keyplays[$orderArray[$thisID]] = array('title' => stripslashes($row['title']), 'author' => stripslashes($row['author']), 'id' => $thisID);
    }
  }
  //traceVar('keyplay-the sorted half',$keyplays);
  //traceVar('keyplayUnordered',$keyplayUnordered);
  ksort($keyplays);

  foreach ($keyplayUnordered as $value) {
    $keyplays[] = $value;
  }

  /**/
  //traceVar('keyplay-combined',$keyplays);
  return $keyplays;
}

//  getKeyPlayArray DONE
/////////////////////////////////////////////
/// returns an array on success, false otherwise 
function returnKeyPlayOrder($year) {

  $orderGetQuery = 'SELECT keyplayIDs from ' . KEY_PLAY_ORDER_TABLE . ' WHERE year=' . $year;
  $orderGetResult = db_query($orderGetQuery, '', __FILE__, __LINE__);

  if (mysqli_num_rows($orderGetResult) < 1) {
    return false;
  }

  $row = mysqli_fetch_row($orderGetResult);
  $orderList = trim($row[0], '|');
  //traceVar('orderList', $orderList);
  $orderArray = explode('|', $orderList);
  $orderArray = array_flip($orderArray);
  //traceVar('orderArray', $orderArray);
  return $orderArray;
}

//  returnKeyPlayOrder DONE
/////////////////////////////////////////////
?>