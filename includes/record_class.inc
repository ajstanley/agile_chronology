<?php

/* * *************************************************************************
 * Records package contains the classes used to control the retrieval of 
 * data and the display of records
 *
 * $Source: /var/lib/cvs/dev/crc/chrono/class.record.php,v $
 * @version $Revision: 1.1 $
 * $Date: 2004/03/19 04:17:52 $
 * $Author: rdeil $
 * ************************************************************************* */


// layout constants

/**
 * Link to sup page from chrono record.
 * 	- 1 anchor href
 * 	- 2 anchor title (link title)
 * 	- 3 text (sup title)
 */
define('FMT_LINK_LIST_ITEM', <<<EOB

	<li><a href="%1\$s" title="%2\$s">%3\$s</a></li>

EOB
);

/// Define what HTML code to display in case no image can be found for this record
define('TXT_PLACE_HOLDER', '<img src="imgs/shim.gif" alt="" width="400" height="1" border="0" />');
/// intro text if there are supplemental records to link to (that haven't been listed in the body)
define('TXT_TERM_LINK', '<p style="text-align: left;">The following entries may also be of interest:</p>');

/**
 * Factory for creating records
 */
class RecordFactory {

  var $record;
  var $language;

  /**
   * @param $language if given, must be EN|FR (string)
   */
  function RecordFactory($language = 'en') {
    $this->language = $language;
  }

  /**
   *  Create and return a record object based on given data.
   *
   * @param $recordID (int)
   * @param $timeline (string)
   * @param $recordDate (int)
   * @param $year Year (int)
   * @param $monthD Month - digital (int)
   * @param $month Month - verbose (string)
   * @param $body The content of the record (string)
   *
   * @return a Record object
   */
  function createStaticRecord($timeline, $recordDate, $year, $monthD = 0, $month = '', $body) {
    $this->record = new Record();
    $this->record->setLanguage($this->language);
    $this->record->setTimelineTable($timeline);

    $this->record->year = $year;

    if ($this->record->timeline_table == CAN_TABLE) {
      unset($this->record->month);
      unset($this->record->monthD);
    }
    elseif ($this->record->isSpecialYear()) {
      unset($this->record->month);
      $this->record->monthD = 0;
    }
    else {
      $this->record->setMonthD($monthD);
    }
    $this->record->setRecordDate($recordDate);

    $this->record->decade = floor($this->record->year / 10) * 10;
    $this->record->body = $body;
    return $this->record;
  }

  /**
   *  Create and return a record object, based on data from DB.
   *
   * @param $recordID (int)
   * @param $timeline (string)
   *
   * @return a Record object
   */
  function createRecordCan($recordID) {
    $this->record = new DisplayRecordCan($recordID, $this->language);
    return $this->record;
  }

  /**
   *  Create and return a record object, based on data from DB.
   *
   * @param $recordID (int)
   * @param $timeline (string)
   *
   * @return a Record object
   */
  function createRecordWest($recordID) {
    trace('createDBDisplayRecordWest');
    $this->record = new DisplayRecordWest($recordID, $this->language);
    return $this->record;
  }

  /**
   *  Create and return a record object formatted for print, based on data from DB.
   *
   * @param $recordID (int)
   * @param $timeline (string)
   *
   * @return a Record object
   */
  function createDBPrintRecord($timelineTable, $recordID) {
    $this->record = new PrintRecord($recordID, $timelineTable, $this->language);
    return $this->record;
  }

}

/**
 * @brief Chronology record abstract class.
 *
 * @abstract
 */
class Record {

  // variable declaration	
  var $body; //!< main content of record.
  var $month; //!< (string) verbose month
  var $monthD; //!< (int) digit month
  var $year; //!< (int) record year
  var $decade;
  var $lang; //!< (string)
  var $timeline_table; //!< (string)
  var $recordDate;
  var $recordID;  //!< (int)

  function isSpecialYear() {
    return Validator::isSpecialYear($this->year);
  }

  function setBody($body) {
    $this->body = $body; // verbose body   
  }

