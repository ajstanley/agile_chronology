<?php

/* * *************************************************************************
 * class to control retrieve and format key play data
 * $Source: /var/lib/cvs/dev/crc/chrono/class.keyplay.php,v $
 * $Revision: 1.1 $
 * $Date: 2004/03/19 04:17:52 $
 * $Author: rdeil $
 * $Locker:  $
 * $State: Exp $
 * ************************************************************************* */

define('FMT_KEYPLAY_PRE', '<span class=play_title>');
define('FMT_KEYPLAY_ITEM', '%s, <u>%s</u><br>');
define('FMT_KEYPLAY_POST', '</span>');
define('FMT_KEYPLAY_LINK', '<a href="#" title="Click for key European plays of %1$s" class="tagline" onClick="event.preventDefault(); return overlib(key_plays_body, CAPICON, \'imgs/book_icon.gif\', CAPTION, \'KEY EUROPEAN PLAYS OF %2$s\', BORDER, 2, FGCOLOR, \'#eeffff\', BGCOLOR, \'#336666\', WIDTH, 250, PADX, 12, 12, PADY, 12, 12, OFFSETX, -50, OFFSETY, -140, SNAPY, 5, CLOSECLICK, STICKY)">Key European plays of %1$s</a>');
define('KEYPLAY_PLACE_HOLDER', '&nbsp;');

class KeyPlayList {

  var $overLibCode = '';
  var $linkCode;
  var $error;
  var $_list = array();
  var $_order = array();
  var $_year;

  function KeyPlayList($year) {
    $this->_setYear($year);
    $this->_setList();
    //  traceVar('KeyPlayList -- unordered', $this);
    $this->_sort();
    $this->_setOverLibCode();
    $this->_setLinkCode();
    traceVar('KeyPlayList -- done', $this);
  }

  function _setYear($year) {
    $year = (int) $year;
    if (!Validator::isValidYear($year)) {
      // too early
      // too late
      trace('bad value for year.');
      $this->error('illegal year');
    }
    else {
      $this->_year = $year;
    }
  }

  function _setList() {
    if ($this->error) {
      return false;
    }

    $query = "SELECT keyplayID, title, author from " . KEY_PLAY_TABLE . " WHERE date_year='$this->_year'";
    $query = db_select(KEY_PLAY_TABLE, 'k');
    $keyplayUnordered = $query
        ->fields('k', array('keyplayID', 'title', 'author'))
        ->condition('date_year', $this->_year)
        ->execute()
        ->fetchAll();
    // $keyplayUnordered = db_query_results($query, Q_TYPE_SELECT, __FILE__, __LINE__);
    if ($keyplayUnordered) {
      $this->_list = $keyplayUnordered;
    }
    else {
      $this->_list = array();
    }
  }

  function _sort() {
    if ($this->error) {
      return false;
    }

    $this->_setOrder();

    if (!is_array($this->_order)) {
      $this->error('expecting an array.');
      return false;
    }

    $keyplayUnordered = array();
    $keyplays = array();

    foreach ($this->_list as $onePlay) {
      $thisID = $onePlay->keyplayID;
      if (!isset($this->_order[$thisID])) { // if its not ordered put it in a temp. holding array
        $keyplayUnordered[] = array('title' => stripslashes($onePlay->title), 'author' => stripslashes($onePlay->author), 'id' => $thisID);
      }
      else { // if it is ordered, then start out sorted array.
        // LeeO 01/03/2004
        // test that this position hasn't already been taken
        if (isset($keyplays[$this->_order[$thisID]])) {
          trace('WARNING: two key plays with the same "order". Putting second into "unsorted" list.');
          $keyplayUnordered[] = array('title' => stripslashes($onePlay->title), 'author' => stripslashes($onePlay->author), 'id' => $thisID);
        }
        // end 01/03/2004
        $keyplays[$this->_order[$thisID]] = array('title' => stripslashes($onePlay->title), 'author' => stripslashes($onePlay->author), 'id' => $thisID);
      }
    }
    traceVar('keyplay-the sorted half', $keyplays);
    traceVar('keyplayUnordered', $keyplayUnordered);
    ksort($keyplays);

    foreach ($keyplayUnordered as $value) {
      $keyplays[] = $value;
    }

    /* */
    traceVar('keyplay-combined', $keyplays);
    $this->_list = $keyplays;
  }

  function _setOrder() {
    if ($this->error) {
      return false;
    }
    // returns an array on success, false otherwise 

    $orderGetQuery = 'SELECT keyplayIDs from ' . KEY_PLAY_ORDER_TABLE . ' WHERE year=' . $this->_year;
    $query = db_select(KEY_PLAY_ORDER_TABLE, 'k');
    $orderGetResult = $query
        ->fields('k', array('keyplayIDs'))
        ->condition('year', $this->_year)
        ->execute()
        ->fetchAssoc();
    // $orderGetResult = db_query($orderGetQuery, '', __FILE__, __LINE__);

    if (count($orderGetResult) < 1) {
      $this->_order = array(); // return an empty array if no records
    }
    else {

      $row = array_shift($orderGetResult);
      $orderList = trim($row, '|');
      $orderArray = explode('|', $orderList);
      $orderArray = array_flip($orderArray);
      $this->_order = $orderArray;
    }
  }

