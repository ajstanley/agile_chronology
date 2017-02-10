<?php

function getMonth($m) {
	$month_array = array('1'=>'January','2'=>'February','3'=>'March','4'=>'April','5'=>'May','6'=>'June','7'=>'July','8'=>'August','9'=>'September','10'=>'October','11'=>'November','12'=>'December');
  return $month_array[$m];
}

function getCategoryArray() {
	$category_array = array('0'=>'Art &amp; Architecture','1'=>'Politics','2'=>'Science &amp; Engineering','3'=>'Society &amp; Culture');
  return $category_array;
}

function getIconArray() {
	$category_array = array('0'=>'Art &amp; Architecture','1'=>'Politics','2'=>'Science &amp; Engineering','3'=>'Society &amp; Culture');
  return $category_array;
}

function getPrevNext($year) {
  $module_path = drupal_get_path('module', 'agile_chronology');

  $prev='';
	$next='';
	$py = intval($year)-5;
  if ($py >= EARLIEST_NEWS) {
		 $prev = "<a href=\"/agile/newslist/$py\" onmouseover=\"changeImages('back_g', '/$module_path/imgs/g_left_ovr.gif'); return true;\" ";
		 $prev .= "onmouseout=\"changeImages('back_g', '/$module_path/imgs/g_left.gif'); return true;\" title=\"previous entry\">";
		 $prev .= "<img name=\"back_g\" src=\"/$module_path/imgs/g_left.gif\" alt=\"previous entry\" width=\"18\" height=\"17\" border=\"0\" /></a>";
	}	
	$ny = intval($year)+5;
	if ($ny <= getLatestYear()) {
	$next = "<a href=\"/agile/newslist/$ny\" onmouseover=\"changeImages('fwd_g', '/$module_path/imgs/g_right_ovr.gif'); return true;\" ";
	$next .= "onmouseout=\"changeImages('fwd_g', '/$module_path/imgs/g_right.gif'); return true;\" title=\"next entry\">";
	$next .= "<img name=\"fwd_g\" src=\"/$module_path/imgs/g_right.gif\" alt=\"next entry\" width=\"18\" height=\"17\" border=\"0\" /></a>";
	}
	return array ($prev,$next);
}

function half_decade($year) {
	$year = (int) $year;
	$startdate = "$year-01-01";
	$endyear = $year + 4;
	$enddate = "$endyear-12-31";
	return array ($startdate,$enddate);
}

/***
 * returns an array of all the news entries for a given half-decade
 * $year represents the first year in the half-decade
 */
function getNewsArray($year) {
	// array[newsID][EN|FR] =  newsBody values.
	// year MUST be a proper year(eg. 1998)
	//enter_function('getNewsArray');
	$year = (int) $year; // force year to be an integer
  
	$startdate='';
  $enddate='';
	list($startdate,$enddate) = half_decade($year);
	
	$news = array(); // start with an empty array		
	
	// check that year is within bounds

	$query="SELECT news_id, category_id, title_en, title_fr, body_en, body_fr, alt_en, alt_fr, date_reported from ".NEWS_TABLE." WHERE date_reported >= '$startdate' AND date_reported <= '$enddate' ORDER BY category_id, date_reported";
	$results = db_query($query)->fetchAllAssoc('category_id');
	/**
	 * if no rows returned, quit early
	 */
	if (!count($results)){
		trace('nothing found');
		//exit_function();
		return $news;
	}
	$newsUnordered = array();

	foreach($results as $row){ 
		// trace('for each row returned.');
		$thisID = $row->news_id;
		$clean_row = array('CAT'=>$row->category_id,'T_EN'=>stripslashes($row->title_en),'T_FR'=>stripslashes($row->title_fr),'EN'=>stripslashes($row->body_en),'FR'=>stripslashes($row->body_fr),'A_EN'=>stripslashes($row->alt_en),'A_FR'=>stripslashes($row->alt_fr),'id'=>$thisID,'dr'=>stripslashes($row->date_reported));
		$newsUnordered[] = $clean_row;
	}

	ksort($news);

	// now get rid of those terrible indexes due to $i=1000 above
	foreach($newsUnordered as $value){
		$news[] = $value;
	}

	/**/

	//exit_function();
	return $news;
}//  getNewsArray DONE
/////////////////////////////////////////////

