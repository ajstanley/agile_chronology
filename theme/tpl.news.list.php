<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
    "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        <meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
        <meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
        <title>Modern Theatre in Context: A Critical Timeline - World News <?php echo $year . "-" . ($year + 4); ?></title>

        <script type="text/javascript" language="Javascript" src="<?php echo "/$module_path/js/timeline.js"; ?>"></script>
        <script type="text/javascript" language="JavaScript" src="<?php echo "/$module_path/js/common.js"; ?>"></script>

        <?php
        $module_path = drupal_get_path('module', 'agile_chronology');
        ?>
        <script language="Javascript" type="text/javascript">

          var preloadFlag = false;
          var count = 0;
          var year = <?php echo $year; ?>;

          function preloadImages() {
              if (document.images) {
                  back_g_ovr = newImage("imgs/g_left_ovr.gif");

                  preloadFlag = true;
              }
          }

          function init() {
              preloadImages();
          }

        </script>
        <link rel="stylesheet" href="<?php echo "/$module_path/css/chrono.css"; ?>" media="screen, projection" />
        <link rel="stylesheet" href="<?php echo "/$module_path/css/search.css"; ?>" media="screen, projection" />
        <link rel="stylesheet" href="<?php echo "/$module_path/css/style_can.css"; ?>" media="screen, projection" />
    </head>
    <body>
        <div id="wrapper">
            <?php include ("$module_path/includes/chronology_bar_can.inc"); ?>
            <div id="middle">
                <div id="container">
                    <div id="content">
                        <div id="subhead">
                            <h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
                            <h1><?php echo $prevLink; ?>World News: <?php echo $year . " - " . ($year + 4); ?> <?php echo $nextLink; ?></h1>

                        </div> <!-- end #subhead -->
                        <div id="main">
                            <?php echo $newslist; ?>
                            <div id="back-fwd"><a href="javascript:history.back()" title="previous entry" id="back-btn">back</a></div>
                        </div><!-- end #main -->
                        <!-- no need for #subfoot in this template -->
                    </div><!-- end #content -->
                </div><!-- end #container -->
                <?php
                $module_path = drupal_get_path('module', 'agile_chronology');
                include ("$module_path/includes/leftnav_can.inc");
                ?>
            </div><!-- end #middle -->
            <?php include ("$module_path/theme/tpl.inc.footer.php"); ?>
        </div><!-- end #wrapper -->
    </body>
</html>
