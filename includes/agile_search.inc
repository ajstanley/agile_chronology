<?php

/**
 * this is the public interface for the regular and advanced search, as well
 * as the search results.
 * */
function agile_search() {
  define('TXT_EN_SEARCH_NO_RESULTS', 'Sorry, no results found. Try a broader search.');
  module_load_include('inc', 'agile_chronology', 'includes/profiler');
  module_load_include('inc', 'agile_chronology', 'includes/record_class');
  module_load_include('inc', 'agile_chronology', 'includes/search_manager_class');
  module_load_include('inc', 'agile_chronology', 'includes/search_query');


  $prof = new Profiler_lee(false);

  $prof->startTimer('include_required_files');
  $prof->stopTimer('include_required_files');

  /**
   * have to call session_start if I want to use the _SESSION variables
   * must happen after the code for the objects have been included or we get the
   * dreaded '__PHP_Incomplete_Class', which, it turns out, is pretty useless really.
   * */
  $prof->startTimer('send_cookie');
  $prof->stopTimer('send_cookie');

  $manager = new SearchManagerBase('EN', $prof); // !! blatant assumption of language.
  $manager->manage($prof);

  $prof->printTimers(false);
}

?>