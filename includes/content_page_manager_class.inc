<?php

/*
  classes defined:
  ContentPageManager
 */

class ContentPageManager {
  /* controller for content pages
   * 
   * handles:
   * - session data (saved searches)
   * - page displaying
   */

  var $language;
  var $error = false;
  var $warning = false;
  var $year = '';
  var $colour = "CAN";

  function display($pageType = CONTENT_PAGE_HELP, $prof = NULL) {
    // Set last year and colour from session
    $this->setYear();
    $this->setColour();

    if (isset($_GET['action']) && (!empty($_GET['action']))) {
      $action = strtoupper($_GET['action']);
      switch ($action) {
        case('PRINT'):
          if (isset($_GET['page'])) {
            $prof->startTimer("setPage");
            $this->queryObj->setPage($_GET['page']);
            $prof->stopTimer("setPage");
          }
          $this->showPrintPage($pageType);
          break;
        default:
          $this->showPage($pageType);
          break;
      }
    }
    else {
      $this->showPage($pageType, $prof);
    }
  }

  function ContentPageManager($language, $prof = NULL) {
    $this->setLanguage($language, $prof);
  }

  function showPage($pageType, $prof) {
    if ($pageType == CONTENT_PAGE_HELP) {
      $prof->startTimer("showPage " . CONTENT_PAGE_HELP);
      $module_path = drupal_get_path('module', 'agile_chronology');

      include("$module_path/theme/tpl.help.php");
      $prof->stopTimer("showPage " . CONTENT_PAGE_HELP);
    }
  }

  function showPrintPage() {
    global $prof;

    if ($pageType == CONTENT_PAGE_HELP) {
      $prof->startTimer("showPrintPage " . CONTENT_PAGE_HELP);
      include('tpl.help_print.php');
      $prof->stopTimer("showPrintPage " . CONTENT_PAGE_HELP);
    }
  }

  function setLanguage($language, $prof = NULL) {

    $prof->startTimer("setLanguage");
    $language = strtoupper($language);
    if (($language != 'EN') AND ( $language != 'FR')) {
      $this->error('Illegal value. Expecting: EN|FR, received: "' . $language . '", defauling to "EN"', WARNING);
      $language = 'EN';
    }
    $this->language = $language;
    $prof->stopTimer("setLanguage");
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

  function error($msg, $code = '') {
    switch ($code) {
      case WARNING:
        $this->warning[] = $msg;
        break;
      case ERROR:
        $this->error[] = $msg;
        exit;
        break;
      default:
        $this->error[] = $msg;
        trigger_error(get_class($this) . ': ' . $msg, FATAL);
    }
  }

}

?>