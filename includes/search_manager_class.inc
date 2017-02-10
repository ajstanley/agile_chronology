<?php

/*
  classes defined:
  SearchManagerBase
 */

class SearchManagerBase {
  /* controller for query objects
   * 
   * handles:
   * - session data (saved searches)
   * - query object creation
   * - page displaying: advanced, refine, results
   */

  var $queryObj;
  var $language;
  var $error = false;
  var $warning = false;
  var $year = '';
  var $colour = "CAN";

  function manage($prof = NULL) {
    module_load_include('inc', 'agile_chronology', 'includes/search_query');
    // Set last year and colour from session
    $this->setYear();
    $this->setColour();
    $_SESSION['queryObjCurrent'] = unserialize(serialize($_SESSION['queryObjCurrent']));
    if (isset($_SESSION['queryObjCurrent']) && is_object($_SESSION['queryObjCurrent']) && is_a($_SESSION['queryObjCurrent'], 'QueryAdvanced')) {
      //echo ' grab existing query object';
      // !! test that it is in fact the correct type of object
      $this->queryObj = $_SESSION['queryObjCurrent'];
    }
    if (isset($_GET['action']) && (!empty($_GET['action'])) && isset($this->queryObj)) {
      // do stuff
      $action = strtoupper($_GET['action']);
      switch ($action) {
        case('LIST'):
          if (isset($_GET['page'])) {
            $this->queryObj->setPage($_GET['page']);
          }
          $this->showPage();
          break;
        case('PRINT'):
          if (isset($_GET['page'])) {

            $this->queryObj->setPage($_GET['page']);

          }
          $this->showPrintPage();
          break;
        case('SAVE'):
          $this->saveQuery();
          $this->showPage();
          break;
        Case('REFINE'):
          //echo 'yeah, refine.';
          // if it's a simplequery, it needs to be 'translated'
          $this->showRefine();
          break;
        Case('LOAD1'):
          $this->loadSearch(0);
          $this->showPage();
          break;
        Case('LOAD2'):
          $this->loadSearch(1);
          $this->showPage();
          break;
        Case('LOAD3'):
          $this->loadSearch(2);
          $this->showPage();
          break;
        Case('PRINT'):
          break;
      }
      // save results(save queries)
      // refine queries
      // print results
      // list results
    }
    else {
      // new search
      if (isset($_GET['query'])) {
        $this->createQuery($prof);
        //print_r($this->queryObj);
        $this->showPage();
        // ?? what about error handling
        /// db problem ? zero results ?
        $_SESSION['queryObjCurrent'] = $this->queryObj;
      }
      else {
        //echo 'hey, I should show you the form.';
        // show form (defaults are used if nothing given)
        $this->showForm();
      }
    }
  }

  function SearchManagerBase($language, $prof = NULL) {
    $this->setLanguage($language, $prof);
  }

  function showForm($selected = array(), $checked = array(), $strings = array()) {
    // default values
    $defaultSelected = array('type1' => array('ANY' => '', 'PW' => '', 'PT' => '', 'TN' => '', 'TG' => '', 'AC' => '', 'DE' => '', 'PR' => '', 'GEO' => '', 'TXT' => ''),
      'type2' => array('ANY' => '', 'PW' => '', 'PT' => '', 'TN' => '', 'TG' => '', 'AC' => '', 'DE' => '', 'PR' => '', 'GEO' => '', 'TXT' => ''),
      'type3' => array('ANY' => '', 'PW' => '', 'PT' => '', 'TN' => '', 'TG' => '', 'AC' => '', 'DE' => '', 'PR' => '', 'GEO' => '', 'TXT' => ''),
      'operator2' => array('AND' => '',
        'OR' => '',
        'NOT' => '',
        'AND NOT' => ''),
      'operator3' => array('AND' => '',
        'OR' => '',
        'NOT' => '',
        'AND NOT' => ''),
      'order' => array('ASC' => '',
        'DESC' => '')
    );
    $defaultChecked = array('timeline' => array('CAN' => 'checked="checked"',
        'WEST' => 'checked="checked"'),
      'order' => array('ASC' => 'checked="checked"',
        'DESC' => '')
    );
    $defaultStrings = array('part1' => '', 'part2' => '', 'part3' => '', 'dateFirst' => '', 'dateLast' => '');

    // merge with incomming values
    $selected = array_merge($defaultSelected, $selected);
    $checked = array_merge($defaultChecked, $checked);
    $strings = array_merge($defaultStrings, $strings);
    $module_path = drupal_get_path('module', 'agile_chronology');
    include("$module_path/theme/tpl.search.php");
  }