  function _setOverLibCode() {
    if ($this->error) {
      return false;
    }
    /*
      $keyPlays
      '
      <span class=play_title>Eugene O&#8217;Neill, <u>The Ice Man Cometh</u><br>
      Aristophanes, <u>The Birds</u><br>Aristophanes, <u>The Frogs</u><br>
      Tennessee Williams, <u>A Streetcar Named Desire</u><br>
      Judith Thompson, <u>White Biting Dog</u>
      </span>
      '
      keyplay-combined - Array
      (
      [0] => Array
      (
      [title] => Play Title "Sam"
      [author] => Author Sev'n
      [id] => 70
      )

      [1] => Array
      (
      [title] => Play Title 6
      [author] => Author Six
      [id] => 69
      )

      [2] => Array
      (
      [title] => Play Title Test 5
      [author] => Author 5
      [id] => 68
      )

      )
     */
    if (!empty($this->_list)) {
      $returnString[] = FMT_KEYPLAY_PRE; // '<span class=play_title>'
      foreach ($this->_list as $keyPlay) {
        $returnString[] = sprintf(FMT_KEYPLAY_ITEM, $keyPlay['author'], $keyPlay['title']);
      }
      $returnString[] = FMT_KEYPLAY_POST; // '</span>';
      $this->overLibCode = addslashes(implode('', $returnString));
    }
  }

  function _setLinkCode() {
    if ($this->error) {
      return false;
    }
    if (empty($this->_list)) {
      $this->linkCode = constant('KEYPLAY_PLACE_HOLDER');
    }
    else {
      $link = $this->_get_keyplay_link();
      $this->linkCode = sprintf($link, $this->_returnYearName(), strtoupper($this->_returnYearName()));
    }
  }

  function is_empty() {
    return empty($this->_list);
  }

  function _get_keyplay_link() {
    $module_path = drupal_get_path('module', 'agile_chronology');
    $image_path = "/$module_path/imgs";
    return '<a href="#" title="Click for key European plays of %1$s" class="tagline" onClick="event.preventDefault(); return overlib(key_plays_body, CAPICON, \'' . $image_path . '/book_icon.gif\', CAPTION, \'KEY EUROPEAN PLAYS OF %2$s\', BORDER, 2, FGCOLOR, \'#eeffff\', BGCOLOR, \'#336666\', WIDTH, 250, PADX, 12, 12, PADY, 12, 12, OFFSETX, -50, OFFSETY, -140, SNAPY, 5, CLOSECLICK, STICKY)">Key European plays of %1$s</a>';
  }

  /**
   * Returns a year, or for special years something like "the 1700s"
   */
  function _returnYearName() {
    if (Validator::isSpecialYear($this->_year)) {
      return 'the ' . $this->_year . 's';
    }
    else {
      return strval($this->_year);
    }
  }

  function error($message) {
    /*
      http://laughingmeme.org/archives/001055.html

      error() takes a message, and an optional error level, appends
      php_errormsg if track_errors is on, prepends a string identifying
      the library that is throwing the error, sets up a package/class
      variable with the resulting error, and, if debugging is on,
      triggers an error message.

      Why is this good? Code using your library has easy way to check for
      error conditions (if ($lib->error) { ... do error handling .. } ),
      error messages are very complete, and consistently formatted, when
      someone is developing with your app they can easily find out what
      went wrong based on their php.ini settings, and lastly if someone
      does need to hack or override your chosen behaviour, they only
      have to do it once.
     */
    $this->error = $message;
  }

}

/* * *************************************************************************
 *
 * $Log: class.keyplay.php,v $
 * Revision 1.1  2004/03/19 04:17:52  rdeil
 * *** empty log message ***
 *
 * Revision 1.6  2004/03/01 20:22:24  leeomara
 * bug - #2 Only 1 key play was ever being shown (problem was with sort)
 *
 * Revision 1.5  2004/02/28 23:21:47  leeomara
 * diff - tweaked format of links that open key play layer(now using two anchor tags -- one for the image, a second for the text).
 *
 * Revision 1.4  2004/01/30 03:04:52  leeomara
 * *** empty log message ***
 *
 * Revision 1.3  2003/12/19 04:57:55  leeomara
 * diff - added code comments
 *
 * Revision 1.1  1997/01/03 14:23:51  joe
 * Add the superfrobnicate option
 *
 */
?>