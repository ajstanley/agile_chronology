<?php

/**
 *
 */
// 
// constants definitions
/// The number of search results to retrieve from the DB
define('QUERY_LIMIT', 10);

/// The number of characters to trim the 'snippet' down to
define('QUOTE_LENGTH', 200);

/// formatting string controlling the HTML output of links to canadian records
define('SEARCH_RESULTS_LINK_CAN', "<a href=\"can/%1\$d\" class='nav' title=\"%1\$d\">%1\$d</a>");
#define('SEARCH_RESULTS_LINK_CAN',"<a href=\"javascript:setFrames('can','can/%1\$d');\" class='nav' title='%1\$d'>%1\$d</a>");
/// formatting string controlling the HTML output of links to west records
define('SEARCH_RESULTS_LINK_WEST', "<a href=\"west/%1\$d/%2\$d\" class='west' title='%3\$s %1\$d'>%3\$s %1\$d</a>");
define('SEARCH_RESULTS_LINK_WEST_CENTURY', "<a href=\"west/%1\$d\" class='west' title='%1\$d'>%1\$d</a>");
#define('SEARCH_RESULTS_LINK_WEST',"<a href=\"javascript:setFrames('west','west/%1\$d/%2\$d');\" class='west' title='%3\$s %1\$d'>%3\$s %1\$d</a>");
#define('SEARCH_RESULTS_LINK_WEST_CENTURY',"<a href=\"javascript:setFrames('west','west/%1\$d/');\" class='west' title='%1\$d'>%1\$d</a>");
/// formatting string controlling the HTML output(print style) of links to canadian records
// <a href="javascript:setFrames('cdn','cdn.htm')" class="nav" title="1943">1943</a>
define('SEARCH_RESULTS_LINK_PRINT_CAN', "%1\$d (Canadian)");

// <a href="javascript:setFrames('west','west.htm')" class="west" title="August 1921">August 1921</a>
/// formatting string controlling the HTML output(print style) of links to west records
define('SEARCH_RESULTS_LINK_PRINT_WEST', "%2\$s %1\$d (UK, US, Irish)");
define('SEARCH_RESULTS_LINK_PRINT_WEST_CENTURY', "%1\$d (UK, US, Irish)");

define('TAG_OPENING_LENGTH', 22);
define('TAG_CLOSING_LENGTH', 9);

/*
  classes defined:
  SearchResultsRecord extends BaseRecord(class.record.php)
  SearchUtility
 */

//
// class definitions

class SearchRecordFactory extends RecordFactory {

  function createRecordCan($rowRecordObj) {
    $this->record = new SearchResultsRecordCan($rowRecordObj);
    return $this->record;
  }

  function createRecordWest($rowRecordObj) {
    $this->record = new SearchResultsRecordWest($rowRecordObj);
    return $this->record;
  }

}

/**
 * search results from the canadian timeline
 */
class SearchResultsRecordCan extends SearchResultsRecord {

  function SearchResultsRecordCan($dbObj) {
    $dbObj->monthD = 1;
    traceVar('SearchResultsRecordCan methods', get_class_methods(get_class($this)));
    SearchResultsRecord::SearchResultsRecord($dbObj);
    unset($this->monthD);
    unset($this->month);
  }

  function returnDate() {
    return sprintf(SEARCH_RESULTS_LINK_CAN, $this->year);
  }

  function returnDateLinkPrint() {
    return sprintf(SEARCH_RESULTS_LINK_PRINT_CAN, $this->year);
  }

}

/**
 * search results from the west timeline
 */
class SearchResultsRecordWest extends SearchResultsRecord {

  function returnDate() {
    if ($this->monthD == 0) {
      return sprintf(SEARCH_RESULTS_LINK_WEST_CENTURY, $this->year);
    }
    else {
      return sprintf(SEARCH_RESULTS_LINK_WEST, $this->year, $this->monthD, $this->month);
    }
  }

  function returnDateLinkPrint() {
    if ($this->monthD == 0) {
      return sprintf(SEARCH_RESULTS_LINK_PRINT_WEST_CENTURY, $this->year);
    }
    else {
      return sprintf(SEARCH_RESULTS_LINK_PRINT_WEST, $this->year, $this->month);
    }
  }

}

/**
 * Foundation class for search results
 */
class SearchResultsRecord extends Record {

  var $body;
  var $lines = array();
  var $linesTagged = array();
  var $language;

  /**
   * SearchResultsRecord constructor
   * @param $recordObj a dataset returned by mysql_fetch_object.
   */
  function SearchResultsRecord($recordObj) {
    $this->setRecordID($recordObj->recordID);
    $this->setYear($recordObj->year);
    $this->setMonthD($recordObj->monthD);
    $this->setBody($recordObj->body);
    unset($this->decade);
    unset($this->lang);
    unset($this->timeline_table);
    unset($this->recordDate);
    unset($this->language);
  }

  function returnDate() {
    trigger_error('calling abstract function');
  }

  function returnDateLinkPrint() {
    trigger_error('calling abstract function');
  }

