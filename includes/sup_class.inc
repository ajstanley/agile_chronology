<?php

/*
  Class supObj - the main object
  constructor takes supID and language
  properties
  an image object
  a linkList object
  methods
  display - regular display
  displayPrint - printable page
 */

/*
  both output methods make use of the $template variable to accomplish the different colour schemes
  display()
  - uses one template (sup_template.php)
  displayPrint()
  - uses one template (sup_print_template.php)
 */

/////////////////////////////////////////////////////////
//
//  S U P   G E N E R A L
//
/////////////////////////////////////////////////////////
/// Supplemental record
class supObj {

  var $sup_id;
  var $links;
  var $image;
  var $colour;
  var $language;
  var $error;
  // display variables
  var $linksCode;
  var $imageCode;
  var $title;
  var $body;
  var $subtitle;

  function supObj($supID, $lang = 'en') {
    $this->sup_id = $supID;
    $this->setLanguage($lang);
    if ($this->error) {
      trace('problem with language');
      Return false;
    }
    $this->getSupText();
    if ($this->error) {
      trace('problem loading text');
      Return false;
    }
    $imageObj = new SupImage($supID);
    if ($imageObj->imageExists()) {
      $this->image = $imageObj;
    }
    else {
      $this->image = false;
    }

    $this->links = new SupLinkList($this->sup_id, $this->language);

    Return true;
  }

  function setLanguage($newLang) {
    $newLang = substr($newLang, 0, 2);
    if ((strtolower($newLang) == 'en') || (strtolower($newLang) == 'fr')) {
      $this->language = strtolower($newLang);
    }
    else {
      $this->error[] = 'illegal value for language';
      Return false;
    }
  }

  function display($scheme = BOTH) {
    # grab the right template variables
    global $template;
    global $path;
    global $year;

    if ($scheme == CAN || $scheme == WEST) {
      $this->colour = $scheme;
    }

    $t = $template[$this->colour];
    traceVar('template variables', $t);

    $dateString = date('j M. Y');  // dd Mon. yyyy
    $thiPage = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];

    // get authorship data, replace citation1
    $authorshipData = getAuthorshipLink($this->sup_id, 'S');
    $authorship = $authorshipData['1']['text_en'];
    $sup_print_title = $this->title;

