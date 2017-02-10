<?php

/*
  classes defined:
  QueryBase
  QueryAdvanced extends QueryBase
 */

class QueryAdvanced extends QueryBase {

  var $queryCan;
  var $queryWest;
  var $queryCount;

  /**
   * sorted list of records that match this query */
  var $wholeRecordList = false;
  var $queryWhere;
  var $terms = array();

  /**
   * process incomming data into the data structure that will be used to build our queries
   */
  function setQueryDetails($queryArray) { // should be 'returnQueryDetails.
    if (!isset($queryArray)) {
      trigger_error('Missing required information (query queryArray). Cannot proceed.', ERROR);
      return false;
    }
    if (!is_array($queryArray)) {
      trigger_error('Wrong variable type (query queryArray). Cannot proceed.', ERROR);
      return false;
    }
    if ((empty($queryArray))) {
      trigger_error('Information missing (query queryArray).', ERROR);
      return false;
    }

    $firstFlag = TRUE; // allows the first term to skip out on needing an operator
    $queryWhere = ' WHERE ';
    foreach ($queryArray as $key => $piece) {
      //print_r($this->queryDetails);            
      // get a list of termIDs for that search term
      if (!empty($piece['string'])) {
        $this->queryDetails[$key]['string'] = $piece['string'];

        $this->_set_details_type($key, $piece);

        $this->_set_details_terms($key, $piece);

        if ($firstFlag) { // the first query term doesn't get an operator.
          // just a placeholder, since the first term doesn't require an operator.    
          $this->queryDetails[$key]['operator'] = '';
        }
        else {
          $this->_set_details_operator($key, $piece);
        }

        $firstFlag = FALSE;
      }
      elseif ($firstFlag) {
        $this->queryDetails = '';
        $this->error('Information missing (no query given).', WARNING);
        return false;
      }
      else {
        break;
      }
    }
    if (empty($this->queryDetails)) {
      trigger_error('Information missing (no query given).', ERROR);
      return false;
    }
  }

  /**
   * helper function
   */
  function _set_details_terms($key, $piece) {
    if ($piece['type'] == 'TXT') { // special treatment for full-text.
      $this->queryDetails[$key]['terms'] = $piece['string'];
    }
    else {
      $this->queryDetails[$key]['terms'] = $this->getTermMatches($piece['string'], $piece['type']);
      if ($this->queryDetails[$key]['terms']) {
        // if terms were found, tack them on to our running list ($this->terms)
        if (isset($piece['operator']) && Validator::isValidOperator($piece['operator']) && strtoupper($piece['operator']) != 'NOT') {
          $this->terms = $this->queryDetails[$key]['terms'] + $this->terms;
        }
      }
      else {
        // even if none found, load with an empty array.
        $this->queryDetails[$key]['terms'] = array();
      }
    }
  }

  /**
   * helper function
   */
  function _set_details_operator($key, $piece) {
    if (isset($piece['operator'])) {
      if (Validator::isValidOperator($piece['operator'])) {
        $this->queryDetails[$key]['operator'] = strtoupper($piece['operator']);
        // 'NOT'  must be adjusted to make SQL logic work.
        if ($this->queryDetails[$key]['operator'] == 'NOT') {
          $this->queryDetails[$key]['operator'] = 'AND NOT';
        }
      }
      else {
        $this->error('Illegal value. Expecting: AND|OR|NOT, defaulting to "OR"', WARNING);
        $this->queryDetails[$key]['operator'] = 'OR';
      }
    }
    else {
      $this->error('Variable not set. Expecting: AND|OR|NOT, defaulting to "OR"', WARNING);
      $this->queryDetails[$key]['operator'] = 'OR';
    }
  }

  /**
   * helper function
   */
  function _set_details_type($key, $piece) {
    if (isset($piece['type']) && $this->isValidTermType($piece['type'])) {
      $this->queryDetails[$key]['type'] = $piece['type'];
    }
    else {
      //trigger_error('Bad or missing information(query term type). Continuing with "ANY".',WARNING);
      $this->error('Bad or missing information(query term type). Continuing with "ANY".', WARNING);
      $piece['type'] = 'ANY';
      $this->queryDetails[$key]['type'] = $piece['type'];
    }
  }

  function isValidTermType($code) {
    //Return in_array($code, array('ANY','PW','TN','TG','AC','DE','PR','GEO','TXT') );
    Return in_array($code, array('ANY', 'PW', 'PT', 'TN', 'TG', 'AC', 'DE', 'PR', 'GEO', 'TXT'));
  }

