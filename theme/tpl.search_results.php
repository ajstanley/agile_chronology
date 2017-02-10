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
        <title>Modern Theatre in Context: A Critical Timeline - Search Results</title>
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
        <link rel="stylesheet" href="<?php echo "/$module_path/css/search.css"; ?>" media="screen, projection" />
        <link rel="stylesheet" href="style/style_<?php echo $this->colour; ?>.css" media="screen, projection" />
    </head>
    <body <?php
    if (isset($GLOBALS['bodyOnload'])) {
      echo 'onload="' . $GLOBALS['bodyOnload'] . '"';
    }
    ?>>
        <div id="wrapper">
            <?php
            $year = $this->year;
            $template = $this->colour == 'chrono' ? 'chronology_bar_can' : 'chronology_bar_west';
            module_load_include('inc', 'agile_chronology', "includes/$template");
            //include ('./inc.chronology_' . $this->colour . '.php');
            ?>
            <div id="middle">
                <div id="container">

                    <div id="content">
                        <div id="subhead">
                            <h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
                            <h1>Results for: <?php echo $this->queryObj->forString; ?></h1>
                        </div> <!-- end #subhead -->
                        <div id="main">


                            <ul id="search-options">
                                <li><a href="/agile/search" class="global" title="Click to begin a new search">new search</a></li>
                                <li><a href="/agile/search?action=refine" class="global" title="Click to refine your search">refine search</a></li>
                                <li><a href="/agile/search/?action=save" class="global" title="Click to save your search results">save results</a></li>
                            </ul>

                            <!--
                            [[ul]]
                            [[li]][[a href="global_chekhov.htm" class="global" title="Anton Chekhov" target="main"]]Anton Chekhov[[/a]]&nbsp;&nbsp;...William [[strong]]Shakespeare[[/strong]] is believed to have been born...[[/li]]
                            [[/ul]]
                            -->
                            <?php
                            $this->returnNotices();
                            $this->returnListOfRecords();
                            ?>

                            <?php $this->returnListOfOtherPages(); ?>

                        </div><!-- end #main -->
                    </div><!-- end #content -->

                    <!-- end nested content table -->

                </div><!-- end #container -->
                <?php
                $colour = $this->colour == 'chrono' ? 'can' : 'west';
                include ("$module_path/includes/leftnav_{$colour}.inc");
                ?>

            </div><!-- end #middle -->
            <?php include ("$module_path/theme/tpl.inc.footer.php"); ?>
        </div><!-- end #wrapper -->
    </body>
</html>