  /**
   * process body into lines 
   * @@@TBD
   *
   * breaks strings that has nested strong tags
   */
  function sortTagged() {
    $offset = 0;

    $subsequentOpenPos = strpos($this->body, '<strong', $offset); // find first begining
    $subsequentEndPos = strpos($this->body, '</strong>', $subsequentOpenPos);

    $last = 0;

    //traceVar('lines-before',$this->lines);
    $this->lines = array();
    $this->linesTagged = array();

    // if there are no tags at all
    if ($subsequentOpenPos === false) {
      trace('no strong tags found.');
      $this->lines[0] = $this->body;
      $this->linesTagged[0] = FALSE;
      return true;
    }

    while ($subsequentOpenPos !== false) {
      $subsequentEndPos = strpos($this->body, '</strong>', $subsequentOpenPos);
      if ($subsequentEndPos !== false) {
        $tmp = substr($this->body, $last, $subsequentOpenPos - $last);
        if ($tmp) {
          $this->lines[] = $tmp;
          $this->linesTagged[] = FALSE;
        }
        $tmp = substr($this->body, $subsequentOpenPos, $subsequentEndPos - $subsequentOpenPos + 9);
        if ($tmp) {
          $this->lines[] = $tmp;
          $this->linesTagged[] = TRUE;
        }
      }
      else {
        //echo('<br>not closed!');
      }
      $offset = $subsequentOpenPos + 1;
      $last = $subsequentEndPos + 9;
      $subsequentOpenPos = strpos($this->body, '<strong', $subsequentEndPos); // find subsequent beginings
    }

    $entireLength = strlen($this->body);

    if ($subsequentEndPos < $entireLength) {
      $this->lines[] = substr($this->body, $subsequentEndPos + 9, $entireLength - $subsequentEndPos);
      $this->linesTagged[] = FALSE;
    }
    traceVar('lines', $this->lines);
  }

  /**
   * Remove the inermost sets of nested strong tags, leaving no nested tags.
   *
   * Approach B
   * 1. find opening tag --> a
   * 2. find matching closing tag --> b
   * 		a. start with offset = a
   * 		b. scan foward after offset for next opening --> c
   * 		c. scan foward after offset for next closing --> d
   * 		d. if c > d, d is pair to a, return d
   * 		e. else set is nested, set offset = c
   * 		f. return to 2.b
   * 3. strip_tags from inside a & b
   * 4. find next opening tag or quit
   */
  function removeNestedTags($input) {

    $offset = 0;

    $output = $input;

    while (strpos($output, '<strong', $offset) !== false) { // for each open tag
      $a = strpos($output, '<strong', $offset);

      # finds its partner

      $nested_count = 0; // reset for each open pair

      $start = $a + 1;

      $next_open = strpos($output, '<strong', $start); // next opening 
      # if no more open tags after this one, grab the line length
      if ($next_open === false) {
        $next_open = strlen($output);
        $offset = $next_open;
      }

      while ($start < strlen($output)) { # find the pair
        $next_close = strpos($output, '</strong', $start);
        if ($next_close === false) {
          # can't do much if there's not matching close tag.
          $output = $output . '</strong>'; //@@@ warn about altering content!
          $next_close = strlen($output) - TAG_CLOSING_LENGTH;
        }

        $next_open = strpos($output, '<strong', $start); // next opening 
        # what if no other tag is found?
        if ($next_open === false) {
          $next_open = strlen($output);
        }

        # is the next tag open or close
        # ie. is the next open before the next close?
        if ($next_open < $next_close) {
          $nested_count += 1;       # the next tag is an open
          $start = $next_open + TAG_CLOSING_LENGTH; # adjust the offset
        }
        else {# the next tag is a close				
          if ($nested_count == 0) { # is this the right close tag?
            # found the partner
            # clean up the content
            $before = substr($output, 0, $a + TAG_OPENING_LENGTH);
            #trace('<br> before: '. htmlspecialchars($before)) ;

            $between = substr($output, $a, $next_close - $a);
            #trace('<br> between: '. htmlspecialchars($between)) ;

            $after = substr($output, $next_close);
            #trace('<br> after: ' . htmlspecialchars($after)) ;

            $cleaned_between = strip_tags($between);
            $output = $before . $cleaned_between . $after;

            $offset = strlen($before) + strlen($cleaned_between) + TAG_CLOSING_LENGTH;
            continue 2;
          }
          else { # no the right tag, but one step closer
            $nested_count = $nested_count - 1;
            $start = $next_close + TAG_CLOSING_LENGTH;
          }
        } // open or close tag
      } // finding the pair
    } // each open tag
    traceVar('removeNestedTags-ed', $output);
    return $output;
  }

  /**
   * glue $this->lines back together.
   */
  function rebuildBody() {
    $this->body = implode('', $this->lines);
  }