  function setForString() {
    // creates search label
    if (!empty($this->error))
      return false;
    if (empty($this->queryDetails))
      return false;
    $this->forString = '';
    foreach ($this->queryDetails as $piece) {
      if (!empty($piece['string'])) {
        if (!empty($piece['operator'])) {
          switch ($piece['operator']) {
            case ('AND NOT'):
              $this->forString .= ' - ';
              break;
            case ('OR'):
              $this->forString .= ' / ';
              break;
            case ('AND'):
            default:
              $this->forString .= ' + ';
              break;
          }
        }
        $this->forString .= $piece['string'];
      }
    }
  }

  function _returnWhereDateClause() {
    return " LEFT(recordDate,4) >=$this->lowerYear AND LEFT(recordDate,4) <= $this->upperYear ";
  }

  function prepare($queryArray = array()) {
    if (empty($this->queryDetails)) {
      $this->setQueryDetails($queryArray);
      #$this->setIndexTables();
      if (!empty($this->error))
        return false;
      $this->setForString();
    }
    if ($this->wholeRecordList === false) {
      // get record list queries (to build the list)
      $this->buildRecordList();
    }

    // get recordID set for this page
    $queryWhereInList = $this->_returnWhereInList();

    if (empty($this->error) && ($this->wholeRecordList !== false)) {

      $recordBody = 'recordBody' . $this->language;

      $canIntro = 'SELECT DISTINCT recordID, recordDate, recordDate as year, ' . $recordBody . ' as body FROM ' . CAN_TABLE;
      $westIntro = 'SELECT DISTINCT recordID, recordDate, MONTHNAME(recordDate) as month, MONTH(recordDate) as monthD, YEAR(recordDate) as year, ' . $recordBody . ' as body FROM ' . WEST_TABLE;

      // build limits and query end
      $queryEnd = 'ORDER BY recordDate ' . $this->order;

      // assemble main queries
      switch ($this->timeline) {
        case('BOTH'):
          $this->queryCan = $canIntro . $queryWhereInList['CAN'] . $queryEnd;
          $this->queryWest = $westIntro . $queryWhereInList['WEST'] . $queryEnd;
          break;
        case ('CAN'):
          $this->queryCan = $canIntro . $queryWhereInList['CAN'] . $queryEnd;
          unset($this->queryWest);
          break;
        case ('WEST'):
          unset($this->queryCan);
          $this->queryWest = $westIntro . $queryWhereInList['WEST'] . $queryEnd;
          break;
      }

      //$this->setTermIDs();            
      //print_r($this);
    }
  }

