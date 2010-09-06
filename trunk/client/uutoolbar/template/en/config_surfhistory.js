//ultra protocol can not use XMLHTTPREQUEST!!

function SurfHistory(id){
	var content="\
		     <table valign=top width=100% border=0><tr>\
		     <td>\
		     <img src='ultra://surfhistory.png' />\
		     <td>\
		     <h3>Surf History Setting</h3>\
		     <hr>\
		     Surf History can record all the websites you have accessed. You can retrieve the url very easily.\
		     </table>\
		     \
		     \
		     \
		     <form name='form' method='post'>\
		     <!-- surfhistory -->\
		     <hr>\
		     <input type='button' value='Confirm' onClick=confirm_httpproxy()>\
		     </form>\
		     ";
	document.getElementById(id).innerHTML=content;
}