  function saveQuery() {
    module_load_include('inc', 'agile_chronology', 'includes/search_query');

    if (!empty($this->queryObj)) {
      if ((!isset($_SESSION['saved'])) || (!is_array($_SESSION['saved']))) {
        $_SESSION['saved'] = array();
      }
      array_push($_SESSION['saved'], $this->queryObj);
      if (sizeof($_SESSION['saved']) > 3) {
        array_shift($_SESSION['saved']);
      }
      $GLOBALS['bodyOnload'] = "alert('Your search has been saved. access your three most recent saved search results in the left navigation bar.')";
      return true;
    }
  }

  function loadSearch($whatToLoad) {

    if (isset($_SESSION['saved'][$whatToLoad])) {
      if (is_subclass_of($_SESSION['saved'][$whatToLoad], 'QueryBase')) {
        $this->queryObj = $_SESSION['saved'][$whatToLoad];
        $_SESSION['queryObjCurrent'] = $_SESSION['saved'][$whatToLoad];
      }
      else {
        // no the right type of object
        trigger_error('Not the right type of object.', ERROR);
      }
    }
    else {
      // nothing found to load.
      //trigger_error('No saved search found.',ERROR); // !! should be dealt with more gently -- a warning, perhaps.
      $this->error('No saved search found.', ERROR); // !! should be dealt with more gently -- a warning, perhaps.
    }
  }

  function showRefine() {
   
    if (!empty($this->queryObj)) {
      $this->refine = TRUE;
      //print_r($this->queryObj);
      $selected = array('type1' => array('ANY' => '', 'PW' => '', 'PT' => '', 'TN' => '', 'TG' => '', 'AC' => '', 'DE' => '', 'PR' => '', 'GEO' => '', 'TXT' => ''),
        'type2' => array('ANY' => '', 'PW' => '', 'PT' => '', 'TN' => '', 'TG' => '', 'AC' => '', 'DE' => '', 'PR' => '', 'GEO' => '', 'TXT' => ''),
        'type3' => array('ANY' => '', 'PW' => '', 'PT' => '', 'TN' => '', 'TG' => '', 'AC' => '', 'DE' => '', 'PR' => '', 'GEO' => '', 'TXT' => ''),
        'operator2' => array('AND' => '',
          'OR' => '',
          'AND NOT' => '',
          'NOT' => ''),
        'operator3' => array('AND' => '',
          'OR' => '',
          'AND NOT' => '',
          'NOT' => ''),
        'order' => array('ASC' => '',
          'DESC' => '')
      );
      $checked = array('timeline' => array('CAN' => '',
          'WEST' => ''),
        'order' => array('ASC' => '',
          'DESC' => '')
      );

      $selected['type1'][$this->queryObj->queryDetails[1]['type']] = 'selected';

      if (isset($this->queryObj->queryDetails[2])) {
        $selected['operator2'][$this->queryObj->queryDetails[2]['operator']] = 'selected';
        $selected['type2'][$this->queryObj->queryDetails[2]['type']] = 'selected';
      }

      if (isset($this->queryObj->queryDetails[3])) {
        $selected['operator3'][$this->queryObj->queryDetails[3]['operator']] = 'selected';
        $selected['type3'][$this->queryObj->queryDetails[3]['type']] = 'selected';
      }

      switch ($this->queryObj->timeline) {
        case('BOTH'):
          $checked['timeline']['CAN'] = 'checked="checked"';
          $checked['timeline']['WEST'] = 'checked="checked"';
          break;
        case('CAN'):
          $checked['timeline']['CAN'] = 'checked="checked"';
          break;
        case('WEST'):
          $checked['timeline']['WEST'] = 'checked="checked"';
          break;
      }

      switch ($this->queryObj->order) {
        case('ASC'):
          $checked['order']['ASC'] = 'checked="checked"';
          break;
        case('DESC'):
          $checked['order']['DESC'] = 'checked="checked"';
          break;
      }

      // !!TODO 04/02/2004 check to see if magic quotes are one first
      $strings['part1'] = htmlspecialchars(stripslashes($this->queryObj->queryDetails[1]['string']));
      if (isset($this->queryObj->queryDetails[2]['string'])) {
        $strings['part2'] = htmlspecialchars(stripslashes($this->queryObj->queryDetails[2]['string']));
      }
      if (isset($this->queryObj->queryDetails[3]['string'])) {
        $strings['part3'] = htmlspecialchars(stripslashes($this->queryObj->queryDetails[3]['string']));
      }

      $strings['dateFirst'] = $this->queryObj->lowerYear ? $this->queryObj->lowerYear : 'YYYY';
      $strings['dateLast'] = $this->queryObj->upperYear ? $this->queryObj->upperYear : 'YYYY';
    }
    else {
      // nothing to refine
      trigger_error('nothing to refine');
    }
    $this->showForm($selected, $checked, $strings);
   
  }