function getCategoryHeader($year,$heading,$icon,$count) {
		return "<li class=\"$icon\">\n<h2>$heading</h2>";
}

function getNewsList($year) {
	// returns HTML code to be used in the display form
	// <li>news body(EN+FR) <a>delete</a></li>
	
	$cArray = getNewsArray($year);

	$cur_category_id = 0;
	$cur_category_count = 0;
	$prev_category_id = 0;
	//$category_array = array('0'=>'Art &amp; Architecture','1'=>'Politics','2'=>'Science &amp; Engineering','3'=>'Society &amp; Culture');
	$category_array = getCategoryArray();
	$count_array = array();
	$icon = '';
	$tmp = '';
	$numberOfRows = sizeof($cArray);
	$i=1;
	for ($cat_id=0;$cat_id<=3;$cat_id++) {
			$count_array[$cat_id] = 0;
	}
	if (!empty($cArray)) {
		$tmp = "<ul id=\"news_list\">"; // table begins

		foreach($cArray as $value) {
		  $count_array[$value['CAT']]++;
		}

		for ($cat_id=0;$cat_id<=3;$cat_id++) {
		
		  $icon = strtolower(substr($category_array[$cat_id],0,3));
		  $tmp .= getCategoryHeader($year,$category_array[$cat_id],$icon,$count_array[$cat_id]);

			if ($count_array[$cat_id] == 0) {
			  $tmp .= "<p>No news available for this category</p>";
			}
			else {
		
		    foreach($cArray as $value) {
			    $cur_category_id = $value['CAT'];
		      // if category change, then include row for category heading
		      if ($cur_category_id == $cat_id) {
			      $cy = intval(substr($value['dr'],0,4));
			      $cm = getMonth(intval(substr($value['dr'],5,2)));
			      $cd = intval(substr($value['dr'],8,2));
			      //$tm = mktime(0, 0, 0, $cm, $cd, $cy);
			      $tmp .= "<h3><a href=\"news.detail.php?year=$year&cat=$icon\">".$cm." ".$cd.", ".$cy;
			      //$tmp .= date("F j, Y",$tm);
			      //$tmp .= date("r",$tm);
			      $tmp .= " - ".$value['T_EN']."</a></h3>"; 
			      $cur_category_count++;
			      $i++;
			    }
		    }
			}
 		  $tmp .= "</li>";
		
		}
		
	} elseif($year >= EARLIEST_NEWS) { // excludes 1600, 1700, 1800
		//$tmp = "<table border=1>"; // table begins
		for ($cat_id=0;$cat_id<=3;$cat_id++) {
    		$icon = strtolower(substr($category_array[$cat_id],0,3));
    		$tmp .= getCategoryHeader($year,$category_array[$cat_id],$icon,$count_array[$cat_id]);
			  $tmp .= "<p>No news available for this category</p>";
		}
		$tmp .= "</li>"; 
	} else {
		$tmp = '';
	}
	return $tmp;

}
/////////////////////////////////////////////

