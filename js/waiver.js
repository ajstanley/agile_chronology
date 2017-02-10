/**
 * sets the 'waived' cookie to 1 (true)
 * then opens the chronology in a new window
 */
function launchChronology(){
	/**
	 * notify users who have cookies disabled
	 */
	if (supportsCookies() === false) {
		alert('Because your browser does not support cookies or support has been disabled, you will not be able to use this website.');
		return false;
		}
		
		setCookie('waived', 1, null);
		
	if (navigator.userAgent.indexOf('Safari') != -1) {
		OpenWindow('frameset.php','newWin','left=0,top=0,location=no,toolbar=yes,resizable=no,scrollbars=no,menubar=no,statusbar=no,height=530,width=732'); 
		} else {
			OpenWindow('frameset.php','newWin','left=0,top=0,location=no,toolbar=yes,resizable=no,scrollbars=no,menubar=no,statusbar=no,height=500,width=732'); 
		}	
}

function OpenWindow(theURL,winName,features) { //v2.0
window.open(theURL,winName,features);
}