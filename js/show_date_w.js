function showdate(year){
	// change display of current year.
	millenium = year.charAt(0);
	century = year.charAt(1);
	decade = year.charAt(2);
	annum = year.charAt(3);

	document.images.year_mil.src = 'numbers/numbers_w_0'+millenium+'.gif';	
	document.images.year_cent.src = 'numbers/numbers_w_0'+century+'.gif';
	document.images.year_dec.src = 'numbers/numbers_w_0'+decade+'.gif';
	document.images.year_ann.src = 'numbers/numbers_w_0'+annum+'.gif';	
}