function getNewsDetail($year,$category) {
	$cArray = getNewsArray($year);

	$cur_category_id = 0;
	$cur_category_count = 0;
	$prev_category_id = 0;
	//$category_array = array('0'=>'Art &amp; Architecture','1'=>'Politics','2'=>'Science &amp; Engineering','3'=>'Society &amp; Culture');
	$category_array = getCategoryArray();
	$icon = '';
	$tmp = '';
	$numberOfRows = sizeof($cArray);
	$i=1;
	if (!empty($cArray)) {
		//$tmp = "<table border=1>"; // table begins

		$icon = strtolower(substr($category_array['0'],0,3));
		$tmp = getCategoryHeader($year,$category_array['0'],$icon,$count_array['0']);
		
		foreach($cArray as $value) {
			$cur_category_id = $value['CAT'];
		  // if category change, then include row for category heading
		  if ($cur_category_id != $prev_category_id) {
			  if ($cur_category_count == 0) {
				  $tmp .= "<br />No news available for this category";
				}
    		$tmp .= "</td></tr>";

    		$icon = strtolower(substr($category_array[$cur_category_id],0,3));
    		$tmp .= getCategoryHeader($year,$category_array[$cur_category_id],$icon,$count_array[$cur_category_id]);

				$prev_category_id = $cur_category_id;
				$cur_category_count = 0;
			}
			$cy = intval(substr($value['dr'],0,4));
			$cm = getMonth(intval(substr($value['dr'],5,2)));
			$cd = intval(substr($value['dr'],8,2));
			//$tm = mktime(0, 0, 0, $cm, $cd, $cy);
			$tmp .= "<br /><a href=\"news.detail.php?year=$year&cat=$icon\">".$cm." ".$cd.", ".$cy;
			//$tmp .= date("F j, Y",$tm);
			//$tmp .= date("r",$tm);
			$tmp .= " - ".$value['T_EN']."</a>"; 
			$cur_category_count++;
			$i++;
		}
		for ($cat_id=$cur_category_id+1;$cat_id<=3;$cat_id++) {

    		$icon = strtolower(substr($category_array[$cat_id],0,3));
    		$tmp .= getCategoryHeader($year,$category_array[$cat_id],$icon,$count_array[$cat_id]);
			  $tmp .= "<br />No news available for this category";
		}
 		$tmp .= "</td></tr>";
		
	} elseif($year > EARLIEST) { // excludes 1600, 1700, 1800
		$tmp = "<table border=1>"; // table begins
		for ($cat_id=0;$cat_id<=3;$cat_id++) {
    		$tmp .= "\n<tr bgcolor='#DDDDFF'><td colspan='5'>".$category_array[$cat_id]."</td></tr>";
			  $tmp .= "\n<tr><td colspan='5' align='center'>No news available for this category</td></tr>";
		}
		$tmp .= "</table>\n"; // close table
	} else {
		$tmp = '';
	}
	return $tmp;

}


function getNewsLink($year) {
	$tmp = '';
	$rem = $year % 5;
	$start = $year - $rem;
	$end = $start+4;
		
	list($startdate,$enddate) = half_decade($start);
	
	$query="SELECT news_id FROM ".NEWS_TABLE." WHERE date_reported >= '$startdate' AND date_reported <= '$enddate';";
	$results = db_query($query)->fetchAll();
	if (!$results){
		 $tmp='';
	}	else {
		 $tmp="<a href=\"/agile/newslist/$start\" title=\"Expanded news: $start-$end\" class=\"tagline\">Expanded news: $start-$end</a>";  
	}
	
	return $tmp;
}

function getNewsArticle($year,$category) {
	$cArray = getNewsArray($year);
	$category_array = getCategoryArray();
	$icon = '';
	$tmp = '';
	$numberOfRows = sizeof($cArray);
	$i=1;
	if (!empty($cArray)) {

		//$icon = strtolower(substr($category_array['0'],0,3));
		//$tmp = getCategoryHeader($year,$category_array['0'],$icon);
		
		foreach($cArray as $value) {
			$cur_category_id = $value['CAT'];
   		$icon = strtolower(substr($category_array[$cur_category_id],0,3));
		
		  if ($icon == $category) {
				
				if ($i==1)
					 $tmp .= getFirstArticle($value,$category_array[$cur_category_id],$icon);
				else
					 $tmp .= getSubsequentArticle($value,$i);
				
					 
  			//$tmp .= "<br /><a href=\"news.detail.php?year=$year&cat=$icon\">".$cm." ".$cd.", ".$cy;
  			//$tmp .= " - ".$value['T_EN']."</a>";
				
				$i++;
			} 
		}
		
	}
	return $tmp;

}


function getFirstArticle($value,$category,$icon) {
	$cy = intval(substr($value['dr'],0,4));
	$cm = getMonth(intval(substr($value['dr'],5,2)));
	$cd = intval(substr($value['dr'],8,2));
	$html = "<h2 class=\"".$icon."\">".$category."<br />$cm $cd, $cy - ".$value['T_EN']."</h2>";
	
	$obj = null;
	$obj = getNewsImage($value['id']);
	
	if ($obj != null)
	  $html .= "<img src=\"{$obj->_fileUrl}\" alt=\"".$value['A_EN']."\" title=\"".$value['A_EN']."\" {$obj->_imageInfo[3]} hspace=\"6\" vspace=\"6\" border=\"2\" class=\"float_right\" align=\"right\" />";
	
	$html .= $value['EN'];
	return $html;
}