  function filter() {
    $args = func_get_args();
    $filterFunction = array_shift($args); // get the function name
    // first check if fnc is callable
    /*
      if(!function_exists($filterFunction)) {
      // raise error about uncallable function
      trigger_error('given function cannot be called.'.$filterFunction);
      die();
      }
     */
    if ((!empty($args)) && (!is_array($args))) {
      // paramaters sent in bad form.
      trigger_error('parameters sent in bad form. Need to be given as an array');
      die();
    }

    $this->sortTagged();
    #traceVar('lines',$this->lines);

    foreach ($this->lines as $key => $stringChunk) {
      trace('calling filterFunction');
      if ($this->linesTagged[$key] == FALSE) {
        $parameters = $args;
        array_unshift($parameters, $stringChunk);

        //$parameters = $parameters + $additionalParameters;
        // traceVar('parameters',$parameters);
        $temp = call_user_func_array($filterFunction, $parameters);
        if ($temp != false) {
          $this->lines[$key] = $temp;
        }
        else {
          // trigger_error('filter function returned false'); // !!
        }
      }
    }
    $this->rebuildBody();
    traceVar('body-after rebuildBody', $this->body);
    $this->sortTagged();
    traceVar('lines sortTagged', $this->lines);
  }

  function trimBody($limit = QUOTE_LENGTH, $seperator = '...', $wordPadding = 7) {
    global $format_prof;

    // does trimming really need to be done?
    if (strlen(strip_tags($this->body)) < $limit) {
      trace('no need to trim.');
      $this->body = comment('short') . $this->body;
      return true;
    }
    // find out how many(and which) terms are present in the body copy

    $format_prof->startTimer('SearchResultsRecord.trimBody.findingTerms');
    $found = array(); // flag -- has this snippet been found/used yet?
    for ($i = 1; $i <= 3; $i++) {
      if (strpos($this->body, 'class="term' . $i . '">') !== false) {
        $found[$i] = true;
      }
    }
    $format_prof->stopTimer('SearchResultsRecord.trimBody.findingTerms');

    if (sizeof($found) < 1) {
      trace('no terms found. trim at will.');
      // trim to limit;
      $this->body = substr($this->body, 0, $limit);
      // find last space
      $lastSpace = strrpos($this->body, ' ');
      $this->body = comment('no terms') . substr($this->body, 0, $lastSpace) . $seperator;
      return true;
    }

    // if only 1 term once
    $format_prof->startTimer('SearchResultsRecord.trimBody.onlyOneTest');
    $onlyOne = (bool) (substr_count($this->body, '<strong') == 1);
    $format_prof->stopTimer('SearchResultsRecord.trimBody.onlyOneTest');
    if ($onlyOne) {
      // only one instance of any term is found.. work around that.
      // find begining of <strong> tag
      $start = strpos($this->body, '<strong');
      $leftHalf = floor($limit / 2);
      $spare = 0;

      // check left
      if ($start - $leftHalf >= 0) { // can't start before first letter!
        $clipStart = $start - $leftHalf;
      }
      else {
        // if not, carry extra ($spare)
        $clipStart = 0;
        $spare = $leftHalf - $start;
      }

      // check right
      $endOfString = strlen($this->body);
      $rightHalf = floor($limit / 2);
      if (($start + $rightHalf + $spare) <= $endOfString) {
        // if fine..
        //  use as much of extra as possible
        $clipEnd = $start + $rightHalf + $spare;
      }
      else {
        // else carry extra ($extra) and..
        //  use as much of extra on left.
        $clipEnd = $endOfString;
        $spare = $endOfString - $start;
        // check left -- again -- maybe we can use the leftovers.
        if (($start - $leftHalf - $spare) >= 0) {
          $clipStart = $start - $leftHalf - $spare;
        } // if not, don't worry about it.
      }

      // cut to size
      $clipLength = $clipEnd - $clipStart;
      $this->body = substr($this->body, $clipStart, $clipLength);

      // if cut at begining, prepend with '...'
      if ($clipStart > 0) {
        $firstSpace = strpos($this->body, ' ');
        $this->body = $seperator . trim(substr($this->body, $firstSpace));
      }

      // if cut at end, append with '...'
      if ($clipEnd < $endOfString) {
        $lastSpace = strrpos($this->body, ' ');
        $this->body = substr($this->body, 0, $lastSpace) . $seperator;
      }
      $this->body = comment('only 1 term, only once') . $this->body;
      Return true;
    }

    // more than 1 term

    $matches = array(); // will hold all the matches from the following regexp

    $eachSide = floor($limit / 6);
    /*
      #        $format_prof->startTimer('SearchResultsRecord.trimBody.preg_match_all.anyChar');
      #        preg_match_all('/ (\S|\s){0,40}<strong class="term[1-3]">([^<]+)<\/strong>(\s|\S){0,40} /', $this->body ,$matches);
      #        $format_prof->stopTimer('SearchResultsRecord.trimBody.preg_match_all.anyChar');
     */

    $this->body = comment(sizeof($found) . ' term') . $this->body;
    // this one is 2nd quickest (and doesn't require a space at the begining)
    $format_prof->startTimer('SearchResultsRecord.trimBody.preg_match_all.spaceORlineEnds');
    preg_match_all('/( |\A)[^>]{0,' . $eachSide . '}<strong class="term[1-3]">([^<]+)<\/strong>[^<]{0,' . $eachSide . '}( |\Z)/', $this->body, $matches);
    $format_prof->stopTimer('SearchResultsRecord.trimBody.preg_match_all.spaceORlineEnds');

    /*
      #        $format_prof->startTimer('SearchResultsRecord.trimBody.preg_match_all.spaceLineSetEnds');
      #        preg_match_all('/ |\A[^>]{0,40}<strong class="term[1-3]">([^<]+)<\/strong>[^<]{0,40} |\Z/', $this->body ,$matches);
      #        $format_prof->stopTimer('SearchResultsRecord.trimBody.preg_match_all.spaceLineSetEnds');
      #
      #        // this one is quickest (sometimes)
      #        $format_prof->startTimer('SearchResultsRecord.trimBody.preg_match_all.spaceEnds');
      #        preg_match_all('/ [^>]{0,40}<strong class="term[1-3]">([^<]+)<\/strong>[^<]{0,40} /', $this->body ,$matches);
      #        $format_prof->stopTimer('SearchResultsRecord.trimBody.preg_match_all.spaceEnds');
      #
      #        // this one is BY FAR THE SLOWEST
      #        $format_prof->startTimer('SearchResultsRecord.trimBody.preg_match_all1');
      #        preg_match_all('/(\S+\s*){0,'.$wordPadding .'}<strong class="term[1-3]">([^<]+)<\/strong>(\s*\S+){0,'.$wordPadding .'}/', $this->body ,$matches);
      #        $format_prof->stopTimer('SearchResultsRecord.trimBody.preg_match_all1');
     */

    trace('<hr>');
    traceVar('this->body', $this->body);
    trace('<br>');
    traceVar('matches', $matches);

    $final = array(); // this will hold our snippets

    $format_prof->startTimer('SearchResultsRecord.trimBody.foreachMatch');
    while ($next = array_pop($matches[0])) {
      // check to see which term it holds
      foreach ($found as $termNumber => $value) {
        trace("checking term $termNumber");
        // if term X is found
        if (strpos($next, 'class="term' . $termNumber . '">') !== false) {
          trace("using term $termNumber here.");
          $final[] = trim($next);
          // flag term X as being 'done'   
          unset($found[$termNumber]);
          // don't keep going, instead, grab the next line
          continue 2;
        }
      }

      // if each term is now represented, just fill up untill length limit.
      if (empty($found)) {
        // check that this one won't put us over the top
        if (strlen(strip_tags((implode($seperator, $final)))) + strlen($next) < $limit) {
          $final[] = trim($next);
        }
        else {
          trace('too long, all done.');
          //break;
        }
      }
    }
    $format_prof->stopTimer('SearchResultsRecord.trimBody.foreachMatch');

    //$this->body = $seperator.implode($seperator, $final).$seperator . '<br><strong>original:</strong>'.$this->body;
    $this->body = $seperator . implode($seperator, $final) . $seperator;
    Return true;
  }

