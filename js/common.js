	// generic rollover script

function newImage(arg) {
	if (document.images) {
		rslt = new Image();
		rslt.src = arg;
		return rslt;
	}
}

function changeImages() {
	if (document.images && (preloadFlag == true)) {
		for (var i=0; i<changeImages.arguments.length; i+=2) {
			document[changeImages.arguments[i]].src = changeImages.arguments[i+1];
		}
	}
}

// preloading of common image resources here only
// template-specific resources should be isolated to those templates


// preload decade images in leftnav

function init(){
	if (document.images) { 
		d1600 = newImage("imgs/decade_images/decade_1600.jpg");
		d1700 = newImage("imgs/decade_images/decade_1700.jpg");
		d1800 = newImage("imgs/decade_images/decade_1800.jpg");
		d1860 = newImage("imgs/decade_images/decade_1860.jpg");
		d1870 = newImage("imgs/decade_images/decade_1870.jpg");
		d1880 = newImage("imgs/decade_images/decade_1880.jpg");
		d1890 = newImage("imgs/decade_images/decade_1890.jpg");
		d1900 = newImage("imgs/decade_images/decade_1900.jpg");
		d1910 = newImage("imgs/decade_images/decade_1910.jpg");
		d1920 = newImage("imgs/decade_images/decade_1920.jpg");
		d1930 = newImage("imgs/decade_images/decade_1930.jpg");
		d1940 = newImage("imgs/decade_images/decade_1940.jpg");
		d1950 = newImage("imgs/decade_images/decade_1950.jpg");
		d1960 = newImage("imgs/decade_images/decade_1960.jpg");
		d1970 = newImage("imgs/decade_images/decade_1970.jpg");
		d1980 = newImage("imgs/decade_images/decade_1980.jpg");
		d1990 = newImage("imgs/decade_images/decade_1990.jpg");
		d2000 = newImage("imgs/decade_images/decade_2000.jpg");
		d2020 = newImage("imgs/decade_images/decade_2010.jpg");
		preloadFlag = true;
	}
	loaded=true;
}

// functions for search form

function fieldFocus(formElement) {
	if(formElement.value == formElement.defaultValue){
		formElement.value = '';
	}
}
function fieldBlur(formElement) {
	if(formElement.value == ''){
		formElement.value = formElement.defaultValue;
	}
}
function checkSearchField(f) {
	e = f['query[1][string]'];
	if((e.value == e.defaultValue) || (e.value == '')){
		alert('Please enter a word or phrase to search for.');
		e.focus();
		return false;
	}
	return true;
}

// http://youmightnotneedjquery.com/#ready
function ready(fn) {
  if (document.readyState != 'loading'){
    fn();
  } else if (document.addEventListener) {
    document.addEventListener('DOMContentLoaded', fn);
  } else {
    document.attachEvent('onreadystatechange', function() {
      if (document.readyState != 'loading')
        fn();
    });
  }
}