function getSubsequentArticle($value,$i) {
	$cy = intval(substr($value['dr'],0,4));
	$cm = getMonth(intval(substr($value['dr'],5,2)));
	$cd = intval(substr($value['dr'],8,2));

  $html = "<h2><a id=\"headline$i\" name=\"headline$i\"></a>";
  $html .= "$cm $cd, $cy - ".$value['T_EN']."</h2>";

	$obj = null;
	$obj = getNewsImage($value['id']);
	
	if ($obj != null)
    $html .= "<img src=\"{$obj->_fileUrl}\" alt=\"".$value['A_EN']."\" title=\"".$value['A_EN']."\" {$obj->_imageInfo[3]} hspace=\"6\" vspace=\"6\" border=\"2\" class=\"float_right\" align=\"right\" />";
		
	$html .= $value['EN'];
	return $html;

}


function getCategoryLinks($year,$category) {
	$category_array = getCategoryArray();
	$icon = '';
	$tmp = '<ul id="exp-news-links">';
	$content = '';
	
	$link_index = 0;
	$link_array = array();
		
  	for ($cat_id=0;$cat_id<=3;$cat_id++) {
		$icon = strtolower(substr($category_array[$cat_id],0,3));
	    if ($icon != $category) {
						
			$content = '';
			$content = contentExists($year,$cat_id);
			 
			 
			if ($content) { 	 
				$tmp .= '<li class="'. $icon . '"><h2>';
				$tmp .= "<a href=\"news.detail.php?year=$year&cat=$icon\" title=\"".$category_array[$cat_id]."\" >".$category_array[$cat_id] . '</a>';
				$tmp .= "</h2></li>";
			}
			else {
				# no link
				$tmp .= '<li class="'. $icon . '"><h2>';
				$tmp .= "<span title=\"".$category_array[$cat_id]."\" >".$category_array[$cat_id] . '</span>';
				$tmp .= "</h2></li>";
			}
			$link_index++;			   		
	  	} 
	}
	$tmp .= '</ul>';
	
	return $tmp;
}



function addNewsStagingPath($imageName) {
	Return SERVER_PATH_TO_IMAGES.'/news/'.$imageName;
}

function _addWebServerPath($imageName) {
	Return TRANSLATED_PATH_TO_IMAGES.'/news/'.$imageName;
}

	
function getNewsImage($id){
    // check filesys for image.
    // traceVar('image exists?', $this->imageExists()  );
    if (newsImageExists($id)) {
            $obj = new stdClass();
            $obj->_fileLocal = addNewsStagingPath($id).'.jpg';
            $obj->_fileUrl = _addWebServerPath($id).'.jpg';
            $obj->_imageInfo = GetImageSize($obj->_fileLocal);
            trace('returning Image Object');
            return $obj;
    } else {
            //trace('not returning anything');
            return false;
    }
}

 
function newsImageExists($id) {
        //echo('<p>'.addNewsStagingPath($id).'</p>');
        $fileExists =  file_exists(addNewsStagingPath($id).'.jpg');
        traceVar("{$id} exists",$fileExists ? 'TRUE' : 'FALSE');
        return (bool)$fileExists;
}

function contentExists($year,$category) {
	$year = (int) $year; // force year to be an integer
  
	$startdate='';
  $enddate='';
	list($startdate,$enddate) = half_decade($year);

	$query="SELECT news_id from ".NEWS_TABLE." WHERE date_reported >= '$startdate' AND date_reported <= '$enddate' AND category_id = '$category';";
	$results = db_query($query);
	/**
	 * if no rows returned, quit early
	 */
	if (!mysqli_num_rows($results)){
		trace('nothing found');
		//exit_function();
		return false;
	}
	
	return true;
}//  contentExists DONE
/////////////////////////////////////////////
?>
