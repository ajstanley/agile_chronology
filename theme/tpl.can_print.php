<?php
////// tpl.can_print.php
$p = $path['root'].'style/style_print.css';

echo <<<EOT

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
        "http://www.w3.org/TR/1999/REC-html401-19991224/loose.dtd">
<html>
<head>
<title>Canadian Theatre: A Critical Timeline :: $year</title>
<link rel="Stylesheet" href="$p" type="text/css" />
</head>
<body bgcolor="#ffffff" link="#000000" alink="#660000" vlink="#660000">
<table border="0" cellpadding="4" cellspacing="4" width="99%" bgcolor="#ffffff">
<tr>
<td colspan="2" align="center"><span class="heading">Canadian Theatre: A Critical Timeline</span></td>
</tr>
<tr>
<td align="center" class="heading">$year</td>
</tr>
<tr>
<td colspan="2">


$record->body


</td>
</tr>
<tr>
<td><hr align="center" size="1" width="70%" /><p style="text-align: left;">MLA citation for this chronology:<br />$authorship "$year." <em>Canadian Theatre: A Critical Timeline</em> in <em>Modern Theatre in Context</em>. Christopher Innes, ed. Mar. 2003-. Canada Research Chair in Performance and Culture. [$dateString]. 
&lsaquo;$thisPage&rsaquo;.</p>
<p>Copyright: Canada Research Chair in Performance and Culture, York University.</p>
</td>
</tr>
</table>
</body>
</html>

EOT;
?>
