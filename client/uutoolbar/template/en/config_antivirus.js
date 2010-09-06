//ultra protocol can not use XMLHTTPREQUEST!!

function AntiVirus(id){
	var content="\
		     <table valign=top width=100% border=0><tr>\
		     <td>\
		     <img src='ultra://antivirus.png' />\
		     <td>\
		     <h3>AntiVirus</h3>\
		     <hr>\
		     Protect your computer when surfing on internet.\
		     </table>\
		     \
		     \
		     \
		     <form name='form' method='post'>\
		     <!-- antivirus -->\
		     <hr>\
		     <input type='button' value='Confirm' onClick=confirm_antivirus()>\
		     </form>\
		     ";
	document.getElementById(id).innerHTML=content;
}
