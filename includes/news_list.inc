<?php

function agile_newslist($year = 1865) {
  module_load_include('inc', 'agile_chronology', 'includes/news_utilities');
  module_load_include('inc', 'agile_chronology', 'includes/timeline_shared');
  module_load_include('inc', 'agile_chronology', 'includes/news_utilities');
  module_load_include('inc', 'agile_chronology', 'includes/db_util');
  $module_path = drupal_get_path('module', 'agile_chronology');

  if (isset($_GET['print'])) {
    //$Sup->displayPrint();
  }
  else {
    //$Sup->format_body(); 
    //$Sup->display();
    $newslist = getNewsList($year);
    list($prevLink, $nextLink) = getPrevNext($year);
    include ("$module_path/theme/tpl.news.list.php");
  }
}