  /// build a big array of chunks -- words or sets of words in a tag.
  function returnChunks($start, $end = FALSE) {
    traceVar('start', $start);
    traceVar('end', $end);
    if (!$end) {
      // no end defined, so we default to the whole string.
      $end = strlen($this->body);
      $lineKeyEnd = sizeof($this->lines) - 1;
    }
    else {
      // find last 'line'
      $charCount = 0;
      //$lineKeyEnd = sizeof($this->lines)-1;
      $lineKeyEnd = 0;
      while (($charCount + strlen($this->lines[$lineKeyEnd])) < ($end)) {
        $charCount = $charCount + strlen($this->lines[$lineKeyEnd]);
        $lineKeyEnd++;
      }
    }
    traceVar('lineKeyEnd', $lineKeyEnd);

    $bigArray = array();

    // find first 'line'
    $charCount = 0;
    $lineKeyStart = 0;
    while (($charCount + strlen(strip_tags($this->lines[$lineKeyStart]))) < $start) {
      $charCount = $charCount + strlen(strip_tags($this->lines[$lineKeyStart]));
      $lineKeyStart++;
    }
    traceVar('lineKeyStart', $lineKeyStart);
    // first line is $lineKeyStart...
    // now find the begining in that
    //  if not a tag, then split it and add to bigArray
    //$lineKeyLength = sizeof($this->lines)-$lineKeyStart;
    $lineKeyLength = $lineKeyEnd - $lineKeyStart;
    $reducedLines = array_slice($this->lines, $lineKeyStart, $lineKeyLength);

    // grab a line,
    foreach ($reducedLines as $key => $oneLine) {

      // check to see if its a tag (editable?) // !! search for '<strong' instead
      if (strstr($oneLine, '<strong') !== FALSE) {
        // if so tack whole thing onto bigarray
        $bigArray[] = $oneLine;
      }
      else {
        // not, then explode on spaces
        //$oneLine = trim($oneLine);
        //$oneLine = rtrim($oneLine);

        if (!empty($oneLine)) {
          $tack = explode(' ', $oneLine);
          foreach ($tack as $key => $value) {
            if ($key != 0) {
              $tack[$key] = ' ' . $value;
            }
          }
          // tack results onto bigarray
          $bigArray = array_merge($bigArray, $tack);
        }
      }
    }
    return $bigArray;
  }

  function myPregReplace($text, $pattern, $replacement) {
    //print_r(func_get_args());
    traceVar('pattern', $pattern);
    traceVar('replacement', $replacement);
    return preg_replace($pattern, $replacement, $text);
  }

