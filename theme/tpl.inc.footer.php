<div id="footer">
<?php if(isset($authorship) && isset($dateString)) { ?>
<p>MLA citation for this chronology:<br /><?php echo $authorship.' "';
if(isset($sup_print_title)){ echo $sup_print_title; } else { echo $year; }
echo '" '; ?><em>Modern Theatre in Context: A Critical Timeline</em> in <em>Modern Theatre in Context</em>. Christopher Innes, ed. Mar. 2003-. Canada Research Chair in Performance and Culture. [<?php echo $dateString; ?>]. 
&lsaquo;http://<?php echo $_SERVER['HTTP_HOST'] . $_SERVER['REQUEST_URI']; ?>&rsaquo;.</p>
<?php } ?>
<strong>&copy; <?php echo date('Y'); ?> Canada Research Chair in Performance &amp; Culture, York University.</strong>
</div><!-- #footer -->


<?php
/*

Note:

- for west records $year
is replaced with:
$record->month $record->year

- for supplementary records
$year
is replaced with:
$this->title;

What about for news list / news details?
What about search page? Search results?
*/
?>