  function setMonthD($monthD) {
    if ($this->timeline_table == CAN_TABLE) {
      // pass
      return;
    }
    $monthD = intval($monthD); // digit month
    if ($monthD == 0 && $this->isSpecialYear()) {
      // pass -- legal for 'special' years (1600, 1700, 1800)
      unset($this->month);
      return;
    }
    else if (!Validator::isValidMonth($monthD)) {
      trigger_error('Illegal value. Expecting: 1-12, received: "' . $monthD . '", defauling to "1"', WARNING);
      $monthD = '1';
    }
    $this->monthD = $monthD;
    $monthName = array(1 => 'January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
    $this->month = $monthName[$this->monthD];
  }

  function setYear($year) {
    $year = intval($year);
    $this->year = $year;
    $this->decade = floor($this->year / 10);
  }

  function setRecordDate($rdate) {
    $this->recordDate = $rdate;
  }

  function setTimelineTable($timeline_table) {
    if (($timeline_table != CAN_TABLE) AND ( $timeline_table != WEST_TABLE)) {
      trigger_error('Illegal value. Expecting: ' . CAN_TABLE . '|' . WEST_TABLE . ', received: "' . $timeline_table . '"', ERROR);
    }
    $this->timeline_table = $timeline_table;
  }

  function setLanguage($language) {
    $language = strtoupper($language);
    if (($language != 'EN') AND ( $language != 'FR')) {
      trigger_error('Illegal value. Expecting: EN|FR, received: "' . $language . '", defauling to "EN"', WARNING);
      $language = 'EN';
    }
    $this->lang = $language;
  }

  function setRecordID($recordID) {
    $this->recordID = intval($recordID);
  }

  /**
   * Formats the body content
   * @return void
   */
  function formatBody() {
    // !! either these should be methods of record or of a util class.
    $this->body = stripslashes(stripslashes($this->body)); // twice MUST BE better!!
    $this->safeCharacters();
  }

  /**
   * clean up evil characters
   * @return void
   */
  function safeCharacters() {
    $evil = array('&nbsp;', 'í', chr(145), 'ó', 'ì', 'î'); // illegal chars (+nbsp)
    $better = array(' ', '&#8217;', '&#8217;', '&#8212;', '&#8220;', '&#8221;'); // friendly chars
    $this->body = str_replace($evil, $better, $this->body); // switch-a-roo!		
  }

}

/**
 * 
 */
class PrintRecord extends Record {

  /**
   * Create a record
   *
   * @param recordID int
   * @param timeline_table string 
   * @param language string
   */
  function PrintRecord($recordID, $timeline_table, $language = 'en') {
    // set attributes
    $this->setRecordID($recordID);
    $this->setTimelineTable($timeline_table);
    $this->setLanguage($language);

    $row = $this->setSpecsFromDB();

    $this->formatBody();
    traceVar('base class initialization', $this);
  }

  /**
   * Formats the body content
   * @return void
   */
  function formatBody() {
    parent::formatBody();
    $this->body = strip_tags($this->body, '<strong>,<em>,<p>'); // shouldn't have anchors here 	
  }

  /**
   * queries the database and sets recordData, year, monthD and body. 
   */
  function setSpecsFromDB() {
    $recordBody = 'recordBody' . $this->lang;
    // get month info
    if ($this->timeline_table == WEST_TABLE) {
      $select = 'SELECT recordID, recordDate, MONTHNAME(recordDate) AS month, MONTH(recordDate) AS monthD, YEAR(recordDate) AS year, ' . $recordBody . ' as body';
    }
    else {
      $select = 'SELECT recordID, recordDate, recordDate AS year, ';
      if (strtolower($this->lang) == 'fr') {
        $select .= 'recordBodyFR AS body';
      }
      else {
        $select .= 'recordBodyEN AS body';
      }
    }
    $recordQuery = $select . ' FROM ' . $this->timeline_table . " WHERE recordID='$this->recordID' AND  status != 'DELETE' ";
    $body_name = $this->lang == 'fr' ? 'recordBodyFR' : 'recordBodyEN';
    $query = db_select($this->timeline_table, 'c');
    $query->addField('c', $body_name, 'body');
    $result = $query
        ->fields('c', array('recordID', 'recordDate'))
        ->condition('status', 'DELETE', '!=')
        ->condition('recordID', $this->recordID)
        ->execute()
        ->fetchAssoc();

    if (count($result)) {
      $this->setRecordDate($result['recordDate']);
      $this->setYear($result['recordDate']);
      if ($this->timeline_table == CAN_TABLE) {
        unset($this->month);
        unset($this->monthD);
      }
      else {
        $date_parts = explode('-', $result['recordDate']);
        $this->setMonthD($date_parts[2]);
      }
      $this->setBody($result['body']);
    }
    else {
      trace('unable to retrieve that record');
      trigger_error('unable to retrieve that record', E_USER_ERROR);
      return false;
    }
  }

}

/**
 * Chronology record used for full display. Abstract class
 *
 */
class DisplayRecord extends PrintRecord {

  var $indexedSupArray = array();

  /**
   * a registry to ensure that sup link 
   * get displayed in only one place (be it
   * in the body copy or in the list at the end) 
   * @var $indexedSupDisplayed (array)
   */
  var $indexedSupDisplayed = array();
  var $termsLinksList;
  var $termsLinksIntro;
  var $context;
  var $keyPlaysList;
  var $newsLink;