  function myStr_replace($text, $pattern, $replacement) {
    Return str_replace($pattern, $replacement, $text);
  }

  /**
   * Format the body of this record, highlighting particular terms.
   * 
   * @param array $searchTermIDs List of search term IDs
   */
  function formatBody() {
    global $format_prof;
    // FIXME move $searchTermIDs onto a property of the object so it 
    // doesn't need to be passed to `formatBody`.
    $searchTermIDs = func_get_arg(0);

    $format_prof->startTimer('Record::formatBody');
    parent::formatBody();
    $format_prof->stopTimer('Record::formatBody');


    $format_prof->startTimer('SearchResultsRecord.convertAnchors');
    $this->body = strip_tags($this->body, '<a>'); // stip all but anchor tags
    $this->convertAnchors($searchTermIDs); // convert matching anchors to <strong> tags
    $this->body = strip_tags($this->body, '<strong>'); // stip remaining anchor tags
    $format_prof->stopTimer('SearchResultsRecord.convertAnchors');
    $this->body = $this->removeNestedTags($this->body);
    traceVar('body-after convertAnchors', $this->body);

    $this->sortTagged();

    $format_prof->startTimer('SearchResultsRecord.convertTermNames');
    //print_r($this->lines);
    $this->convertTermNames($searchTermIDs); // hightlight text matching term names with <strong> tags
    $this->body = $this->removeNestedTags($this->body);
    $format_prof->stopTimer('SearchResultsRecord.convertTermNames');


    $format_prof->startTimer('SearchResultsRecord.convertUserInput');
    $this->convertUserInput($searchTermIDs); // hightlight text matching user input with <strong> tags
    $format_prof->stopTimer('SearchResultsRecord.convertUserInput');


    /* print_r ($this->body);
      echo('<hr>'); */

    unset($this->lines);
    unset($this->linesTagged);

    $format_prof->startTimer('SearchResultsRecord.trimBody');
    $this->trimBody();
    $format_prof->stopTimer('SearchResultsRecord.trimBody'); /*       */
  }

  /*
    // slower but case-insensitive version
    #    function convertTermNames($queryDetails){
    #        if(empty($queryDetails)) return;
    #
    #        // matching term names from DB
    #        foreach($queryDetails as $key => $part){
    #            $termNameMaches = array();
    #            foreach($part['terms'] as $termID => $termName){
    #                // match the names of the terms
    #                $termNameMaches[] =     '('.preg_addslashes($termName).')';
    #            }
    #            $pattern ='/'.implode('|',$termNameMaches).'/siU';
    #            $replacement = '<strong class="'.'term'.$key.'">\1</strong>';
    #            traceVar('term names pattern', $pattern);
    #            //traceVar('body, before', $this->body);
    #            $this->body = preg_replace($pattern, $replacement, $this->body);
    #        }
    #    }
   */

  /**
   * Wrap term names in <strong> tags
   *
   * - don't highlight query parts bound by 'NOT' operator
   *
   * KNOWN ISSUE (!!)
   * convertTermNames IS case-sensitive. (a limit of PHP4)
   * the version above is case-insensitive, but is also 10-20 times slower.
   */
  function convertTermNames($queryDetails) {
    if (empty($queryDetails))
      return;
    if (!is_array($queryDetails)) {
      trigger_error('wrong data type. expecting an array');
      Return false;
    }

    // matching term names from DB
    foreach ($queryDetails as $key => $part) {
      if (isset($part['operator']) && ($part['operator'] == 'AND NOT')) {
        continue; // don't highlight NOT terms
      }
      $replacement = array();
      $pattern = array();
      if ((isset($part['terms'])) && is_array($part['terms'])) { // don't highliht NOT terms
        foreach ($part['terms'] as $termName) {
          // match the names of the terms
          // deal with any regex special characters, like . or /
          $regexsafe_termName = preg_quote($termName, '/');
          // deal with accents
          $pattern[] = '/' . SearchUtility::translateAccentsRegexp($regexsafe_termName) . '/';
          # $pattern[] = $termName;
          $replacement[] = '<strong class="' . 'term' . $key . '">' . $termName . '</strong>';
        }
        traceVar('term names pattern', $pattern);
        traceVar('term names replacement', $replacement);
        //traceVar('body, before', $this->body);
        //$this->body =  str_replace($pattern, $replacement, $this->body);		
        if ((!empty($replacement)) && (!empty($pattern))) {
          #$this->filter(array(&$this, 'myStr_replace'),$pattern, $replacement); // convert matching anchors to <strong> tags
          $this->filter(array(&$this, 'myPregReplace'), $pattern, $replacement); // convert matching anchors to <strong> tags using regular exp (for accents)
        }
      }
    }
  }

