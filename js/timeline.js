// functions used by the graduated timeline in the top frame.
var base = "/sites/all/modules/agile_chronology/imgs/";
function swap_on_timeline(year){ // show the on(red) bars in the timeline

    if (self.loaded != true){
		return;
    }
    // first check to see if its one of the years on top (1865 + twenties)
    if (year%20==0){
        label = 'id'+year;
        //alert(document[label].src):
        document[label].src = base + 'tl/'+year+'_ovr.gif';
    }
    bar = 'b_'+year;
    if (document['b_'+year] && document['b_'+year].src){
        if (year%10==0){ // decade
              document['b_'+year].src = tens_on.src;
        } else if(year%5==0){ // fiver 
              document['b_'+year].src = fives_on.src; 
        } else { // all others 
              document['b_'+year].src = ones_on.src;
        }
    }
    showdate(year);
}

function swap_off_timeline(year){ // show the off(black) bars in the timeline
    // first check to see if its one of the years on top (1865 + twenties)
    
    if (year%20==0){
        label = 'id'+year;
        //alert(document[label].src);
        document[label].src = base +'tl/'+year+'.gif';
    }
    bar = 'b_'+year;
    if (document['b_'+year]){
        if (year%10==0){ // decade 
             document['b_'+year].src = base + 'tl/xf.gif'; 
        } else if(year%5==0){ // fiver 
             document['b_'+year].src = base +'tl/vf.gif'; 
        } else { // all others 
             document['b_'+year].src = base +'tl/if.gif'; 
        } 
    }
}


function showdate(year){// change display of current year.
	if(!year){
		//alert('year not set.');
		return;
	}
    if (self.loaded==true){
        // setting the date now. 
        year = year + ''; // ensure we treat year as a string.
        millenium = year.charAt(0);
        century = year.charAt(1);
        decade = year.charAt(2);
        annum = year.charAt(3);
        //alert('showdate year:' + year);
        document.images.year_mil.src = base + 'tl/numbers/numbers_'+timeline+'_0'+millenium+'.gif';
        document.images.year_cent.src = base + 'tl/numbers/numbers_'+timeline+'_0'+century+'.gif';
        document.images.year_dec.src = base + 'tl/numbers/numbers_'+timeline+'_0'+decade+'.gif';
        document.images.year_ann.src = base + 'tl/numbers/numbers_'+timeline+'_0'+annum+'.gif';
		showdecadePic(year);
    } else { 
		//alert('waiting, year = ' + year);
        // waiting for top frame. 
        self.setTimeout("showdate("+year+")",203); // check again in .2 seconds    
    }
}
function showSpecialDate(century){
    if (self.loaded==true){
		document.images.year_mil.src = base + 'tl/numbers/'+ timeline + '_'+ century + '_1.gif';
		document.images.year_cent.src = base + 'tl/numbers/'+ timeline + '_'+ century + '_2.gif';
		document.images.year_dec.src = base + 'tl/numbers/'+ timeline + '_'+ century + '_3.gif';
		document.images.year_ann.src = base + 'tl/numbers/'+ timeline + '_'+ century + '_4.gif';
		showdecadePic(century+'00');
    } else { 
		//alert('waiting, year = ' + year);
        // waiting for top frame. 
        self.setTimeout("showSpecialDate("+century+")",204); // check again in .2 seconds    
    }
}

/**
 * load thumbs for decade preview
 */
function showdecadePic(year){
    year = year +'';
    decade = year.substring(0,3)+"0"; // eg. 1923 -> 1920
    decade_fileName = base + 'decade_images/decade_'+decade+'.jpg'
    window.document.images.decade_img.src = decade_fileName;  
}