    #  a combination of color and language.
    if (!$this->error) { # if error free, go ahead and display.
      $this->linksCode = $this->links->returnHtml($this->colour);
      if ($this->image) {
        $this->imageCode = $this->image->returnImgTag();
      }
      else {
        $this->imageCode = '<!-- no image found -->';
      }
      $module_path = drupal_get_path('module', 'agile_chronology');
      include("$module_path/theme/tpl.sup.php");
    }
    else {
      echo("there's been a problem that means we're unable to display this page.<br>");
      echo(implode("\n", $this->error));
    }
  }

  function displayPrint() {
    // grab the right template variables
    global $template;
    global $path;
    $t = $template[$this->colour];
    traceVar('template variables', $t);

    $dateString = date('j M. Y');  // dd Mon. yyyy
    $thiPage = 'http://' . $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];

    // get authorship data, replace citation1
    $authorshipData = getAuthorshipLink($this->sup_id, 'S');
    $authorship = $authorshipData['1']['text_en'];
    $sup_print_title = $this->title;

    //  a combination of color and language.
    if (!$this->error) { // if error free, go ahead and display.
      traceVar('supObj', $this);
      include('./tpl.sup_print.php');
    }
    else {
      echo("there's been a problem that means we're unable to display this page.<br>");
      echo(implode("\n", $this->error));
    }
  }

  function getSupText() {
    // build query
    $LA = strtoupper($this->language);
    $getSupQuery = "SELECT sup_id, title$LA as title, body$LA as body, subtitle$LA as subtitle, colour from " . SUPLEMENTAL_TABLE . " WHERE sup_id='" . $this->sup_id . "'";

    // execute query
    // $getSupResults = db_query($getSupQuery, '', __FILE__, __LINE__);
    $query = db_select(SUPLEMENTAL_TABLE, 's');
    $query->addField('s', "title$LA", 'title');
    $query->addField('s', "body$LA", 'body');
    $query->addField('s', "subtitle$LA", 'subtitle');
    $supTextObj = $query
        ->fields('s', array('sup_id', 'colour'))
        ->condition('sup_id', $this->sup_id)
        ->execute()
        ->fetchObject();

    // if no results, return false

    if (empty($supTextObj)) {
      trace('no trace of that sup page');
      $this->error[] = 'no trace of that sup page';
      Return false;
    }
    else {
      $supTextObj->error = false;
      $this->sup_id = $supTextObj->sup_id;
      $this->colour = strtoupper($supTextObj->colour);
      $this->title = $supTextObj->title;
      $this->body = $supTextObj->body;
      if (isset($supTextObj->subtitle)) {
        $this->subtitle = $supTextObj->subtitle;
      }
      else {
        $this->subtitle = '';
      }
      return true;
    }
  }

  /// format the links in a sup record
  function format_body() {
    // update all the appropriate anchor tags
    $this->convertAnchors();
    // remove all the anchor tags that dont match
    $this->body = removeTermUrls($this->body);
  }

  /**
   *  replace term:// URIs with URIs that point to sup records
   *
   * @return (string)	updated copy of body text
   *
   * @param $body_text		(string)	body copy of record to be updated
   * @param $sup_objects	(array)	list of sup records to be linked
   */
  function convertAnchors() {
    // make a list of all terms listed in this item
    $linkedTermIDs = returnLinkedTerms($this->body);
    // find out which terms have sup records to be linked to
    $sup_objects = returnExistingSups($linkedTermIDs);

    $body_text = $this->body;

    if (empty($sup_objects)) {
      trace('no terms');
      return;
    };
    foreach ($sup_objects as $key => $supObj) {
      if (strstr($body_text, 'term://' . $supObj->termID)) {
        # build regexp patterns
        $pattern = '/<a href="term:\/\/' . $supObj->termID . '\/">/i';
        $supLink = returnSupLink($supObj->supID);
        $title = $supObj->linkTitle ? 'title="' . $supObj->linkTitle . '"' : '';
        $replacement = '<a href="' . $supLink . '" ' . $title . '>';

        //traceVar('pattern', $pattern);
        //traceVar('replacement', $replacement);

        $body_text = preg_replace($pattern, $replacement, $body_text);

        // if a link has been added, then mark this sup object as 'used'
        // register this sup ID as being displayed in the body 
        $this->links->registerSup($supObj->supID);
      }
    }
    $this->body = $body_text;
  }

}

function supExists($supID) { // returns true if a record exist with the given ID
  $supID = (int) $supID;
  $countSupQ = 'SELECT count(*) FROM ' . SUPLEMENTAL_TABLE . ' WHERE sup_id=' . $supID;
  $results = db_query_results($countSupQ, Q_TYPE_SELECT_SINGLE, __FILE__, __LINE__);

  Return $results;
}

/////////////////////////////////////////////////////////
//
//  S U P   I M A G E
//
/////////////////////////////////////////////////////////


function _addServerPath($imageName) {
  return null;
  Return SERVER_PATH_TO_IMAGES . '/supplemental/' . $imageName;
}

/**
 * image object for suplimental record 
 */
class SupImage {

  var $_supID; /* < int Supplemental record ID */
  var $_altText; /* < string Image alt text */
  var $_fileLocal; //< string absolute path to image on filesystem
  var $_fileUrl; //< string image URI
  var $_imageInfo; //< array Image data (as returned by GetImageSize())

  /**
   * image object constructor
   *
   * @param $supID (int) Suplemental record ID
   */

  function SupImage($supID) {
    $this->_supID = (int) $supID;
    // check filesys for image.
    // traceVar('image exists?', $this->imageExists()  );
    if ($this->imageExists()) {
      $this->getAlt();
      $this->_fileLocal = _addServerPath($this->_supID) . '.jpg';
      $this->_fileUrl = _addWebServerPath($this->_supID) . '.jpg';
      $this->_imageInfo = GetImageSize($this->_fileLocal);
      trace('returning Image Object');
      //return $this;
    }
    else {
      //trace('not returning anything');
      //return false;
    }
  }