  function convertUserInput($queryDetails) {
    if (empty($queryDetails))
      return;
    if (!is_array($queryDetails)) {
      trigger_error('wrong data type. expecting an array');
      Return false;
    }

    // matching user input
    $pattern = array();
    $replacement = array();
    foreach ($queryDetails as $key => $part) {
      if (isset($part['operator']) && ($part['operator'] == 'AND NOT')) {
        continue; // don't highlight NOT terms
      }
      $className = 'term' . $key;
      // the actual query word
      //$pattern[] =     '/\b('.SearchUtility::translateWildCardsRegexp($part['string']).')\b/i'; 
      $accentSafe = SearchUtility::translateAccentsRegexp($part['string']);
      $pattern[] = '/(' . SearchUtility::translateWildCardsRegexp($accentSafe) . ')/i';
      $replacement[] = '<strong class="' . $className . '">\1</strong>';
    }
    traceVar('user input pattern', $pattern);
    //traceVar('body, before', $this->body);
    //$this->body = preg_replace($pattern, $replacement, $this->body);		    
    if ((!empty($replacement)) && (!empty($pattern))) {
      $this->filter(array(&$this, 'myPregReplace'), $pattern, $replacement); // convert matching anchors to <strong> tags        	
    }
  }

  /**
   * Convert the coded-in term link to a strong tag.
   * 
   * Cycles through the terms already found (those listed in $queryDetails). For 
   * each part of the query, replace the links (<a href="term://...) with a pair
   * of <strong> tags.
   *
   * @param $queryDetails (array) The query details array (containing a list of 'terms' keyed by ID)
   *  eg.
   *   Array ( [1] => Array(
   * 	 [string] => sun
   * 	 [type] => ANY
   * 	 [terms] => Array (
   *  		[1500] => While the Sun Shines
   * 			[1528] => Candles to the Sun
   * 			[1769] => A Yard of Sun
   * 			[1834] => New York World-Telegram and Sun
   * 			[1928] => A Life in the Sun
   * 			[2407] => Sun
   * 			)...
   */
  function convertAnchors($queryDetails) {
    if (!empty($queryDetails)) {
      if (!is_array($queryDetails)) {
        trigger_error('wrong data type. expecting an array');
        Return FALSE;
      }

      # skip this whole excercise if there are no links in the body
      if (strpos($this->body, '<a href="term:') === false) {
        trace('no anchor tags in body copy.');
        Return TRUE;
      }
      #traceVar('queryDetails', $queryDetails);
      # build the search and replace patterns
      $pattern = array();
      $replacement = array();
      foreach ($queryDetails as $key => $part) {
        $className = 'term' . $key;
        # don't attempt to find anchor tags for full-text searches
        if ($part['type'] != 'TXT') {
          foreach ($part['terms'] as $termID => $termName) {

            # only go through with the search if
            # an anchor tag exists for this term
            if (strpos($this->body, '<a href="term://' . $termID) === false) {
              trace("term://$termID not found, don't bother with regex");
            }
            else {
              # match where anchors have been marked up.
              $pattern[] = '/<a href="term:\/\/' . $termID . '\/">(.*)<\/a>/siU';
              $replacement[] = '<strong class="' . $className . '">\1</strong>';
            }
          }
        }
      }

      if (empty($replacement)) {
        trace('no terms at all to replace');
        Return TRUE;
      }
      else {
        traceVar('anchor pattern', $pattern);
        traceVar('anchor replacement', $replacement);
      }

      # search and replace
      //$this->body = preg_replace($pattern, $replacement, $this->body);
      traceVar('body, before', $this->body);
      //$this->filter(array(&$this, 'myPregReplace'),$pattern, $replacement); // convert matching anchors to <strong> tags        	
      $this->body = preg_replace($pattern, $replacement, $this->body);
      traceVar('body, after', $this->body);

      Return TRUE;
    }
    else {
      Return FALSE;
      ;
    }
  }

  function removeTermAnchors() {
    $this->body = preg_replace('/<a href="term:\/\/\d+\/">(.*)<\/a>/siU', '\1', $this->body);
  }

}

class SearchUtility {

  static function mergeLists($canList, $westList, $max_size = 10000, $order = 'ASC') {

    // sort the two array into one.
    $recordList = array();
    while ((count($canList) || (isset($currentCan))) && (count($westList) || (isset($currentWest)))) {
      // if the cue is empty, grab the next one
      if ((!isset($currentCan)) || (!empty($canList))) {
        $currentCan = array_shift($canList);
      }
      if ((!isset($currentWest)) || (!empty($westList))) {
        $currentWest = array_shift($westList);
      }
      if ($order == 'ASC') {
        if ($currentCan->year <= $currentWest->year) {
          $recordList[] = $currentCan;
          unset($currentCan);
        }
        else {
          $recordList[] = $currentWest;
          unset($currentWest);
        }
      }
      else {
        if ($currentCan->year >= $currentWest->year) {
          $recordList[] = $currentCan;
          unset($currentCan);
        }
        else {
          $recordList[] = $currentWest;
          unset($currentWest);
        }
      }
    }
    if (isset($currentWest)) {
      $recordList[] = $currentWest;
    }
    if (isset($currentCan)) {
      $recordList[] = $currentCan;
    }

    // tack on any left overs
    if (isset($canList)) {
      $recordList = array_merge($recordList, $canList);
    }
    if (isset($westList)) {
      $recordList = array_merge($recordList, $westList);
    }

    // trim down to max_size if to large
    if (sizeof($recordList) > $max_size) {
      trace('trim record list down');
      $recordList = array_slice($recordList, 0, $max_size);
    }

    return $recordList;
  }