  /**
   * create a sorted list of records that match the criteria.
   *
   * this list is used as the basis of all record retrival queries
   * we only look for terms once, then make a master list(sorted) 
   * and use the recordIDs to feed additional queries (page 2, 3, ...)
   *
   * must be re-run whenever the criteria changes
   */
  function buildRecordList() {

    //	$prof->startTimer( 'QueryAdvanced.buildRecordList');
    module_load_include('inc', 'agile_chronology', 'includes/search_utilities');
    if (empty($this->queryDetails)) {
      trigger_error('Missing required information (queryDetails). Cannot proceed.', ERROR);
      return false;
    }
    if (!empty($this->error))
      return false;
    if (empty($this->queryDetails))
      return false;

    // for each termset, get rough query of records mapped (for each timeline)
    //  timeline neutral process -- no timeline tables are used untill just before the merge step.
    foreach ($this->queryDetails as $key => $piece) {
      trace("<hr>term $key");
      // special treatment for full-text searches
      if ($piece['type'] == 'TXT') {
        // if full-text, search both timelines
        $recordBody = 'recordBody' . $this->language;
        traceVar('stringIn', $piece['terms']);
        $quote_safe = SearchUtility::translateQuotes($piece['terms']);
        $accent_variable = SearchUtility::translateAccentsRegexp($quote_safe);
        $search_term = SearchUtility::translateWildCardsRegexp($accent_variable);
        $whereClause = ' ' . $recordBody . " RLIKE '" . db_escape_string($search_term) . "' ";
        if ($this->timeline == 'BOTH' || $this->timeline == 'CAN') {
          $qtermMatch = 'SELECT DISTINCT recordID FROM ' . CAN_TABLE . ' WHERE ' . $whereClause;
          // run query
          $tmp_records_can = db_query($qtermMatch)->fetchAll(PDO::FETCH_ASSOC); // if none found, use an empty array
          $holder = array();
          foreach($tmp_records_can as $can){
            $holder[] = $can['recordID'];
          }
          $tmp_records_can =$holder;
          $tmp_records = $tmp_records_can;
        }
        if ($this->timeline == 'BOTH' || $this->timeline == 'WEST') {
          $qtermMatch = 'SELECT DISTINCT recordID FROM ' . WEST_TABLE . ' WHERE ' . $whereClause;
          // run query
          $tmp_records_west = db_query($qtermMatch)->fetchAll(PDO::FETCH_ASSOC); // if none found, use an empty array
          //          $holder = array();
          foreach($tmp_records_west as $west){
            $holder[] = $west['recordID'];
          }
          $tmp_records_west = $holder;
          # are there canadian records to be merged with?
          if (isset($tmp_records_can) && is_array($tmp_records_can)) {
            $merged = array_merge($tmp_records_can, $tmp_records_west);
            $tmp_records = array_unique($merged);
          }
          else {
            $tmp_records = $tmp_records_west;
          }
        }
      }
      else {
        if (sizeof($piece['terms']) > 0) {
          // implode term list
          $coma_sep_term_list = implode(',', array_keys($piece['terms']));
          // build query
          $qtermMatch = 'SELECT DISTINCT mapRecordID FROM ' . INDEX_TABLE . ' WHERE mapTermID IN (' . $coma_sep_term_list . ')';
          // run query
          $tmp_records = db_query_results($qtermMatch, Q_TYPE_SELECT_LIST, __FILE__, __LINE__); // if none found, use an empty array			
        }
        else {
          $tmp_records = false;
        }
      }
      if (!$tmp_records) {
        $tmp_records = array();
      }

      // use query detail boolean operator to combine/reduce result set.
      if (empty($piece['operator'])) { // first term doesn't have an operator
        $runningList = $tmp_records;
      }
      else {
        switch ($piece['operator']) {
          case ('AND NOT'):
            trace('boolean NOT -- array_diff');
            $runningList = array_diff($runningList, $tmp_records);
            break;
          case ('OR'):
            trace('boolean OR -- array_merge');
            $runningList = array_unique(array_merge($runningList, $tmp_records));
            break;
          case ('AND'):
          default:
            trace('boolean AND -- array_intersect');
            $runningList = array_intersect($runningList, $tmp_records);
            break;
        }
      }
    }

    // go through the list and remove any empty items
    $tmp = array();
    foreach ($runningList as $value) {
      if (!empty($value)) {
        if(is_object($value)) {
          $tmp[] = $value->mapRecordID;
        }
        else {
          $tmp[] = $value;
        }
      }
    }
    $runningList = $tmp;

    // if no results are found, don't keep processing
    if (empty($runningList) || (count($runningList) < 1)) {
      $this->wholeRecordList = array();
      return false;
    }

    // query record tables for match(to recordID) within timeline restrictions
    //  must be done once for each timeline
    //  implode record list
    $coma_sep_record_list = implode(',', $runningList);

    //  sort order
    $orderby = ' ORDER BY recordDate ' . $this->order;

    $whereClause = ' WHERE ' . $this->_returnWhereDateClause() . ' AND recordID IN (' . $coma_sep_record_list . ')';
    $canQuery = 'SELECT DISTINCT  recordID, recordDate as "year" FROM ' . CAN_TABLE . $whereClause . $orderby;
    $westQuery = 'SELECT DISTINCT recordID, YEAR(recordDate) as "year" FROM ' . WEST_TABLE . $whereClause . $orderby;

    switch ($this->timeline) {
      case('BOTH'):
        // count everything -- no join
        $canResults = db_query($canQuery)->fetchAll();
        $westResults = db_query($westQuery)->fetchAll();

        break;
      case ('CAN'):
        $canResults = db_query($canQuery);
        break;
      case ('WEST'):
        $westResults = db_query($westQuery);
        break;
    }

    $canList = array();
    $westList = array();
    if (!empty($canResults)) {
      foreach ($canResults as $canRecordObj) {
        $canRecordObj->timeline = 'CAN';
        $canList[] = $canRecordObj;
      }
    }
    if (!empty($westResults)) {
      foreach ($westResults as $westRecordObj) {
        $westRecordObj->timeline = 'WEST';
        $westList[] = $westRecordObj;
      }
    }

    // have list sorted and merged into master result set.
    $this->wholeRecordList = SearchUtility::mergeLists($canList, $westList, 10000000, $this->order);
    // $prof->stopTimer('QueryAdvanced.buildRecordList');
  }