  /**
   * Does the sup record have an image. Checks the file system for appropriate file
   *
   * @return boolean image exists
   */
  function imageExists() {
    trace(_addServerPath($this->_supID));
    $fileExists = file_exists(_addServerPath($this->_supID) . '.jpg');
    traceVar("{$this->_supID} exists", $fileExists ? 'TRUE' : 'FALSE');
    Return (bool) $fileExists;
  }

  /**
   * return the alt text
   *
   * @return string Alt text of image
   */
  function returnAlt() {
    Return $this->_altText;
  }

  /**
   * return a complete image tag
   *
   * @return string image tag. Ex: "<img src='blah.jpg' width='10' alt='boo!'> " 
   */
  function returnImgTag() {
    //<img src="imgs/can/09_1973.jpg" alt="Something descriptive" width="225" height="225" class="float_right" align="right" border="2" hspace="6" vspace="6" />
    return sprintf('<img src="%s" %s alt="%s" title="%s" align="right" border="2" hspace="6" vspace="6">', $this->_fileUrl, $this->_imageInfo[3], $this->_altText, $this->_altText);
    # Return "<img src='$this->_fileUrl' {$this->_imageInfo[3]} alt='$this->_altText' title='$this->_altText' align='right' border='2' hspace='6' vspace='6'>";
  }

  /// sets the Alt attribute. Fetches data from DB.
  function getAlt() {
    //  build query
    $supID = (int) $this->_supID;
    $getAltQ = 'SELECT imageAlt FROM ' . SUPLEMENTAL_IMAGE_ALT . "  WHERE supID = '$supID'";
    //  execute query
    $result = db_query($getAltQ, '', __FILE__, __LINE__);
    if (mysqli_num_rows($result) > 0) {
      $row = mysqli_fetch_array($result);
      $this->_altText = $row['imageAlt'];
    }
    else {
      $this->_altText = '';
    }
  }

}

// end of ImageObj class
/////////////////////////////////////////////////////////
//
//  S U P   L I N K S
//
/////////////////////////////////////////////////////////
/// Link from a supplemental record page
class SupLink {

  var $link_id;
  var $sup_id;
  var $text;
  var $title;
  var $url;
  var $typeID;
  var $typeIcon;
  var $typeName;

  /**
   * SupLink constructor
   *
   * @param $dbObj	stdObj one record object
   */
  function SupLink($dbObj) {
    $this->text = $dbObj->text;
    $this->title = $dbObj->title;
    $this->typeID = (int) $dbObj->type;
    $this->url = $dbObj->url;
    $this->getTypeInfo();
    if (isset($dbObj->sup_id)) {
      $this->sup_id = $dbObj->sup_id;
    }
    if (isset($dbObj->link_id)) {
      $this->link_id = $dbObj->link_id;
    }
  }

  /**
   * Return a link to this sup resource. 
   * Regular links (FTP, Web, Essay, etc.) return a javascript:updateOpener address. DM - REMOVED SO PAGE LOADS IN SAME WINDOW
   * Links to other supplemental pages return a relative link to sup.php with the supID bugged in.
   *
   * @return string link to this resource.
   */
  function returnHref() {
    switch ($this->typeID) {
      case 4: // supplemental pages
        Return 'sup.php?id=' . $this->url;
        break;
      default:
        Return $this->url;
        break;
    }
  }

  /// is there a link in the db with this ID?
  function linkExists($linkID) {

    $linkTable = SUPLEMENTAL_LINK_TABLE;
    $getSupLinksQ = "SELECT 1 FROM $linkTable WHERE link_id = $linkID";


    Return db_query_results($getSupLinksQ, Q_TYPE_SELECT_SINGLE, __FILE__, __LINE__);
  }