  /**
   *  ? = exactly one character (including space)
   *  * = any series of characters (or none)
   */
  static function translateWildCardsSQL($string) {
    $wildcardsIn = array('?', '*'); // used for open ended searches
    $wildcardsSQL = array('_', '%'); // used by MySQL
    return str_replace($wildcardsIn, $wildcardsSQL, $string); // switch-a-roo!		
  }

  /**
   * replace 'fancy' quotes with regular ones
   * �� - single quotes (U+2018 + U+2019) ==> '
   * �� - double quotes (U+201C + U+201D) ==> "
   */
  static function translateQuotes($string) {
    $funnyQuotesIn = array('�', '�', '�', '�'); // fancy quotes
    $regQuotesOut = array("'", "'", '"', '"'); // used by MySQL
    return str_replace($funnyQuotesIn, $regQuotesOut, $string); // switch-a-roo!		
  }

  /**
   * take in non-escaped text string
   * @return (string) An escaped string that has wildcards translated into regex equiv.
   * 	-  .? = exactly one character (including space)
   * 	-  .* = any series of characters (or none)
   */
  static function translateWildCardsRegexp($string) {
    // $string = quotemeta($string); // was adding an extra slash to regexes eg.  ... REGEXP "[[:<:]]O'N\[a-zA-Z0-9]*[[:>:]]"
    $string = addslashes($string);
    $wildcardsIn = array('?', '*');
    $wildcardsRegexp = array('[a-zA-Z0-9]?', '[a-zA-Z0-9]*');
    // what about word end and word begin?
    //  can't be done here because word boudaries are done differently in 
    //  mySql vs. PHP (and this function is used for both)
    return str_replace($wildcardsIn, $wildcardsRegexp, $string); // switch-a-roo!		
  }

  /**
   * substitute any accentable character with a regexp set of potential accents
   * eg. a ==> [a������]
   */
  static function translateAccentsRegexp($string) {
    $accentsIn = array('/[a������������]/',
      '/[e��������]/',
      '/[i��������]/',
      '/[o������������]/',
      '/[u��������]/',
      '/[y���]/',
      '/[n��]/',
      '/[c��]/'
    );
    $accentsRegexp = array('[a������������]',
      '[e��������]',
      '[i��������]',
      '[o������������]',
      '[u��������]',
      '[y���]',
      '[n��]',
      '[c��]'
    );
    return str_replace($accentsIn, $accentsRegexp, $string); // switch-a-roo!		
    //return  preg_replace($accentsIn, $accentsRegexp, $string);
  }

  static function stripNonAlpha($text) {
    // place each character in a [] -- that way its literal
    $safe = preg_replace("(/[^a-zA-Z0-9\*\?]|'.|%u2019.)/", " ", $text); // 
    return $safe;
  }

}

//
// functions be here
function buildWordMatchQuery($wordMatchString, $termType) {
  if (empty($wordMatchString)) {
    trigger_error('unable to complete search. Nothing to match against.', WARNING);
    Return false;
  }
  $termType = strtoupper($termType);
  if (($termType == 'ANY')OR ( empty($termType))) {
    // don't descriminate based on type
    $andTypeClause = '';
  }
  elseif (($termType == 'GEO')) {
    // do discriminate based on type
    $andTypeClause = "AND (termsType = 'LG' OR termsType='LC')";
  }
  else {
    // do discriminate based on type
    $andTypeClause = "AND termsType = '$termType'";
  }
  traceVar('stringIn', $wordMatchString);
  $quote_safe = SearchUtility::translateQuotes($wordMatchString);
  $accent_variable = SearchUtility::translateAccentsRegexp($quote_safe); // 
  $literal_text = SearchUtility::translateWildCardsRegexp($accent_variable); // replaces ? and *
  traceVar('stringTranslated', $literal_text);
  $searchItems = getbits($literal_text);
  // for each word in the list, add another WHERE clause
  //$wordMatchSelect = 'SELECT termsID,termsName,termsType';
  $wordMatchSelect = 'SELECT termsID, termsName ';
  $wordMatchSelect .= " FROM " . TERMS_TABLE . ' WHERE  ';

  // start with empty list
  $whereClause = array();
  $regexes = array();
  $counter = 1;
  foreach ($searchItems as $oneBite) {
    $placeholder = "regex$counter";
    $regex = "[[:<:]]{$oneBite}[[:>:]]";
    $regexes[$placeholder] = $regex;
    $whereClause[] = "termsName REGEXP (:$placeholder)";
    $counter++;
  }
  if (!empty($whereClause)) {
    $wordMatchWhere = implode(' AND ', $whereClause);
  }
  else {
    return false;
  }
  return array(
    'q' => $wordMatchSelect . $wordMatchWhere . $andTypeClause,
    'vals' => $regexes,
  );
//  return $wordMatchSelect . $wordMatchWhere . $andTypeClause;
}

