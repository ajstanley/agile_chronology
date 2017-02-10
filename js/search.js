function submitform(){
    //alert('hey');
	if (formIsValid()){
		document.search_form.submit();
	}
}

function formIsValid() {
    // alert('hey');
    var errorstring ='';
	var errorcount = 0;
	// check that at least some text has been entered.
    if(!validQuery()) {
    	errorstring += "- Search query was empty.\n\n";    	
		errorcount++;
    }
    // check that years make sense
    if(!validYears()) {
    	errorstring += "- Search Dates must be between 1865 and 2005, or \n  one of 1600, 1700, 1800.\n\n"; // FIXME use constants
		errorcount++;
    }
    // check that at least one timeline is chosen.
    if(!validTimeline()) {
    	errorstring += "- At least one of the timelines must be chosen.\n\n";
		errorcount++;
    }

    if(!errorstring) return true;
	// some gramar
	plural = (errorcount>1) ? 's' : '';
	these = (errorcount>1) ? 'these' : 'this';

    msg = "The form was not submitted for the following reason"+plural+ ":\n";
    msg += "_________________________________________________\n\n";
    msg += errorstring;
    msg += "_________________________________________________\n\n";
    msg += "Please correct "+these+" error"+plural+ " and re-submit.\n";
    alert(msg);
    return false
}

function ensureOneTimeline() {
	//alert(document.search_form['timeline[can]'].checked);
    if(!validTimeline()) {
    	msg = "For the search to function properly, you must choose at\n";
		msg += "least one timeline.\n\n";
		//msg += "Please choose one, or both before submitting your search.\n";
		alert(msg);
        return false;
    }
}

function validYears() {
    if(validYearFirst() && validYearLast()){
        return true;
    }
    return false;
}

function validYearLast() {
    var l = document.search_form['dateLast'];
	if (l.value==1600 || l.value==1700 || l.value==1800)
	{
		return true;
	}
	if(l.value >= 1865 && l.value <= 2005) // FIXME use constants
	{
		return true;
	}
    if((l.value == l.defaultValue) || (l.value =='')) // if unchanged, ignore
	{ 
	return true;
    }
	return false;
}

function validYearFirst() {
    var l = document.search_form['dateFirst'];
	if (l.value==1600 || l.value==1700 || l.value==1800)
	{
		return true;
	}
	if(l.value >= 1865 && l.value <= 2005)  // FIXME use constants
	{
		return true;
	}
    if((l.value == l.defaultValue) || (l.value =='')) // if unchanged, ignore
	{ 
	return true;
    }
	return false;
}

function validTimeline() {
    var c = document.search_form['timeline[can]'];
    var w = document.search_form['timeline[west]']
    if(c.checked || w.checked) {
        return true;
    }
    return false;
}

function validQuery() {
    var q = document.search_form['query[1][string]'];
    if(q.value) {
	    return true;
    }
    return false;
}

function toggle(id){
	if (document.getElementById){
		element = document.getElementById(id);
	} else if(document.all){
		element = document.all[id];
	}
	element.click();
}