  /**
   * return a where clause for the record retrieval query 
   * based on the recordlist, current page and limit per page
   */
  function _returnWhereInList() {
    $list = array();
    if (!empty($this->error))
      return false;
    if (empty($this->queryDetails))
      return false;

    // get slice of recordlist
    $offset = $this->limit * $this->page;
    $thisPage = array_slice($this->wholeRecordList, $offset, 10);

    // look for records from this timeline
    foreach ($thisPage as $record) {
      $list[$record->timeline][] = $record->recordID;
    }

    // build where clause
    if (isset($list['CAN'])) {
      $where['CAN'] = ' WHERE recordID IN (' . implode(',', $list['CAN']) . ') ';
    }
    else {
      $where['CAN'] = ' WHERE 0 '; // !! could be optimized so that this query doesn't even happen
    }
    if (isset($list['WEST'])) {
      $where['WEST'] = ' WHERE recordID IN (' . implode(',', $list['WEST']) . ') ';
    }
    else {
      $where['WEST'] = ' WHERE 0 ';
    }

    return $where;
  }

  function getTermMatches($string, $type) {
    module_load_include('inc','agile_chronology', 'includes/search_utilities');
    $q = buildWordMatchQuery($string, $type);
    if (empty($q)) {
      Return array();
    }
    
    
    // rebuild here
    $query = $q['q'];
    $values =  $q['vals'];
    $resultset = db_query($query, $values);
    $results = $resultset->fetchAll();
    
    
    
    if (!$results) {
      return false;
    }
    $tmp = array();
    foreach ($results as $rowObj) {
      $tmp[$rowObj->termsID] = $rowObj->termsName;
    }
    return $tmp;
  }

  function formatResults() {
    global $format_prof;
    $format_prof = new Profiler_lee(false);
    foreach ($this->recordList as $key => $recordObj) {
      $format_prof->startTimer('SearchResultsRecord.formatBody');
      $this->recordList[$key]->formatBody($this->queryDetails);
      $format_prof->stopTimer('SearchResultsRecord.formatBody');
    }
    $format_prof->printTimers(false);
  }

  function getResults() {
    module_load_include('inc', 'agile_chronology', 'includes/search_utilities');
    module_load_include('inc', 'agile_chronology', 'includes/db_util');

    $canList = array();
    $westList = array();
    if (empty($this->error)) {
      $this->recordList = array();
      $factory = new SearchRecordFactory($this->language);
      if (isset($this->queryCan)) {
        //  $prof->startTimer('QueryAdvanced.getResults_record_query_canadian');
        $canResults = db_query_results($this->queryCan);
        //  $prof->stopTimer("QueryAdvanced.getResults_record_query_canadian");
        if (!empty($canResults)) {
          // $prof->startTimer("QueryAdvanced.getResults_creating_canadian_record_objercts");
          foreach ($canResults as $canRecordObj) {
            // process the records into a nice list.
            //  $prof->startTimer('SearchResultsRecord.constructor');
            $tmpObj = $factory->createRecordCan($canRecordObj);
            //  $prof->stopTimer('SearchResultsRecord.constructor');
            //$tmpObj->formatText($this->terms);

            /* $prof->startTimer('SearchResultsRecord.formatBody');
              $tmpObj->formatBody($this->queryDetails);
              $prof->stopTimer('SearchResultsRecord.formatBody');
             */
            $canList[] = $tmpObj;
          }
        
        }
      }
      if (isset($this->queryWest)) {
        // $prof->startTimer("QueryAdvanced.getResults_record_query_west");
        $westResults = db_query_results($this->queryWest);
        //  $prof->stopTimer("QueryAdvanced.getResults_record_query_west");
        if (!empty($westResults)) {
         // $prof->startTimer('QueryAdvanced.getResults_creating_west_record_objects');
          //$this->recordList += $westList;
          foreach ($westResults as $westRecordObj) {

            //$tmpObj->formatText($this->terms);
            // process the records into a nice list.
            //   $prof->startTimer('SearchResultsRecord.constructor');
            $tmpObj = $factory->createRecordWest($westRecordObj);
            //   $prof->stopTimer('SearchResultsRecord.constructor');
            //$tmpObj->formatText($this->terms);

            /* $prof->startTimer('SearchResultsRecord.formatBody');
              $tmpObj->formatBody($this->queryDetails);
              $prof->stopTimer('SearchResultsRecord.formatBody');
             */
            $westList[] = $tmpObj;
          }
          //  $prof->stopTimer('QueryAdvanced.getResults_creating_west_record_objects');
        }
      }

      // sort the two array into one.
      $this->limit = 10;
      $this->recordList = SearchUtility::mergeLists($canList, $westList, $this->limit, $this->order); // !!
    }
  }

}

/**
 * abstact class
 */