  /**
   * Create a record with content formatted for display
   *
   * @param $recordID int 
   * @param $timeline_table string 
   * @param $language string 
   */
  function DisplayRecord($recordID, $timeline_table, $language = 'en') {

    $this->PrintRecord($recordID, $timeline_table, $language);

    $this->_set_context();
    $this->_set_keyplay_list();

    $this->_set_news_link();

    $this->_set_remaining_terms_list();

    $this->_setHeading();
    $this->_set_next_record();
    $this->_set_prev_record();

    traceVar('other goods tracked down', $this);
  }

  function _set_news_link() {
    $this->newsLink = getNewsLink($this->year);
  }

  function _set_keyplay_list() {
    $this->keyPlaysList = new KeyPlayList($this->year);
  }

  function _set_context() {
    $this->context = returnFormattedContext($this->year);
  }

  /**
   *  Formats the body content (must retrieve the indexed info first)
   */
  function formatBody() {
    parent::formatBody();
    // get the list of indexed terms
    $this->indexedSupArray = returnIndexedSups($this->recordID, $this->lang);
    $this->convertAnchors();
    $this->removeLeftoverAnchors();
  }

  /**
   *  Remove all unused anchor tags (terms that don't have Sup info)
   */
  function removeLeftoverAnchors() {
    $this->body = preg_replace('/<a href="term:\/\/\d+\/">(.*)<\/a>/siU', '\1', $this->body);
  }

  /**
   *  Remove all unused anchor tags (terms that don't have Sup info)
   */
  function convertAnchors() {
    //$pattern = array();
    //$replacement = array();
    if (empty($this->indexedSupArray))
      return;
    foreach ($this->indexedSupArray as $key => $supObj) {
      if (strstr($this->body, '<a href="term://' . $supObj->termID)) {
        // build regexp patterns
        $pattern = '/<a href="term:\/\/' . $supObj->termID . '\/">/i';
        $supLink = $this->returnSupLink($supObj->supID);
        $title = $supObj->linkTitle ? 'title="' . $supObj->linkTitle . '"' : '';
        $replacement = '<a href="' . $supLink . '" ' . $title . '>';

        //traceVar('pattern', $pattern);
        //traceVar('replacement', $replacement);

        $this->body = preg_replace($pattern, $replacement, $this->body);

        // if a link has been added, then mark this sup object as 'used'
        // register this sup ID as being displayed in the body 
        $this->registerSup($supObj->supID);
      }
    }
    //traceVar('pattern', $pattern);
  }

  /**
   *  build the html code to display the terms list of links. Sets both $termsLinksList and $termsLinksIntro
   */
  function _set_remaining_terms_list() {
    $module_path = drupal_get_path('module', 'agile_chronology');

    $this->termsLinksIntro = "<img src='$module_path/imgs/shim.gif' alt='' width='400' height='1' border='0' />";  // assume no terms left to show
    if (empty($this->indexedSupArray)) {
      Return false;
    }

    $out = array(); // buffer
    traceVar('sup', $this->indexedSupArray);
    // keep track of what sup pages have already been done.
    // in case two indexed terms refer to the same sup page -- no point having the link repeated
    $out[] = '<!-- begin sup -->';
    foreach ($this->indexedSupArray as $supObj) { // still need to build anchor href
      if ($this->isSupAlreadyDisplayed($supObj->supID)) {
        // don't show this one.
      }
      else {
        $supLink = $this->returnSupLink($supObj->supID);
        $out[] = sprintf(FMT_LINK_LIST_ITEM, $supLink, $supObj->linkTitle, $supObj->title);

        // register this sup ID as having been dispayed
        $this->registerSup($supObj->supID);

        // as soon a one sup link is found, change termsLinksIntro
        $this->termsLinksIntro = TXT_TERM_LINK;
      }
    }
    $out[] = '<!-- end sup -->';
    traceVar('this->indexedSupDisplayed', $this->indexedSupDisplayed);

    $this->termsLinksList = implode('', $out);
  }

  /**
   *  set nextdate to the next available record.
   */
  function _set_next_record() {
    // find the next record (id) that has body info.
    $nextQuery = "SELECT recordDate FROM " . $this->timeline_table . " WHERE recordDate > '$this->recordDate' AND  status != 'DELETE' ORDER BY recordDate ASC LIMIT 1";
    $query = db_select($this->timeline_table, 't');
    $nextRow = $query
        ->fields('t', array('recordDate'))
        ->condition('recordDate', $this->recordDate, '>')
        ->condition('status', 'DELETE', '!=')
        ->orderBy('recordDate', 'ASC')
        ->execute()
        ->fetchAssoc();

    $nextDate = $nextRow['recordDate'];
    $this->nextDate = $nextDate;
  }

