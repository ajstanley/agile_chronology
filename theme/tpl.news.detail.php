<?php

if (isset($_SESSION["colour"])) {
	if($_SESSION["colour"] == "can") {
		$scheme = CAN;
	} else {
		$scheme = WEST;
	}
} else {
	$scheme = BOTH;
}

if($scheme == BOTH || $scheme == CAN) {
	$colour_suffix = "can";	
} else {
	$colour_suffix = "west";
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
        "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<meta name="keywords" content="chronology, modern drama, drama, timeline, interactive, multimedia, Canada, Canadian, America, American, Europe, European, dramatist, plays" />
<meta name="description" content="Modern Theatre in Context is a multimedia chronology of modern drama." />
<title>Modern Theatre in Context: A Critical Timeline - World News <?php echo $year."-".($year+4); ?></title>
<base href="<?php echo $path['root']; ?>">
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
		var scheme = '<?php echo $scheme; ?>';;
		var year = <?php echo $year; ?>;

		function preloadImages() { 
            if (document.images) {
                back_g_ovr	=	newImage("imgs/g_left_ovr.gif");

                preloadFlag = true;
            }
        }
        
        function init(){
            preloadImages();
        }
                        
    </script>
<link rel="stylesheet" href="style/style_<?php echo $colour_suffix;?>.css" media="screen, projection" />
<link rel="stylesheet" href="style/search.css" media="screen, projection" />
<link rel="stylesheet" href="style/style_print.css" media="print" />
</head>
<body>
<div id="wrapper">
<?php include ('./inc.chronology_'.$colour_suffix.'.php'); ?>
<div id="middle">
<div id="container">
<div id="content">
	<div id="subhead">
	<h1 class="toph1">Modern Theatre in Context: A Critical Timeline</h1>
		<h1><?php echo $prevLink; ?> World News: <?php echo $year." - ".($year+4); ?> <?php echo $nextLink; ?></h1>
	</div> <!-- end #subhead -->
	<div id="main">
		<?php echo $articles;?>
<div id="back-fwd"><a href="javascript:history.back()" title="previous entry" id="back-btn">back</a></div>
	</div><!-- end #main -->
	<div id="subfoot">
		<?php echo $category_links;?>
	</div><!-- end #subfoot -->
</div><!-- end #content -->
</div><!-- end #container -->
<?php include ('./inc.chrono.leftnav-'.$colour_suffix.'.php'); ?>
</div><!-- end #middle -->
<?php include ('./tpl.inc.footer.php'); ?>
</div><!-- end #wrapper -->
</body>
</html>