  function createQuery($prof) {
    module_load_include('inc', 'agile_chronology', 'includes/db_util');
    module_load_include('inc', 'agile_chronology', 'includes/search_query');

   
    // find out what type of query (simple|advanced)
    // !! for now we'll just assume that its an advanced search.
    //create new query object
    // figure out timeline
    //$timelines = $_GET['timeline'];
    if (isset($_GET['timeline']['can']) && ($_GET['timeline']['can'])) {
      if (isset($_GET['timeline']['west']) && ($_GET['timeline']['west'])) {
        $timeline = 'both';
      }
      else {
        $timeline = 'can';
      }
    }
    elseif (isset($_GET['timeline']['west']) && ($_GET['timeline']['west'])) {
      $timeline = 'west';
    }
    else {
      //trigger_error('Missing required information (query timeline). Continuing with "both".',WARNING);
      $this->error('Missing required information (query timeline). Continuing with "both".', WARNING);
      $timeline = 'both';
    }

    // figure out order
    $order = strtoupper($_GET['order']);
    if (($order != 'ASC') AND ( $order != 'DESC')) {
      //trigger_error('Missing required information (query order). Continuing with "ascending"',WARNING);
      $this->error('Missing required information (query order). Continuing with "ascending"', WARNING);
      $order = 'ASC';
    }

    // get year limits
    // dates need not be given, but if they are, then they better be valid
    $lower = 1600;
    if (isset($_GET['dateFirst']) && !empty($_GET['dateFirst'])) {
      if (!Validator::isValidYear($_GET['dateFirst'])) {
        $this->error('Invalid date(from). Continuing with "1600"', WARNING);
        $lower = 1600;
      }
      else {
        $lower = (int) $_GET['dateFirst'];
      }
    }
    $upper = getLatestYear();
    if (isset($_GET['dateLast']) && !empty($_GET['dateLast'])) {
      if (!Validator::isValidYear($_GET['dateLast'])) {
        $this->error('Invalid date(to). Continuing with "$upper"', WARNING);
      }
      else {
        $upper = (int) $_GET['dateLast'];
      }
    }


    //$this->queryObj = new QueryAdvanced($_GET['query'], $timeline, 0, QUERY_LIMIT, $this->language);
    // clean up the incomming query -- remove slashes if required.
    if (get_magic_quotes_gpc()) {
      // remove extra slashes if appropriate
      $incommingQuery = rec_stripslashes($_GET['query']);
    }
    else {
      $incommingQuery = $_GET['query'];
    }
    $sam = new QueryAdvanced($incommingQuery, $lower, $upper, $timeline, $order, 0, 'QUERY_LIMIT', $this->language);
    //print_r($sam);
    $this->queryObj = $sam;
    
  }

  function createSimpleQuery() {
    //create new query object
  
  
    $t = new QuerySimple($_GET['query']);
    $this->queryObj = $t;
  
  }

  function showPage() {

  
    $module_path = drupal_get_path('module', 'agile_chronology');
    include("$module_path/theme/tpl.search_results.php");
   
    //$this->returnListOfRecords();        
    //$this->returnListOfOtherPages();  
  }

  function showPrintPage() {
    include('tpl.search_results_print.php');
  }

