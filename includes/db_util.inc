<?php

if (isset($_GET['trace'])) {
  $hideTrace = false;
}
else {
  $hideTrace = true;
}

if (isset($_GET['prof'])) {
  $hideProf = false;
}
else {
  $hideProf = true;
}

if (!$hideTrace) {
  echo '<pre>';
}

//db_connect();
//session_start();

function getLatestYear() {
  // current year or 2020, whichever is lower
  $latestYear = date("Y");
  if ($latestYear > 2020)
    $latestYear = 2020;
  return $latestYear;
}

function db_connectx() {
  global $db;
  $token = mysqli_connect($db['Server'], $db['User'], $db['Pass']) or trigger_error("Can't connect to DB Server. " . ((is_object($token)) ? mysqli_error($token) : mysqli_connect_error()), E_USER_ERROR);
  mysqli_set_charset($token, 'utf8');
  ((bool) mysqli_query($token, "USE " . $db['Name'])) or trigger_error("Can't load DB. " . ((is_object($token)) ? mysqli_error($token) : mysqli_connect_error()), E_USER_ERROR);
  ;
  return $token;
}

function db_query_results($queryString, $queryType = '', $callingFile = 'unknown', $callingLine = '??') { // execute a query and return an appropriate result 
  /*
    type is either:
    Q_TYPE_SELECT, Q_TYPE_UPDATE, Q_TYPE_DELETE, Q_TYPE_INSERT, Q_TYPE_SELECT_SINGLE, Q_TYPE_SELECT_LIST
   */
  $rows = array();
  $results = db_query($queryString);
  $raw_rows = $results->fetchAll();

  foreach ($raw_rows as $row) {
    if (isset($row->recordID)) {
      $rows[] = $row->recordID;
    }
    if (isset($row->mapRecordID)) {
      $rows[] = $row->mapRecordID;
    }
  }
  $count = count((array) $raw_rows[0] > 1);
  if ($raw_rows && count((array) $raw_rows[0]) >1) {
    $rows = $raw_rows;
    
  }
  if (!is_resource($results) && ($results == false)) {
    return -1;
  }
  if (empty($queryType)) {
    $queryType = firstWord($queryString);
  }
  // act based on type of query
  switch ($queryType) {
    case(Q_TYPE_SELECT_LIST): // Select query -- return all data as a 1D array (even if only 1 record)
    // pass
    case(Q_TYPE_SELECT): // Select query -- return all data as array of obj (even if only 1 record)
      if (!$rows) {
        return false;
      }
      $resultsArray = $rows;


      return $resultsArray;
      break;
    case(Q_TYPE_SELECT_SINGLE): // returns the first row as an object unless there is only one field, then the value is returned.
      if (mysqli_num_rows($results) < 1) {
        return false;
      }
      switch ((($___mysqli_tmp = mysqli_num_fields($results)) ? $___mysqli_tmp : false)) {
        case(1):
          $row = mysqli_fetch_row($results);
          Return $row[0];
          break;
        default:
          $firstObj = mysqli_fetch_object($results);
          return $firstObj;
          break;
      }
      break;
    case(Q_TYPE_UPDATE):
    case(Q_TYPE_DELETE):
    case(Q_TYPE_INSERT):
      return mysqli_affected_rows($GLOBALS["con"]);
      break;
  }
}

function db_escape_string($string) {
  return $string;
  return mysqli_real_escape_string($string);
}

function firstWord($phrase) { // returns the first word of a phrase
  $words = explode(' ', trim($phrase));
  return strtolower($words[0]);
}

define('Q_TYPE_SELECT', 'select');
define('Q_TYPE_UPDATE', 'update');
define('Q_TYPE_DELETE', 'delete');
define('Q_TYPE_INSERT', 'insert');
define('Q_TYPE_SELECT_SINGLE', 'selectsingle');
define('Q_TYPE_SELECT_LIST', 'selectlist');

function traceVar($label, $value) {
  return "traceVAR";
  if (isset($GLOBALS['hideTrace'])) {
    Return true;
  }
  else {
    trace("\n $label - " . gettype($value));
    if (empty($value)) {
      $value = "** empty **\n";
    }
    print_r($value);
    trace("\n");
  }
}

function trace($msg) {
  if (isset($GLOBALS['hideTrace'])) {
    Return true;
  }
  else {
    //	echo($msg."\n"  );
  }
}