  function getTypeInfo() {
    global $linkTypes;
    if (!isset($linkTypes) || empty($linkTypes)) {
      $linkTypes = Util::returnLinkTypes();
    }
    if (isset($linkTypes[$this->typeID])) {
      $this->typeName = $linkTypes[$this->typeID]['name'];   // french names ??
      $this->typeIcon = $linkTypes[$this->typeID]['icon'];   // just the file name. Path must be added in the template.
    }
    else {
      $this->typeName = '';   // french unknown ??
      $this->typeIcon = 'default_icon.gif';  // ?
    }
  }

}

/// a collection of SupLink objects
class SupLinkList {

  var $linkList = array();
  var $lang;
  var $supID;
  var $indexedSupDisplayed = array();

  function SupLinkList($supID, $lang = 'EN') {
    $this->lang = strtoupper($lang);
    $this->supID = $supID;

    $this->getLinks();
    if (!empty($this->linkList)) {
      $this->sortLinks();
      # remove any links that show up in the body
    }
    else {
      unset($this->linkList);
    }
  }

  function returnHtml($scheme = BOTH) {
    // build the table that gets displayed and display it... or return the code?
    /* looks like
      #        <!-- begin nested links table -->
      #        <table border="0" width="100%" cellspacing="0" cellpadding="3" bgcolor="#eeeeee">
      #        <tr><td align="center" colspan="2"><img src="imgs/red_shim2.gif" alt="" width="530" height="3" border="0" vspace="2" /></td></tr>
      #        <tr>
      #        <td>
      #        <p style="text-align: left;">For more information explore one of the related topics below:</p>
      #        <!-- begin nested link icon table -->
      #        <table border="0" cellspacing="0" cellpadding="2" bgcolor="#eeeeee">
      #        <tr>
      #        <td width="20" align="center"><img src="imgs/audio_icon.gif" alt="audio link" width="15" height="15" border="0" /></td>
      #        <td><a href="javascript:updateOpener('http://www.theatrelibrary.org/links/TheatreGenres.html')" title="audio link: 1924 recordings of an early producer's recollections">Broadway: an oral history</a></td>
      #        </tr>
      #        <tr>
      #        <td width="20" align="center"><img src="imgs/video_icon.gif" alt="video link" width="15" height="15" border="0" /></td>
      #        <td><a href="javascript:updateOpener('http://www.kingarthursknights.com/')" title="video link: footage of the Broadway strip">Video footage of the Broadway strip</a></td>
      #        </tr>
      #        <tr>
      #        <td width="20" align="center"><img src="imgs/essay_icon.gif" alt="essay link" width="15" height="15" border="0" /></td>
      #        <td><a href="javascript:updateOpener('http://www.kingarthursknights.com/')" title="essay link: An Assessment of Mallory's influence on British theatre">Sir Thomas Mallory and the British Stage</a></td>
      #        </tr>
      #        <tr>
      #        <td width="20" align="center"><img src="imgs/www_icon.gif" alt="World Wide Web link" width="15" height="15" border="0" /></td>
      #        <td><a href="javascript:updateOpener('http://www.kingarthursknights.com/')" title="world wide web link: site devoted to the lore of King Arthur and the Knights of the Round Table">www.kingarthursknights.com</a></td>
      #        </tr>
      #        <tr>
      #        <td width="20" align="center"><img src="imgs/crc_icon.gif" alt="Chronology link" width="15" height="15" border="0" /></td>
      #        <td><a href="javascript:setFrames('west','west_shakespeare.htm')" title="short biography of Shakespeare at www.theatre.com">Shakespeare, William</a></td>
      #        </tr>
      #        <tr>
      #        <td width="20" align="center"><img src="imgs/crc_icon2.gif" alt="Chronology link" width="15" height="15" border="0" /></td>
      #        <td><a href="javascript:setFrames('west','west_shakespeare.htm')" title="short biography of Shakespeare at www.theatre.com">Shakespeare, William</a></td>
      #        </tr>
      #        <tr>
      #        <td width="20" align="center"><img src="imgs/crc_icon3.gif" alt="Chronology link" width="15" height="15" border="0" /></td>
      #        <td><a href="javascript:setFrames('west','west_shakespeare.htm')" title="short biography of Shakespeare at www.theatre.com">Shakespeare, William</a></td>
      #        </tr>
      #        </table>
      #        <!-- end nested icon link table -->
      #
      #        //////////////////////////////////////////////////
      #
      #        the types are:
      #            audio - audio_icon.gif
      #            video - video_icon.gif
      #            essay - essay_icon.gif
      #            www - www_icon.gif
      #            internal -  crc_icon.gif
      #            FPT - ftp_icon.gif
      #
      #        //////////////////////////////////////////////////
      #
      #        each record looks like:
      #            <tr>
      #                <td width="20" align="center">
      #                    <img src="imgs/video_icon.gif" alt="video link" width="15" height="15" border="0" />
      #                </td>
      #                <td>
      #                    <a href="javascript:updateOpener('http://www.kingarthursknights.com/')" title="video link: footage of the Broadway strip">
      #                        Video footage of the Broadway strip
      #                    </a>
      #                </td>
      #            </tr>
      #
      #        //////////////////////////////////////////////////
      #
      #        generically:
      #            <tr>
      #                <td width="20" align="center">
      #                    <img src="imgs/$icon" alt="$type" width="15" height="15" border="0" />
      #                </td>
      #                <td>
      #                    <a href="javascript:updateOpener('$address')" title="$type: $title">
      #                        $text
      #                    </a>
      #                </td>
      #            </tr>
      #
      #        //////////////////////////////////////////////////
      #
      #        with the whole thing surrounded by:
      #            <!-- begin nested links table -->
      #            <table border="0" width="100%" cellspacing="0" cellpadding="3" bgcolor="#eeeeee">
      #                <tr><td align="center" colspan="2"><img src="imgs/red_shim2.gif" alt="" width="530" height="3" border="0" vspace="2" /></td></tr>
      #                <tr>
      #                    <td>
      #                        <p style="text-align: left;">For more information explore one of the related topics below:</p>
      #                        <!-- begin nested link icon table -->
      #                        <table border="0" cellspacing="0" cellpadding="2" bgcolor="#eeeeee">
      #                            $rows
      #                        </table>
      #                        <!-- end nested icon link table -->
      #                        </td>
      #                    <td valign="top" align="right" class="small"><a href="javascript:history.back()" onmouseover="changeImages('back_c2', 'imgs/c_left_ovr.gif'); return true;" onmouseout="changeImages('back_c2', 'imgs/c_left.gif'); return true;" title="previous page"><img name="back_c2" src="imgs/c_left.gif" alt="previous page" width="18" height="17" border="0" /></a><img src="imgs/shim.gif" alt="" width="30" height="1" border="0" /><br />previous page</td>
      #                </tr>
      #            </table>
      #            <!-- end nested links table -->

      //////////////////////////////////////////////////
     */

    /*
      Links have these properties:
      $this->text
      $this->title
      $this->typeID // this is a numeric code
      $this->typeName // friendly display text
      $this->typeIcon // graphic to display (sans-path info)
      $this->url // untranslatted url // !!
     */

    $row = array(); // each row will hold one link (formatted)

    /*
      $template[CAN]['backName'] = 'c_back';
      $template[CAN]['backSrc'] = 'imgs/c_left.gif';
     */
    global $template;
    $backName = $template[$scheme]['backName'];
    $backSrc = $template[$scheme]['backSrc'];
    $backSrcOver = $template[$scheme]['backSrcOver'];
    $ruleSrc = $template[$scheme]['ruleSrc'];

    # if there are any links in the list
    if ((isset($this->linkList)) AND ( is_array($this->linkList))) {

      # for each link build a row
      foreach ($this->linkList as $linkObj) {
        if ($this->isSupAlreadyDisplayed($linkObj->url)) {
          // don't show this one.
          trace('already been shown, skip');
        }
        else {
          $row[] = sprintf(FMT_SUP_LINK_LIST_ITEM, $linkObj->returnHref(), $linkObj->title, $linkObj->text, $linkObj->typeName, $linkObj->typeIcon);
        }
      }

      # combine the table rows
      $rowsCode = implode("\n", $row);

      # insert rows into the link table
      $tmp = sprintf(FMT_SUP_LINK_TABLE, $rowsCode, $backName, $backSrc, $backSrcOver, $ruleSrc);
    }
    else {
      # if there are NO links in the list
      $tmp = sprintf(FMT_SUP_LINK_EMPTY_TABLE, $backName, $backSrc, $backSrcOver, $ruleSrc);
    }
    Return $tmp;
  }