class QueryBase {

  var $queryDetails = array(); //< POST or GET, info from the submitted query        
  var $recordList = array(); //< array of records returned by the query for display
  var $page; //< what page to show ? (start counting at zero)
  var $count; //< how many records match the search
  var $limit;
  var $forString; //< show what the (users)query string was.
  var $timeline; //< CAN | WEST
  var $order; //< ASC | DESC
  var $language; //< EN | FR
  var $lowerYear;
  var $upperYear;
  var $error = array();
  var $warning = array();

  function QueryBase($query, $lower = '', $upper = '', $timeline = '', $order = 'ASC', $page = 0, $limit = 10, $language = 'EN') {

    //$prof->startTimer( "Query_init" );
    // store query info
    $this->setLanguage($language);
    $this->setTimeline($timeline);
    $this->setOrder($order);
    $this->page = (int) $page;
    $this->limit = (int) $limit;
    $this->setLowerY($lower);
    $this->setUpperY($upper);
    // $prof->stopTimer( "Query_init" );

    if (!empty($this->error))
      return FALSE;
    // build queries (need at least two. One to get the 10 records to display now, and one to count the total number of matches)
    // $prof->startTimer( "Query_prepare" );
    $this->prepare($query);
    // $prof->stopTimer( "Query_prepare" );

    if (!empty($this->error))
      return FALSE;
    // execute query == get records
    // $prof->startTimer("Query.getResults");
    $this->getResults();
    // $prof->stopTimer("Query.getResults");
    if (!empty($this->error))
      return FALSE;
    //  $prof->startTimer("Query.formatResults");
    $this->formatResults();
    //   $prof->stopTimer("Query.formatResults");
    if (!empty($this->error))
      return FALSE;
//    $prof->startTimer("Query.setCount");
    $this->setCount();
    //   $prof->stopTimer("Query.setCount");
  }

  function prepare($query) {
    // placeholder for real function
    trigger_error('"placeholder" not defined');
    die;
  }

  function getResults() {
    // placeholder for real function
    trigger_error(' "getResults"  not defined');
    die;
  }

  function formatResults() {
    // placeholder for real function
    trigger_error(' "formatResults"  not defined');
    die;
  }

  function setLowerY($year) {
    $year = (int) $year;
    //echo $year;
    $this->lowerYear = $year;
  }

  function setUpperY($year) {
    $year = (int) $year;
    $this->upperYear = $year;
  }

  function nextPage() {
    return $this->setPage($this->page + 1);
  }

  function setPage($newPage) {
    $newPage = (int) $newPage;
    if ((($newPage) * $this->limit) > $this->count) {
      $this->error('Illegal value. No more results, show last shown page.', WARNING);
      $this->prepare();
      $this->getResults();
      return false;
    }
    $this->page = $newPage;
    $this->prepare();
    $this->getResults();
    $this->formatResults();
    return TRUE;
  }

  function setLanguage($language) {
    $language = strtoupper($language);
    if (($language != 'EN') AND ( $language != 'FR')) {
      $this->error('Illegal value. Expecting: EN|FR, received: "' . $language . '", defauling to "EN"', WARNING);
      $language = 'EN';
    }
    $this->language = $language;
  }

  function setTimeline($timeline) {
    $timeline = strtoupper($timeline);
    if (($timeline != 'CAN') AND ( $timeline != 'WEST') AND ( $timeline != 'BOTH')) {
      $this->error('Illegal value for timeline. Expecting: CAN|WEST|BOTH, received: "' . $timeline . '", defauling to "BOTH"', WARNING);
      $timeline = 'BOTH';
    }
    $this->timeline = $timeline;
  }

  function setOrder($order) {
    $order = strtoupper($order);
    if (($order != 'ASC') AND ( $order != 'DESC')) {
      $this->error('Illegal value order. Expecting: ASC|DESC, received: "' . $order . '", defauling to "ASC"', WARNING);
      $order = 'ASC';
    }
    $this->order = $order;
  }

  function setCount() {
    if (!empty($this->error))
      return false;
    if (empty($this->queryDetails)) {
      $this->count = 1;
    }
    if ($this->wholeRecordList !== false) {
      $this->count = sizeof($this->wholeRecordList);
    }
  }

  function error($msg, $code = '') {
    switch ($code) {
      case WARNING:
        $this->warning[] = $msg;
        break;
      case ERROR:
        $this->error[] = $msg;
        include('./error.php');
        exit;
        break;
      default:
        $this->error[] = $msg;
        trigger_error(get_class($this) . ': ' . $msg, FATAL);
    }
  }

}

?>