function preg_addslashes($foo) {
  return preg_replace("/([^A-z0-9_-]|[\\\[\]])/", "\\\\\\1", $foo);
}

// redefine the user error constants - PHP 4 only
define("FATAL", E_USER_ERROR);
define("ERROR", E_USER_WARNING);
define("WARNING", E_USER_NOTICE);

// set the error reporting level for this script
//error_reporting (FATAL | ERROR | WARNING);
//error_reporting (FATAL | ERROR);
// error handler function
function myErrorHandler($errno, $errstr, $errfile, $errline) {
  $errorLog = "./error.log";
  error_log($errstr, 3, $errorLog);
  switch ($errno) {
    case FATAL:
      error_log($errstr, 1, 'lomara@symaptico.ca');
      include('./error.php');
      //echo "<b>FATAL</b> [$errno] $errstr<br />\n";
      //echo "  Fatal error in line $errline of file $errfile";
      //echo "Aborting...<br />\n";
      exit(1);
      break;
    case ERROR:
      echo "<b>ERROR</b> [$errno] $errstr<br />\n";
      break;
    case WARNING:
      echo "<b>WARNING</b> [$errno] $errstr<br />\n";
      break;
    default:
      echo "Unkown error type: [$errno] $errstr<br />\n";
      break;
  }
  echo "  in line $errline of file $errfile \n\n\n<br>";
}

function comment($message) {
  /**/
  if (isset($GLOBALS['hideTrace'])) {
    Return "<!-- $message -->";
  }
  else {
    Return "[$message]";
  }//
}

function rec_stripslashes($vals) {
  // recursive stripslashes 
  // doesn't work with Objects !!
  if (is_array($vals)) {
    foreach ($vals as $key => $val) {
      $vals[$key] = rec_stripslashes($val);
    }
  }
  else {
    $vals = stripslashes($vals);
  }
  return $vals;
}

// set to the user defined error handler
//$old_error_handler = set_error_handler("myErrorHandler");

/**
 * utility class to centralise data checking
 */
class Validator {

  /**
   * Check for valid months. If $month is less than 1 or greater than 12 it is invalid.
   *
   * @param $month (int) number to test
   * @return (boolean) 
   */
  static function isValidMonth($month) {
    $month = (int) $month;
    if ($month < 1 || $month > 12) {
      Return false;
    }
    else {
      Return true;
    }
  }

  /**
   * Test a string to see if it is a valid chronology. 
   * A value is legal if after being uppercased, it is 'CAN' or 'WEST'
   *
   * @param $chronology (string) string to test
   * @return (boolean)
   */
  static function isValidChronology($chronology) {
    $chronology = strtoupper($chronology);
    if ($chronology == 'WEST' || $chronology == 'CAN') {
      Return true;
    }
    else {
      Return false;
    }
  }

  /**
   * Test a number to see if it is a valid year. 
   * A value is legal if it is between 1865 and the current year up to a max of 2020 or if it is 1600, 1700 or 1800
   *
   * @param $year (int) number to test
   * @return (boolean)
   * @see isSpecialYear()
   */
  static function isValidYear($year) {
    $year = (int) $year;
    if ($year >= EARLIEST && $year <= getLatestYear()) {
      Return true;
    }
    else {
      if (Validator::isSpecialYear($year)) {
        Return true;
      }
      else {
        Return false;
      }
    }
  }

  /**
   * Test a number to see if it is a valid decade. 
   * A value is legal if it is between 186 and 200.
   *
   * @param $decade (int) number to test
   * @return (boolean)
   */
  static function isValidDecade($decade) {
    $decade = (int) $decade;
    $earliestDecade = (int) (constant('EARLIEST') / 10);
    $latestDecade = (int) (getLatestYear() / 10);
    if ($decade < $earliestDecade || $decade > $latestDecade) {
      Return false;
    }
    else {
      Return true;
    }
  }

  /**
   * Test a number to see if it is 1600, 1700 or 1800. 
   *
   * @param $year (int) number to test
   * @return (boolean)
   */
  static function isSpecialYear($year) {
    $year = (int) $year;
    return in_array($year, array(1600, 1700, 1800));
  }

  /**
   * Test a string to see if it is 'AND', 'OR', 'NOT'. 
   *
   * @param $operator (string) string to test
   * @return (boolean)
   */
  static function isValidOperator($operator) {
    return in_array(strtoupper($operator), array('AND', 'OR', 'NOT'));
  }

}

?>