  function setLanguage($language, $prof = NULL) {

    $language = strtoupper($language);
    if (($language != 'EN') AND ( $language != 'FR')) {
      //trigger_error('Illegal value. Expecting: EN|FR, received: "'.$language.'", defauling to "EN"',WARNING);
      $this->error('Illegal value. Expecting: EN|FR, received: "' . $language . '", defauling to "EN"', WARNING);
      $language = 'EN';
    }
    $this->language = $language;

  }

  function returnListOfRecords() {
    module_load_include('inc', 'agile_chronology', 'includes/search_utilities');
    ob_start();
    if (empty($this->queryObj->recordList)) {
      // !! should be language neutral -- need something like getPhrase('SEARCH_NO_RESULTS')
      echo '<p class="noresults">' . TXT_EN_SEARCH_NO_RESULTS . '</p>';
    }
    else {
      $count = $this->queryObj->page * $this->queryObj->limit + 1;
      echo "<ol start='$count' id=\"results\">";
      $recordList = unserialize(serialize($this->queryObj->recordList));
      foreach ($recordList as $result) {
     
        printf('<li>%s %s</li>', $result->returnDate(), $result->body);
        //print('<li>'.$result->returnDate().$result->body.'</li>');
      }
      echo'</ol>';
    }
    ob_end_flush();

  }

  function returnPrintListOfRecords() {

    ob_start();
    if (empty($this->queryObj->recordList)) {
      echo '<p class="noresults">' . TXT_EN_SEARCH_NO_RESULTS . '</p>';
    }
    else {
      $count = $this->queryObj->page * $this->queryObj->limit + 1;
      echo "<ol start='$count'>";
      foreach ($this->queryObj->recordList as $result) {
        printf('<li>%s %s</li>', $result->returnDateLinkPrint(), $result->body);
        //print('<li>'.$result->returnDate().$result->body.'</li>');
      }
      echo'</ol>';
    }
    ob_end_flush();

  }

  function returnListOfOtherPages() {

    echo '<ol id="pagination">';
    if (!empty($this->queryObj->recordList)) {
      $noOfPages = ceil($this->queryObj->count / $this->queryObj->limit);
      //echo $noOfPages.'<br>';
      for ($i = 0; $i < $noOfPages; $i++) {
        if ($i == $this->queryObj->page) {
          printf("<li><strong>%d</strong></li>", $i + 1);
        }
        else {
          printf("<li><a href='%s?action=list&amp;page=%d' class='global' title='page %d'>%d</a></li>", '/agile/search', $i, $i + 1, $i + 1);
        }
      }
    }
    echo '</ol>';
  }

  function setYear() {
    if (isset($_SESSION["year"])) {
      $this->year = $_SESSION["year"];
    }
    else {
      $this->year = "1800";
    }
  }

  function setColour() {
    if (isset($_SESSION["colour"])) {
      $this->colour = $_SESSION["colour"];
    }
    else {
      $this->colour = "can";
    }
  }

  function returnNotices() {
    $msg = '';
    // grab errors and warnings from queryObj too
    if ((isset($this->queryObj->error)) && (!empty($this->queryObj->error))) {
      $this->error = array_merge($this->error, $this->queryObj->error);
    }
    if ((isset($this->queryObj->warning)) && (!empty($this->queryObj->warning))) {
      $this->warning = array_merge($this->warning, $this->queryObj->warning);
    }
    if ($this->error) {
      $format = '<h3 style="color:red">Error:<br> %s</h3>';
      $msg .= sprintf($format, $implode('<br>', $this->error));
    }
    else {
      $msg .= '<!-- no errors -->';
    }
    if ($this->warning) {
      $format = '<h3 style="color:red">Warning:<br> %s</h3>';
      $msg .= sprintf($format, implode('<br>', $this->warning));
    }
    else {
      $msg .= '<!-- no warnings -->';
    }
    echo($msg); /**/
  }

  function error($msg, $code = '') {
    switch ($code) {
      case WARNING:
        $this->warning[] = $msg;
        break;
      case ERROR:
        $this->error[] = $msg;
        //include('./error.php');
        exit;
        break;
      default:
        $this->error[] = $msg;
        trigger_error(get_class($this) . ': ' . $msg, FATAL);
    }
  }

}

?>