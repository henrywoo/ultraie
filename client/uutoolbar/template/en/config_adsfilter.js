//ultra protocol can not use XMLHTTPREQUEST!!

function AdsFilter(id){
	var content="\
		     <table valign=top width=100% border=0><tr>\
		     <td>\
		     <img src='ultra://adsfilter.png' />\
		     <td>\
		     <h3>Ads Filter Setting</h3>\
		     <hr>\
		     Media detection can detect the url of media resources when you are surfing on internet.\
		     </table>\
		     \
		     \
		     \
		     <form name='form' method='post'>\
		     <!-- adsfilter -->\
		     <hr>\
		     <input type='button' value='Confirm' onClick=confirm_httpproxy()>\
		     </form>\
		     ";
	document.getElementById(id).innerHTML=content;
}
