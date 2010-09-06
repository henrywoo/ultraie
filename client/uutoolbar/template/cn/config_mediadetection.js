//ultra protocol can not use XMLHTTPREQUEST!!

function MediaDetection(id){
	var content="\
		     <table valign=top width=100% border=0><tr>\
		     <td>\
		     <img src='ultra://settings.png' />\
		     <td>\
		     <h3>Media Detection Setting</h3>\
		     <hr>\
		     Media detection can detect the url of media resources when you are surfing on internet.\
		     </table>\
		     \
		     \
		     \
		     </div>\
		     Audio/Vedio media files:\
		     <hr>\
		     <form name='form_mediadetection' method='post'>\
		     <!-- MediaDetection -->\
		     ";
	document.getElementById(id).innerHTML=content;
}
