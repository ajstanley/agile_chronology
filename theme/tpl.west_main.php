<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
    "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
    <?php
    $module_path = drupal_get_path('module', 'agile_chronology');
    module_load_include('inc', 'agile_chronology', 'includes/utilities');
    ?>
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        <meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
        <meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
        <base href="<?php echo "/chronowest"; ?>">
            <title>Modern Theatre in Context: A Critical Timeline - <?php echo $titleDate; ?></title>
            <link rel="stylesheet" href="<?php echo "$module_path/css/chrono.css"; ?>" media="screen, projection" />
            <link rel="stylesheet" href="<?php echo "$module_path/css/style_west.css"; ?>" media="screen, projection" />
            <script type="text/javascript" language="Javascript" src="<?php echo "$module_path/js/timeline.js"; ?>"></script>
            <script type="text/javascript" language="JavaScript" src="<?php echo "$module_path/js/common.js"; ?>"></script>
            <script language="Javascript" src="<?php echo "$module_path/js/overlib_mini.js"; ?>" type="text/javascript"></script>


            <?php
            $module_path = drupal_get_path('module', 'agile_chronology');
            module_load_include('inc', 'agile_chronology', 'includes/utilities');
            ?>


            <script type="text/javascript" language="JavaScript">
              self.loaded = false;
              timeline = 'w';
              var imdir = '/sites/all/modules/agile_chronology/imgs/';
              function preloadImgs() {
                  if (document.images) {
                      id1800s_ovr = newImage(imdir + "tl/1800s_ovr.gif");
                      id1700s_ovr = newImage(imdir + "tl/1700s_ovr.gif");
                      id1600s_ovr = newImage(imdir + "tl/1600s_ovr.gif");
                      id1880_ovr = newImage(imdir + "tl/1880_ovr.gif");
                      id1900_ovr = newImage(imdir + "tl/1900_ovr.gif");
                      id1920_ovr = newImage(imdir + "tl/1920_ovr.gif");
                      id1940_ovr = newImage(imdir + "tl/1940_ovr.gif");
                      id1960_ovr = newImage(imdir + "tl/1960_ovr.gif");
                      id1980_ovr = newImage(imdir + "tl/1980_ovr.gif");
                      id2000_ovr = newImage(imdir + "tl/2000_ovr.gif");
                      id2020_ovr = newImage(imdir + "tl/2020_ovr.gif");
                      ones_on = newImage(imdir + "tl/io.gif");// small(ones) bar
                      fives_on = newImage(imdir + "tl/vo.gif"); // fives bar
                      tens_on = newImage(imdir + "tl/xo.gif"); // big ten bar
                      preloadFlag = true;
                  }
              }
              ready(function () {
                  preloadImgs();
                  showdate(<?php echo $record->year; ?>);
                  self.loaded = true;
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
            <?php include ("$module_path/includes/chronology_bar_west.inc"); ?>
            <div id="middle">
                <div id="container">

                    <div id="content">
                        <div id="subhead">
                            <h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
                            <h1><?php echo $prevTop; ?><?php echo $record->heading; ?><?php echo $nextTop; ?></h1>

                            <?php
                            if (!$record->isSpecialYear()) {
                              echo $record->monthNav;
                            }
                            ?>

                        </div> <!-- end #subhead -->
                        <div id="main">
                            <?php echo $imageCode; ?>
                            <?php echo $record->body; ?>
                            <!--
                            <div id="back-fwd"><a href="#"><img id="back_w2" src="imgs/w_left.gif" alt="previous entry" width="18" height="17" /></a><a href="#"><img id="fwd_w2" src="imgs/w_right.gif" alt="next entry" width="18" height="17" /></a></div>
                            -->
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
                include ("$module_path/includes/leftnav_west.inc");
                ?>

            </div><!-- end #middle -->
            <?php include ("$module_path/theme/tpl.inc.footer.php"); ?>
        </div><!-- end #wrapper -->
    </body>
</html>