function buildVerbatimQuery($wholeTermString) {
  if (empty($wholeTermString)) {
    trigger_error('unable to complete search. Nothing to match against.');
    Return false;
  }
  $verbatimSelect = 'SELECT termsID,termsName,termsType';
  $verbatimSelect .= " FROM " . TERMS_TABLE . ' WHERE ';
  //$verbatimSelect .= " FROM " . TERMS_TABLE . " LEFT JOIN " . INDEX_TABLE. " ON " . TERMS_TABLE . ".termsID = " . INDEX_TABLE. ".mapTermID ";
  #$literal_text = SearchUtility::stripNonAlpha($wholeTermString);
  $literal_text = db_escape_string($wholeTermString);
  $literal_text = SearchUtility::translateWildCardsSQL($literal_text);
  $verbatimWhere = " termsName LIKE '$literal_text' ";
  //$verbatimWhere .= " AND termsType = '$termsType'";
  return $verbatimSelect . $verbatimWhere;
}

function getbits($termName) {
  //$trash = array(' and ', ' it ',"'",',',' the ','"');
  $trash = array("'", ',', '"'); // drop single quotes, commas and double quotes

  /*
   * ?? what about apostrophes in names?
   *                      
   * Well, the query:
   *  SELECT ...WHERE termsName REGEXP "[[:<:]]o[[:>:]]" and termsName REGEXP "[[:<:]]neill[[:>:]]"
   * returns:
   *    termsID,termsName
   *    400,O'Neill, Eugene
   *    1006,O'Neill, James
   * Which gives the same results as if we queried:
   *  SELECT ...WHERE termsName REGEXP "[[:<:]]O'Neill[[:>:]]" 
   */

  /*
   * ?? what about case sensitive searches?
   *                     ^^^^^^^
   * By default string comparisons are NOT case sensitive.
   * To preform a case sensitive search, use the binary keyword before the column that is to be case sensitive
   *  eg. SELECT ... WHERE binary termsName like "%Ad%"
   */

  #$termName = str_replace($trash, ' ', $termName);
  // first break it up into seperate 'words'
  $termBits = explode(" ", $termName);
  // get rid of any extra spaces left over.

  $c = count($termBits);
  //tracevar('termBits',$termBits);

  for ($i = 0; $i < $c; $i++) {
    // clean-up first.
    $trash = array(' and ', ' it ', "'", ',', ' the ', '"');
    #$termBits[$i] = str_replace($trash, ' ', $termBits[$i]);
    if (empty($termBits[$i]) || strlen($termBits[$i]) < 2)
      unset($termBits[$i]);
  }

  Return $termBits;
}

function returnIndexedCanRecords($termID, $limit) {
  if (empty($termID)) {
    return false;
  }
  $canQuery = "SELECT " . CAN_TABLE . ".recordDate as year, " . CAN_TABLE . ".recordID FROM " . CAN_TABLE . ", " . INDEX_TABLE . "  WHERE " . INDEX_TABLE . ".mapRecordID=" . CAN_TABLE . ".recordID  AND " . INDEX_TABLE . ".mapTermID = $termID ORDER BY " . CAN_TABLE . ".recordDate ASC LIMIT $limit";

  $canResult = db_query($canQuery, '', __FILE__, __LINE__);

  $foundInCanTable = mysqli_num_rows($canResult);

  if (!$foundInCanTable) {  // if we were't able to load the info, don't crash, but do register a complaint
    return false;
  }

  // build can results.
  if ($foundInCanTable > 0) {  // if there are any.
    $recordArrayCan = array();
    while ($recordObj = mysqli_fetch_object($canResult)) { // start pulling them off the DB
      $recordArrayCan[$recordObj->year][0] = $recordObj->recordID;
    }
  }
  else { // zero canadian records indexed to this terms.
    $recordArrayCan = false;
  }
  traceVar('recordArrayCan', $recordArrayCan);

  return $recordArrayCan;
}

function returnIndexedWestRecords($termID, $limit) {
  if (empty($termID)) {
    return false;
  }

  $westQuery = "SELECT MONTH(" . WEST_TABLE . ".recordDate) as month, YEAR(" . WEST_TABLE . ".recordDate) as year, " . WEST_TABLE . ".recordID FROM " . WEST_TABLE . ", " . INDEX_TABLE . " WHERE " . INDEX_TABLE . ".mapRecordID=" . WEST_TABLE . ".recordID AND " . INDEX_TABLE . ".mapTermID = $termID ORDER BY " . WEST_TABLE . ".recordDate ASC LIMIT $limit";
  $westResult = db_query($westQuery, '', __FILE__, __LINE__);

  $foundInWestTable = mysqli_num_rows($westResult);

  if (!$foundInWestTable) {  // if we were't able to load the info, don't crash, but do register a complaint
    exit_function();
    return false;
  }

  // build can results.
  if ($foundInWestTable > 0) {  // if there are any.
    $recordArrayWest = array();
    while ($recordObj = mysqli_fetch_object($westResult)) { // start pulling them off the DB
      $recordArrayWest[$recordObj->year][$recordObj->month] = $recordObj->recordID;
    }
  }
  else { // zero canadian records indexed to this terms.
    $recordArrayWest = false;
  }
  traceVar('recordArrayWest', $recordArrayWest);

  return $recordArrayWest;
}

?>