  //function display($colour='both') { // have to account for different colour schemes (at least for the ruler, and the back button)
  function display($scheme = BOTH) {
    echo $this->returnHtml($scheme);
  }

  ///Populate $linkList from database.
  function getLinks() {
    # links come from two places
    # 1. the links table -- it hold URLs (web, ftp, etc.)
    # 2. mapped terms
    # commong bits
    switch (strtolower($this->lang)) {
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
    // text
    $text = 'text' . $end;

    # 1. the links table
    $linkTable = SUPLEMENTAL_LINK_TABLE;
    $getSupLinksQ = <<<EOQ
            SELECT sup_id, link_id, $text as text, $title as title,  type, url
            FROM $linkTable
            WHERE sup_id = $this->supID
EOQ;
    // $getSupLinksR = db_query($getSupLinksQ, '', __FILE__, __LINE__);
    $query = db_select($linkTable, 'l');
    $query->addField('l', $text, 'text');
    $query->addField('l', $title, 'title');

    $getSupLinksR = $query
        ->fields('l', array('sup_id', 'link_id', 'type', 'url'))
        ->condition('sup_id', $this->supID)
        ->execute()
        ->fetchAll();
    // if no results, return false
    if (!$getSupLinksR) {
      trace('problem retrieving links');
      $this->error[] = 'problem retrieving links';
      Return false;
    }



    $objList = array();
    foreach ($getSupLinksR as $obj) {
      $this->linkList[$obj->link_id] = new SupLink($obj);
    }


    # 2. mapped terms
    // build query
    $termsTable = TERMS_TABLE;
    $supTermMapTable = SUPLEMENTAL_TERM_MAP_TABLE;
    $supMapTable = SUPLEMENTAL_MAP_TABLE;
    $supInfoTable = SUPLEMENTAL_TABLE;

    $getSupTermsQuery = <<<OEQ
		SELECT
		  $supInfoTable.$title AS text,
		  $supInfoTable.sup_id AS url,
		  $supInfoTable.$linkTitle AS title,
		  4 as 'type',
		  0 as 'link_id'
		 FROM $termsTable, $supTermMapTable, $supMapTable, $supInfoTable
		 WHERE $termsTable.termsID =  $supTermMapTable.term_id
		  AND $supInfoTable.sup_id = $supMapTable.sup_id 
		  AND $supMapTable.term_id = $supTermMapTable.term_id 
		  AND $supTermMapTable.sup_id = $this->supID
		 GROUP BY url
OEQ;

    // $getSupLinksR = db_query($getSupTermsQuery, '', __FILE__, __LINE__);
    $query = db_select($termsTable, 't');
    $query->join($supTermMapTable, 'm', 't.termsID = m.term_id');
    $query->join($supMapTable, 'sm', 'sm.term_id = m.term_id');
    $query->join($supInfoTable, 'si', 'si.sup_id = sm.sup_id');

    $query->addField('si', "$title", 'text');
    $query->addField('si', "sup_id", 'url');
    $query->addField('si', "$linkTitle", 'title');
    $getSupLinksR = $query
        ->condition('m.term_id', $this->supID)
        ->execute()
        ->fetchAll();
    foreach ($getSupLinksR as $obj) {
      $obj->type = 4;
      $obj->link_id = 0;
      $this->linkList[] = new SupLink($obj);
    }
  }

  /// sort $linkList items
  function sortLinks() {
    // copy orginal list
    $objList = $this->linkList;

    // get the order
    $orderA = $this->getOrder();
    // sort
    foreach ($orderA as $nextLink) {
      if (isset($objList[$nextLink])) {
        $orderedLinks[] = $objList[$nextLink];
      }
      unset($objList[$nextLink]);
    }
    if (!empty($objList)) {
      trace('some links are still not sorted...');
      foreach ($objList as $key => $unsorted) {
        $orderedLinks[] = $unsorted;
        $orderA[] = $unsorted->link_id;
        trace(' * ' . $unsorted->link_id);
      }
      trace('update db with correct order info');
      $this->putOrder($orderA);
      traceVar('ordered Links', $orderedLinks);
      $this->linkList = $orderedLinks;
    }
    else {
      $this->LinkList = '';
    }
  }

  /// @return an array containing linkIDs in order
  function getOrder() {
    $supID = $this->supID;
    // build query
    //  $getOrderQ = 'SELECT sup_link_order from ' . SUPLEMENTAL_LINK_ORDER . ' WHERE sup_id=' . $supID;
    // execute query
    //   $getOrderR = db_query($getOrderQ, '', __FILE__, __LINE__);
    //   $query = db_select(SUPLEMENTAL_LINK_ORDER, s);
    $query = db_select(SUPLEMENTAL_LINK_ORDER, 's');
    $getOrderR = $query
        ->fields('s', array('sup_link_order'))
        ->condition('s.sup_id', $supID)
        ->execute()
        ->fetchAssoc();
    if (!$getOrderR) {
      trace('no order found');
      return array();
    }
    $orderS = $getOrderR['sup_link_order'];

    // split
    $orderArray = explode('|', $orderS);

    // remove empty entries
    foreach ($orderArray as $key => $value) {
      if (empty($value)) {
        unset($orderArray[$key]);
      }
    }

    traceVar('orderArray', $orderArray);
    // return order as array
    return $orderArray;
  }

  /// save links order to DB
  function putOrder($orderArray) {
    $fields = array(
      'sup_id' => $this->supID,
      'sup_link_order' => '|' . implode('|', $orderArray) . '|',
    );
    $query = db_merge(SUPLEMENTAL_LINK_ORDER)
        ->key(array(
          'sup_id' =>  $this->supID,
          ))
        ->fields($fields)
        ->execute();
    return;
        
    $supID = $this->supID;
    $orderS = '|' . implode('|', $orderArray) . '|';

    // UPDATE db or INSERT ?
    // does record already exist? lets  check...
    $checkQ = 'SELECT 1 from ' . SUPLEMENTAL_LINK_ORDER . ' WHERE sup_id=' . $supID;
    $checkR = db_query($checkQ, '', __FILE__, __LINE__);
    if (mysqli_num_rows($checkR) < 1) {
      trace('previous order NOT found, INSERT');
      $insertQ = 'INSERT into ' . SUPLEMENTAL_LINK_ORDER . " (sup_id, sup_link_order) VALUES ($supID, '$orderS')";
      return db_query_results($insertQ, '', __FILE__, __LINE__);
    }
    else {
      trace('previous order WAS found, UPDATE');
      $updateQ = 'UPDATE ' . SUPLEMENTAL_LINK_ORDER . " SET sup_link_order = '$orderS' WHERE  sup_id=$supID";
      return db_query_results($updateQ, '', __FILE__, __LINE__);
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

}

class Util {

  static function returnLinkTypes() { // get a list of type of links from DB
    // 	build query
    $typeQ = 'SELECT type_id, type_name_display, type_icon, type_handler FROM ' . SUPLEMENTAL_LINK_TYPES;
    // run query
    // $typeList = db_query_results($typeQ, Q_TYPE_SELECT, __FILE__, __LINE__);
    $query = db_select(SUPLEMENTAL_LINK_TYPES, 's');
    $typeList = $query
        ->fields('s', array('type_id', 'type_name_display', 'type_icon', 'type_handler'))
        ->execute()
        ->fetch();
    if (empty($typeList)) {
      trigger_error('unable to get link types from DB');
    }
    // turn list of objects into list of arrays keyed to link type
    $types = array();
    foreach ($typeList as $key => $typeObj) {
      $types[$typeObj->type_id] = array('name' => $typeObj->type_name_display,
        'icon' => $typeObj->type_icon,
        'handler' => $typeObj->type_handler
      );
    }
    traceVar('typelist', $types);
    Return $types;
  }

}

?>
