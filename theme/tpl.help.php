<?php
$module_path = drupal_get_path('module', 'agile_chronology');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
    "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        <meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
        <meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
        <title>Modern Theatre in Context: A Critical Timeline - Help</title>

        <link rel="stylesheet" href="<?php echo "/$module_path/css/chrono.css"; ?>" media="screen, projection" />
        <script type="text/javascript" language="Javascript" src="<?php echo "/$module_path/js/timeline.js"; ?>"></script>
        <script type="text/javascript" language="JavaScript" src="<?php echo "/$module_path/js/common.js"; ?>"></script>

        <?php
        /*
          Reuben - can this be moved into either common.js or otherwise compacted? The images are not west/can specific, but there seems to be a timeline flag of some sort in there - see line 2
         */
        ?>
        <script language="Javascript" type="text/javascript">

          var preloadFlag = false;
          var count = 0;
          var scheme = '<?php echo $this->colour; ?>';
          var year = <?php echo $this->year; ?>;

          function preloadImages() {
              if (document.images) {
                  back_g_ovr = newImage("imgs/g_left_ovr.gif");

                  preloadFlag = true;
              }
          }

          ready(function () {
              preloadImages();
              showdate(year);
          });

        </script>
        <link rel="stylesheet" href="<?php echo "/$module_path/css/style_" . $this->colour . '.css'; ?>" media="screen, projection" />
        <link rel="stylesheet" href="<?php echo "/$module_path/css/search.css"; ?>" media="screen, projection" />
        <link rel="stylesheet" href="<?php echo "/$module_path/css/style_print.css"; ?>" media="print" />
    </head>
    <body>
        <div id="wrapper">
            <?php
            $year = $this->year;
            $identifier = $this->colour == 'chrono' ? 'can' : 'west';
            include ("$module_path/includes/chronology_bar_$identifier.inc");
            ?>
            <div id="middle">
                <div id="container">
                    <div id="content">
                        <div id="subhead">
                            <h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
                            <h1>Select a help topic below:</h1>
                        </div> <!-- end #subhead -->
                        <div id="main">

                            <ul id="help-links">
                                <li><a href="#timeline1" title="how the timeline works">How the timeline works</a></li>
                                <li><a href="#search1" title="how to use the simple search">How to use the 'simple search' feature effectively</a></li>
                                <li><a href="#search2" title="how to use the advanced search">How to use the advanced search feature</a></li>
                                <li><a href="#results1" title="how to refine search results">How to refine search results</a></li>
                                <li><a href="#printresults" title="how to print search results">How to print search results</a></li>
                                <li><a href="#saveresults" title="how to save search results">How to save search results</a></li>
                                <li><a href="#credits" title="how to save search results">How to view credits</a></li>
                            </ul>

                            <div id="help-text">
                                <h2><a id="timeline1"></a>How the timeline works:</h2>
                                <p>Each chronology - Canadian and Western (US, UK and Ireland) - has its own timeline. To view a chronology entry, click on the timeline bars in the upper portion of the window. Grey bars indicate years for which no information is currently available in the database. Black bars indicate that information is available. When the mouse pointer is positioned over one of the timeline bars the date is displayed to the right of the timeline. To view the records for that year, click on the bar. You may also choose to view information decade by decade by clicking on the decades displayed above and to the left of the timeline bars.</p>

                                <h2><a id="search1"></a>How to use the 'simple search' feature effectively:</h2>
                                <p>At any time you may use the search field in the left portion of the window ("search for...") to find specific information in the chronology. By entering a search term and clicking "go" it is possible to search the database for chronology references to one or more terms. For example, entering "eugene o'neill" will return a list of chronology entries that contain "eugene o'neill". Searching for "eugene" will return a list of entries that contain the text "eugene".</p>

                                <h2><a href="#top" title="back to top" class="top-link">top</a></h2>

                                <h2><a id="search2"></a>How to use the advanced search feature:</h2>
                                <p>The advanced search page can be accessed by clicking on "advanced search" below the simple search input field. Every chronology entry is indexed with important terms. The advanced search fields will search all keywords by default. To narrow your search, choose a type of keyword to search for (for example, if you are interested in the acting career of a well-known playwright, enter the playwright's name with the term type set to "performer". You can also search the text in the same way as the simple search by selecting "full text" as the term type. To limit the range of results, you may also choose a boolean operator and add a search term to the second or third field. For example, searching for "shaw" not "george bernard" will return entries that reference Lady Astor Shaw and Irwin Shaw, but not George Bernard Shaw. Additionally, you may narrow your search further by specifying a date range or by deselecting one of the chronologies for your search query. To sort your results from the earliest entries to the most recent, specify ascending order. To display your results from most recent to earliest entries, select descending order.</p>

                                <h2><a href="#top" title="back to top" class="top-link">top</a></h2>

                                <h2><a id="results1"></a>How to refine search results:</h2>
                                <p>If, after performing a search, the results listings are too long or too broad, you may choose to refine your search by clicking "refine search" on the results page. This will return you to the advanced search page with the same fields you selected previously, and you can modify your search query to improve the results. Try choosing more or less specific keyword types, or adding search criteria to the other search fields.</p>

                                <h2><a id="printresults"></a>How to print search results:</h2>
                                <p>To print out your search results for future reference, simply click "print results" and a new window will open with a page formatted for printing. Click the print icon on your browser or select "print..." from the browser menu bar to send the document to your printer.</p>

                                <h2><a href="#top" title="back to top" class="top-link">top</a></h2>

                                <h2><a id="saveresults"></a>How to save search results:</h2>
                                <p>You may save up to three sets of search results during any session. To do so, click "save results" on the search results page. Click "ok" and a link will appear in the left navigation bar - saved search results: 1. To return to your search results listings at any time, click on the numbered link. Up to three sets of results may be saved. Thereafter, the most recent search will automatically remove the oldest saved results listing. Links to saved results are only saved for the current session: closing the browser window will delete the saved results from the left navigation bar.</p>

                                <h2><a id="credits"></a>How to view credits:</h2>
                                <p>All images that appear in the chronology have a caption and credit. To view the information, hover your mouse over the image and the text will appear. To view page-specific author credits for chronology and supplementary entries, click "printable version". Authors are listed in the citation information. For general credits, please see <a href="intro.php">About this site</a></p>

                                <h2><a href="#top" title="back to top" class="top-link">top</a></h2>
                            </div><!-- end #help-text -->

                        </div><!-- end #main -->
                    </div><!-- end #content -->
                </div><!-- end #container -->
                <?php
                $module_path = drupal_get_path('module', 'agile_chronology');
                include ("$module_path/includes/leftnav_$identifier.inc");
                ?>
            </div><!-- end #middle -->
             <?php include ("$module_path/theme/tpl.inc.footer.php"); ?>
        </div><!-- end #wrapper -->
    </body>
</html>
