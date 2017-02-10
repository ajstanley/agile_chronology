<?php
if ($this->colour == BOTH || $this->colour == CAN) {
  $colour_suffix = "chrono";
  $file = "leftnav_can.inc";
}
else {
  $colour_suffix = "chronowest";
  $file = "leftnav_west.inc";
}
$module_path = drupal_get_path('module', 'agile_chronology');
module_load_include('inc', 'agile_chronology', 'includes/utilities');
$file = "leftnav_can.inc";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
    "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="content-type" content="text/html; charset=utf-8" />
        <meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
        <meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
        <title>Modern Theatre in Context: A Critical Timeline - <?php echo $this->title; ?></title>
        <base href="<?php echo $path['root']; ?>">
            <link rel="stylesheet" href="<?php echo "/$module_path/css/chrono.css"; ?>" media="screen, projection" />
            <script type="text/javascript" language="Javascript" src="<?php echo "/$module_path/js/timeline.js"; ?>"></script>
            <script type="text/javascript" language="JavaScript" src="<?php echo "/$module_path/js/common.js"; ?>"></script>
            <script language="Javascript" src="<?php echo "$module_path/js/overlib_mini.js"; ?>" type="text/javascript"></script>

    </head>

    <body>
        <div id="wrapper">

            <?php include ("$module_path/includes/chronology_bar_can.inc"); ?>
            <div id="middle">
                <div id="container">
                    <div id="content">
                        <div id="subhead">
                            <h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
                            <h1><?php echo $this->title; ?></h1>
                            <h2><?php echo $this->subtitle; ?></h2>
                        </div> <!-- end #subhead -->
                        <div id="main">
                            <?php echo $this->imageCode; ?>
                            <?php echo $this->body; ?>

                            <div id="back-fwd"><a href="javascript:history.back()" title="previous entry" id="back-btn">back</a></div>

                        </div><!-- end #main -->
                        <div id="subfoot">
                            <?php echo $this->linksCode; ?>
                        </div><!-- end #subfoot -->
                    </div><!-- end #content -->
                </div><!-- end #container -->
                <?php 
                $module_path = drupal_get_path('module', 'agile_chronology');

                include ("$module_path/includes/$file"); 
                ?>
            </div><!-- end #middle -->
            <?php include ("$module_path/theme/tpl.inc.footer.php"); ?>
        </div><!-- end #wrapper -->

    </body>
</html>