  /**
   *  set prevdate to the previous available record.
   */
  function _set_prev_record() {
    // find the previous record (id) that has body info.
    $query = db_select($this->timeline_table, 't');
    $prevRow = $query
        ->fields('t', array('recordDate'))
        ->condition('recordDate', $this->recordDate, '<')
        ->condition('status', 'DELETE', '!=')
        ->orderBy('recordDate', 'ASC')
        ->execute()
        ->fetchAssoc();
    $prevDate = $prevRow['recordDate'];
    $this->prevDate = $prevDate;
  }

  function returnSupLink($supID) {
    return "/agile/sup/$supID";
  }

  function _setHeading() {
    if ($this->isSpecialYear()) {
      $this->heading = 'The ' . $this->year . 's';
    }
    else {
      $this->heading = (isset($this->month)) ? " $this->month $this->year" : " $this->year";
    }
  }

  /**
   *  Register a Sup record as being displayed
   *
   * @param $supID		int Suplemental record ID
   */
  function registerSup($supID) {
    $this->indexedSupDisplayed[$supID] = true;
  }

  /**
   * Identifies whether a Sup record has been displayed already
   * @return boolean
   * @param $supID		int Suplemental record ID 
   */
  function isSupAlreadyDisplayed($supID) {
    if (isset($this->indexedSupDisplayed[$supID])) {
      Return $this->indexedSupDisplayed[$supID];
    }
    else {
      Return false;
    }
  }

  function returnKeyPlaysOverlibCode() {
    Return $this->keyPlaysList->overLibCode;
  }

}

/**
 *  A canadian record with content formatted for display
 *
 */
class DisplayRecordCan extends DisplayRecord {

  function DisplayRecordCan($recordID, $language = 'en') {
    $this->DisplayRecord($recordID, CAN_TABLE, $language = 'en');
  }

}

/**
 *  A west record with content formatted for display
 *
 */
class DisplayRecordWest extends DisplayRecord {

  var $monthNav = false;

  /**
   *  Create a west record with content formatted for display
   * @param $recordID	int
   * @param $language	string
   */
  function DisplayRecordWest($recordID, $language = 'en') {
    DisplayRecord::DisplayRecord($recordID, WEST_TABLE, $language);
    $this->monthNav = $this->returnMonthNav();
  }

  function returnMonthNav() {
    if ($this->isSpecialYear()) {
      Return false;
    }
    global $path;

    $year = $this->year;

    $recordID = $this->recordID;

    $root = $path['root'];

    // return a formatted navigation that points to other records in this same year.
    $monthArray = $this->returnRecThisYearArray();
    // print_r($monthArray);
    $monthName = array(1 => 'January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');

    $monthNav = '<ol>';
    foreach ($monthName as $monthNumber => $monthLabel) {
      if (array_key_exists($monthNumber, $monthArray)) {
        // something to link to
        $monthNav .= '<li>';
        $monthNav .= '<a class = "nav" href = "' . $root . 'west/' . $year . '/' . $monthNumber . '" title = "' . $monthLabel . ' ' . $year . '">';
        //$monthNav .= '<a href = "' . $_SERVER['PHP_SELF'] . '?recordID=' . $monthArray[$monthNumber] . '">';
        //$monthNav .= $monthLabel;
        $monthNav .= substr($monthLabel, 0, 1);
        $monthNav .= '</a>';
      }
      else {
        // no anchor
        if ($monthNumber == $this->monthD) { // maybe it this month?
          trace('you are here');
          $monthNav .= '<li id = "thismonth">'; // bold the record your on
          $monthNav .= substr($monthLabel, 0, 1);
          $monthNav .= '</li>';
        }
        else {
          $monthNav .= '<li>';
          $monthNav .= substr($monthLabel, 0, 1);
        }
      }
      $monthNav .= "</li>";
    }
    $monthNav .= "</ol>";
    Return $monthNav;
  }

  /// returns an array of record ids that are in the same year. Indexed to their month.
  function returnRecThisYearArray() {
    $monthQuery = "SELECT recordID, MONTH(recordDate) as m from " . WEST_TABLE . " WHERE YEAR(recordDate)='$this->year' AND recordID!=$this->recordID AND  status != 'DELETE' ORDER BY recordDate ASC"; // and APPROVED!!
    // $monthResult = db_query($monthQuery, '', __FILE__, __LINE__);
    $query = db_select(WEST_TABLE, 'w');
    $monthResult = $query
        ->fields('w', array('recordID', 'recordDate'))
        ->condition('status', 'DELETE', '!=')
        ->condition('recordDate', "YEAR('recordDate') = $this->year")
        ->condition('recordID', $this->recordID, '!=')
        ->execute()
        ->fetchAll();

    $monthArray = array();
//    while ($row = mysqli_fetch_array($monthResult, MYSQLI_ASSOC)) {
//      $monthArray[$row['m']] = $row['recordID'];
//    }

    return $monthArray;
  }

}

?>