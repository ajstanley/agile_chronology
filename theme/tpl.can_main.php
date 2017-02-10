<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
    "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <?php
    $module_path = drupal_get_path('module', 'agile_chronology');
    module_load_include('inc', 'agile_chronology', 'includes/utilities');
    ?>
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        <meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
        <meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
        <base href="<?php echo "/chrono"; ?>">
            <title>Modern Theatre in Context: A Critical Timeline - <?php echo $titleDate; ?></title>
            <link rel="stylesheet" href="<?php echo "$module_path/css/chrono.css"; ?>" media="screen, projection" />
            <link rel="stylesheet" href="<?php echo "$module_path/css/style_can.css"; ?>" media="screen, projection" />
            <script type="text/javascript" language="Javascript" src="<?php echo "$module_path/js/timeline.js"; ?>"></script>
            <script type="text/javascript" language="JavaScript" src="<?php echo "$module_path/js/common.js"; ?>"></script>
            <script language="Javascript" src="<?php echo "$module_path/js/overlib_mini.js"; ?>" type="text/javascript"></script>


            <script language="Javascript" type="text/javascript">

              var preloadFlag = false;
              var count = 0;
              var scheme = 'CAN';
              var year = <?php echo $record->year; ?>;
              var trdir = '<?php echo "/$module_path/"; ?>'
              function preloadImages() {
                  if (document.images) {
                      back_c_ovr = newImage(trdir + "imgs/c_left_ovr.gif");
                      fwd_c_ovr = newImage(trdir + "imgs/c_right_ovr.gif");
                      back_c2 = newImage(trdir + "imgs/c_left_ovr.gif");
                      fwd_c2 = newImage(trdir + "imgs/c_right_ovr.gif");
                      preloadFlag = true;
                  }
              }

              ready(function () {
                  preloadImages();
                  showdate(<?php echo $record->year; ?>);
              });
            </script>

            <script language="Javascript" type="text/javascript">
              var headline_copy = "<?php echo $record->context; ?>";
              var key_plays_body = "<?php echo $record->returnKeyPlaysOverlibCode(); ?>";
            </script>

    </head>
    <body>
 <div id="overDiv" style="position: absolute; visibility: hidden; z-index: 1000;"></div>
        <div id="wrapper">
            <?php include ("$module_path/includes/chronology_bar_can.inc"); ?>
            <div id="middle">
                <div id="container">

                    <div id="content">
                        <div id="subhead">
                            <h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
                            <h1><?php echo $prevTop; ?> <?php echo $record->year; ?> <?php echo $nextTop; ?></h1>
                        </div> <!-- end #subhead -->

                        <div id="main">
                            <?php echo $imageCode; ?>
                            <?php echo $record->body; ?>

                        </div><!-- end #main -->
                        <div id="subfoot">

                            <?php echo $record->termsLinksIntro; ?>
                            <ul id="chrono-links">
                                <?php echo $record->termsLinksList; ?>
                            </ul>

                            <ul id="feature-links">
                                <?php
                                if (!$record->keyPlaysList->is_empty()) {
                                  ?>
                                  <li id="keyplay"><?php echo $record->keyPlaysList->linkCode; ?></li>
                                <?php }
                                ?>
                                <li id="headline"><a href="#" title="Click for headlines of the <?php echo $record->decade; ?>0s" onclick="event.preventDefault();
                                      return overlib(headline_copy, CAPICON, '<?php echo "$module_path/"; ?>imgs/headlines2.gif', CAPTION, '&nbsp;', BORDER, 2, FGCOLOR, '#eeeeee', BGCOLOR, '#777777', TEXTCOLOR, '#000000', WIDTH, 254, PADX, 10, 10, PADY, 10, 10, OFFSETX, -440, OFFSETY, -240, CLOSECLICK, STICKY, TEXTFONTCLASS, 'headline', TEXTFONT, 'serif', CAPTIONSIZE, 2, TEXTSIZE, 1, CAPTIONFONT, 'serif')">Headlines of the <?php echo $record->decade; ?>0s</a></li>
                                <li id="news"><?php echo $record->newsLink; ?></li>
                            </ul>

                        </div><!-- end #subfoot -->
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
