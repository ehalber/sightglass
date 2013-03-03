// This is my own utility javascript functions file
// I mainly use this for function I often find
// myself using over and over and over again

   
var eh = {
	/**
	 * Returns the value of the query string id
	 * @param  {string} key
	 * @return {string} value
	 */
	getParameterByName: function(name) {
		name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
		var regexS = "[\\?&]"+name+"=([^&#]*)";
		var regex = new RegExp( regexS );
		var results = regex.exec( window.location.href );
		if( results == null )
			return "";
		else
		return decodeURIComponent(results[1].replace(/\+/g, " "));
	}
};






