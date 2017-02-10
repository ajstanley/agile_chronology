<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
    "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        <meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
        <meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
        <title>Modern Theatre in Context: A Critical Timeline - Search the Chronology</title>
        <link rel="stylesheet" href="style/chrono.css" media="screen, projection" />
        <script type="text/javascript" language="Javascript" src="js/timeline.js"></script>
        <script type="text/javascript" language="JavaScript" src="js/common.js"></script>

        <?php
        $module_path = drupal_get_path('module', 'agile_chronology');
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
        <link rel="stylesheet" href="<?php echo "/$module_path/css/chrono.css"; ?>" media="screen, projection" />
        <link rel="stylesheet" href="<?php echo "/$module_path/css/search.css"; ?>" media="screen, projection" />
        <script type="text/javascript" language="Javascript" src="<?php echo "/$module_path/js/timeline.js"; ?>"></script>
        <script type="text/javascript" language="JavaScript" src="<?php echo "/$module_path/js/common.js"; ?>"></script>
    </head>
    <body>
        <div id="wrapper">
            <?php
            $year = $this->year;
            $template = $this->colour == 'chrono' ? 'chronology_bar_can' : 'chronology_bar_west';
            module_load_include('inc', 'agile_chronology', "includes/$template");
            ?>
            <div id="middle">
                <div id="container">
                    <div id="content">
                        <div id="subhead">
                            <h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
                            <h1>Search the Chronology</h1>
                        </div> <!-- end #subhead -->
                        <div id="main">

                            <form method="get" action="/agile/search" name="search_form" onsubmit="return formIsValid();">
                            <!--<input type='hidden' name='action' value='list'>-->
                                <!-- begin nested form table -->
                                <div id="keywords">
                                    <div>
                                        Search:
                                        <select name="query[1][type]" size="1" class="search">
                                            <option <?php echo $selected['type1']['ANY']; ?> label="all keywords" value="ANY">all keywords</option>
                                            <option <?php echo $selected['type1']['PW']; ?> label="author" value="PW">author</option>
                                            <option <?php echo $selected['type1']['PT']; ?> label="title" value="PT">title</option>
                                            <option <?php echo $selected['type1']['TN']; ?> label="venue" value="TN">venue</option>
                                            <option <?php echo $selected['type1']['TG']; ?> label="theatre company" value="TG">theatre company</option>
                                            <option <?php echo $selected['type1']['AC']; ?> label="performer" value="AC">performer</option>
                                            <option <?php echo $selected['type1']['DE']; ?> label="designer" value="DE">designer</option>
                                            <option <?php echo $selected['type1']['PR']; ?> label="director/producer" value="PR">director/producer</option>
                                            <option <?php echo $selected['type1']['GEO']; ?> label="geographic location" value="GEO">geographic location</option>
                                            <option <?php echo $selected['type1']['TXT']; ?> label="full text" value="TXT">full text</option>
                                        </select> for
                                        <input value="<?php echo $strings['part1']; ?>" type="text" name="query[1][string]" size="23" maxlength="150" class="txt_field2" />
                                    </div>
                                    <div>
                                        <select name="query[2][operator]" size="1" class="search">
                                            <option <?php echo $selected['operator2']['AND']; ?> label="and" value="and" selected>and</option>
                                            <option <?php echo $selected['operator2']['OR']; ?> label="or" value="or">or</option>
                                            <option <?php echo $selected['operator2']['AND NOT']; ?> label="not" value="not">not</option>
                                        </select>
                                        <select name="query[2][type]" size="1" class="search">
                                            <option <?php echo $selected['type2']['ANY']; ?> label="all keywords" value="ANY">all keywords</option>
                                            <option <?php echo $selected['type2']['PW']; ?> label="author" value="PW">author</option>
                                            <option <?php echo $selected['type2']['PT']; ?> label="title" value="PT">title</option>
                                            <option <?php echo $selected['type2']['TN']; ?> label="venue" value="TN">venue</option>
                                            <option <?php echo $selected['type2']['TG']; ?> label="theatre company" value="TG">theatre company</option>
                                            <option <?php echo $selected['type2']['AC']; ?> label="performer" value="AC">performer</option>
                                            <option <?php echo $selected['type2']['DE']; ?> label="designer" value="DE">designer</option>
                                            <option <?php echo $selected['type2']['PR']; ?> label="director/producer" value="PR">director/producer</option>
                                            <option <?php echo $selected['type2']['GEO']; ?> label="geographic location" value="GEO">geographic location</option>
                                            <option <?php echo $selected['type2']['TXT']; ?> label="full text" value="TXT">full text</option>
                                        </select>
                                        for
                                        <input  value="<?php echo $strings['part2']; ?>" type="text" name="query[2][string]" size="23" maxlength="150" class="txt_field2" />
                                    </div>
                                    <div>
                                        <select name="query[3][operator]" size="1" class="search">
                                            <option <?php echo $selected['operator3']['AND']; ?> label="and" value="and" selected>and</option>
                                            <option <?php echo $selected['operator3']['OR']; ?> label="or" value="or">or</option>
                                            <option <?php echo $selected['operator3']['AND NOT']; ?> label="not" value="not">not</option>
                                        </select>
                                        <select name="query[3][type]" size="1" class="search">
                                            <option <?php echo $selected['type3']['ANY']; ?> label="all keywords" value="ANY">all keywords</option>
                                            <option <?php echo $selected['type3']['PW']; ?> label="author" value="PW">author</option>
                                            <option <?php echo $selected['type3']['PT']; ?> label="title" value="PT">title</option>
                                            <option <?php echo $selected['type3']['TN']; ?> label="venue" value="TN">venue</option>
                                            <option <?php echo $selected['type3']['TG']; ?> label="theatre company" value="TG">theatre company</option>
                                            <option <?php echo $selected['type3']['AC']; ?> label="performer" value="AC">performer</option>
                                            <option <?php echo $selected['type3']['DE']; ?> label="designer" value="DE">designer</option>
                                            <option <?php echo $selected['type3']['PR']; ?> label="director/producer" value="PR">director/producer</option>
                                            <option <?php echo $selected['type3']['GEO']; ?> label="geographic location" value="GEO">geographic location</option>
                                            <option <?php echo $selected['type3']['TXT']; ?> label="full text" value="TXT">full text</option>
                                        </select> for
                                        <input value="<?php echo $strings['part3']; ?>" type="text" name="query[3][string]" size="23" maxlength="150" class="txt_field2" />
                                    </div>
                                </div>

                                <div id="date-range">Search dates <label for="dateFirst"><strong>from:</strong></label>&nbsp;<input type="text" name="dateFirst" id="dateFirst" size="5" maxlength="4"  value='<?php echo $strings['dateFirst']; ?>' onfocus="if (this.value == 'YYYY')
                                          this.value = '';" class="search" />
                                    <label for="dateLast"><strong>to</strong></label>&nbsp;&nbsp;<input type="text" name="dateLast" id="dateLast" size="5" maxlength="4" value='<?php echo $strings['dateLast']; ?>' onfocus="if (this.value == 'YYYY')
                                              this.value = '';" class="search" />
                                </div>


                                <div id="region">Display results for:&nbsp;&nbsp;<label for="chronoCan" onclick='toggle("chronoCan");
                                      return false;'><img src="<?php echo "/$module_path/"; ?>imgs/cdn_sml.gif" alt="search the Canadian chronology" width="20" height="10" border="0" /></label>&nbsp;&nbsp;<input onclick="ensureOneTimeline();" type="checkbox" name="timeline[can]"  id="chronoCan" <?php echo $checked['timeline']['CAN']; ?>  value='true'/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<label for="chronoWest"  onclick='toggle("chronoWest");
                                            return false;'><img src="<?php echo "/$module_path/"; ?>imgs/uk_sml.gif" alt="search the British / Irish / American chronology" width="19" height="10" border="0" />&nbsp;<img src="<?php echo "/$module_path/"; ?>imgs/eire_sml.gif" alt="search the British / Irish / American chronology" width="19" height="10" border="0" />&nbsp;<img src="<?php echo "/$module_path/"; ?>imgs/us_sml.gif" alt="search the British / Irish / American chronology" width="19" height="10" border="0" /></label>&nbsp;&nbsp;<input type="checkbox" onclick="ensureOneTimeline();" name="timeline[west]" id="chronoWest" <?php echo $checked['timeline']['WEST']; ?> value='true'/></td>
                                </div>

                                <div id="order">Display in <label for="orderASC"> <strong>ascending</strong></label> <input type="radio" name="order" id="orderASC" value="ASC" <?php echo $checked['order']['ASC']; ?> /> or <label for="orderDESC"><strong>descending</strong></label> <input type="radio" name="order" id="orderDESC" value='DESC' <?php echo $checked['order']['DESC']; ?> /> date order
                                </div>

                                <div align="center">
                                    <input type="image" src="<?php echo "/$module_path/"; ?>imgs/search_btn.gif" title="search" />
                                </div>

                            </form>
                        </div><!-- end #main -->
                    </div><!-- end #content -->
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
