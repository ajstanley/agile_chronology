<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
        "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
<meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
<title>Modern Theatre in Context: A Critical Timeline - Help</title>
<link rel="stylesheet" href="style/chrono.css" media="screen, projection" />
<script type="text/javascript" language="Javascript" src="js/timeline.js"></script>
<script type="text/javascript" language="JavaScript" src="js/common.js"></script>

<?php
/*
Reuben - can this be moved into either common.js or otherwise compacted? The images are not west/can specific, but there seems to be a timeline flag of some sort in there - see line 2
*/
?>
    <script language="Javascript" type="text/javascript">

        var preloadFlag = false;
        var count = 0;
        var scheme = '<?php echo $this->colour;?>';
        var year = <?php echo $this->year; ?>;

        function preloadImages() { 
            if (document.images) {
                back_g_ovr  =   newImage("imgs/g_left_ovr.gif");

                preloadFlag = true;
            }
        }
        
        ready(function(){
            preloadImages();
            showdate(year);
        });
                        
    </script>
<link rel="stylesheet" href="style/style_<?php echo $this->colour;?>.css" media="screen, projection" />
<link rel="stylesheet" href="style/search.css" media="screen, projection" />
<link rel="stylesheet" href="style/style_print.css" media="print" />
</head>
<body>
<div id="wrapper">
<?php 
$year = $this->year;
include ('./inc.chronology_'.$this->colour.'.php'); 
?>
<div id="middle">
<div id="container">
<div id="content">
<div id="subhead">
<h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
<h1>Help</h1>
</div> <!-- end #subhead -->
<div id="main">

<div id="helloworld">
<div>
Lorem ipsum dolor sit amet.
</div>
</div>

</div><!-- end #main -->
</div><!-- end #content -->
</div><!-- end #container -->
<?php include ('./inc.chrono.leftnav-'.$this->colour.'.php'); ?>
</div><!-- end #middle -->
<?php include ('./tpl.inc.footer.php'); ?>
</div><!-- end #wrapper -->
</body>
</html>
