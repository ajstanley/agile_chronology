<script type="text/javascript" language="JavaScript">
<?php
$module_path = '/' . drupal_get_path('module', 'agile_chronology');
$img_path = "$module_path/imgs/";
?>
  self.loaded = false;
  timeline = '<?php echo $timeline_letter; ?>';
  function preloadImgs() {
      if (document.images) {
          id1800s_ovr = newImage("<?php echo $img_path; ?>tl/1800s_ovr.gif");
          id1700s_ovr = newImage("<?php echo $img_path; ?>tl/1700s_ovr.gif");
          id1600s_ovr = newImage("<?php echo $img_path; ?>tl/1600s_ovr.gif");
          id1880_ovr = newImage("<?php echo $img_path; ?>tl/1880_ovr.gif");
          id1900_ovr = newImage("<?php echo $img_path; ?>tl/1900_ovr.gif");
          id1920_ovr = newImage("<?php echo $img_path; ?>tl/1920_ovr.gif");
          id1940_ovr = newImage("<?php echo $img_path; ?>tl/1940_ovr.gif");
          id1960_ovr = newImage("<?php echo $img_path; ?>tl/1960_ovr.gif");
          id1980_ovr = newImage("<?php echo $img_path; ?>tl/1980_ovr.gif");
          id2000_ovr = newImage("<?php echo $img_path; ?>tl/2000_ovr.gif");
          ones_on = newImage("<?php echo $img_path; ?>/tl/io.gif");// small(ones) bar
          fives_on = newImage("<?php echo $img_path; ?>/tl/vo.gif"); // fives bar
          tens_on = newImage("<?php echo $img_path; ?>/tl/xo.gif"); // big ten bar
          preloadFlag = true;
      }
  }
  function init() {
      preloadImgs();
      self.loaded = true;
  }
  init(); /* TODO Defer */
</script>


<div id="timeline"> 
    <div id="tl-centuries">
        <!-- begin nested table (contains 1600s, 1700s, 1800s) -->
        <h2>
            <a href="<?php echo $_SESSION["colour"]; ?>/1800" target="main" onmouseover="changeImages('id1800s', '<?php echo $module_path; ?>/imgs/tl/1800s_ovr.gif');
      showSpecialDate(18);
      return true;" onmouseout="changeImages('id1800s', '<?php echo $img_path; ?>/tl/1800s.gif');
            return true;"><img name="id1800s" src="<?php echo $img_path; ?>/tl/1800s.gif" width="47" height="21" alt="The 1800s"></a>
        </h2>
        <h2>
            <a href="<?php echo $_SESSION["colour"]; ?>/1700" target="main" onmouseover="changeImages('id1700s', '<?php echo $img_path; ?>/tl/1700s_ovr.gif');
                  showSpecialDate(17);
                  return true;" onmouseout="changeImages('id1700s', '<?php echo $module_path; ?>/imgs/tl/1700s.gif');
                        return true;"><img name="id1700s" src="<?php echo $module_path; ?>/imgs/tl/1700s.gif" width="47" height="21" alt="The 1700s"></a>
        </h2>
        <h2>
            <a href="<?php echo $_SESSION["colour"]; ?>/1600" target="main" onmouseover="changeImages('id1600s', '<?php echo $img_path; ?>tl/1600s_ovr.gif');
                  showSpecialDate(16);
                  return true;" onmouseout="changeImages('id1600s', '<?php echo $img_path; ?>tl/1600s.gif');
                        return true;"><img name="id1600s" src="<?php echo $img_path; ?>/tl/1600s.gif" width="47" height="21" alt="The 1600s"></a>
        </h2>
    </div><!-- end #centuries -->

    <!-- begin decade headers -->
    <div id="tl-decades">
        <?php
        // TODO move to configuration
        $decades = array(1880 => 82, 1900 => 82, 1920 => 82, 1940 => 82, 1960 => 82, 1980 => 82, 2000 => 39, 2020 => 44); // decade => width_of_image
        foreach ($decades as $decade => $width) {
          echo decadeHeader($decade, $width);
        }
        ?>
    </div><!-- done decade headers -->

    <div id="tl-date">
        <h1>
            <?php
            echo $img_brace_l;
            // TODO replace shims with correct graphics for the current date, if one is set.
            // eg. 2020 = tl/numbers/numbers_w_02.gif, .../numbers_w_00.gif, .../numbers_w_02.gif, .../numbers_w_00.gif
            ?><img src="<?php echo $img_path; ?>/shim.gif" width="16" height="30" name="year_mil"  alt=""><img src="<?php echo $img_path; ?>/shim.gif" width="16" height="30" name="year_cent" alt=""><img src="<?php echo $module_path; ?>/imgs/shim.gif" width="16" height="30" name="year_dec"  alt=""><img src="<?php echo $module_path; ?>/imgs/shim.gif" width="16" height="30" name="year_ann"  alt=""><?php echo $img_brace_r; ?>
        </h1>
    </div><!-- end #tl-date -->

    <?php echo($timeline); ?>

</div><!-- end #timeline -->

