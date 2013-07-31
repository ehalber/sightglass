/**
 * handling js_link class
 */
function javascriptLinks(){
  jQuery('a.js_link').click(function() {
  	var answer = confirm('Are you sure you want to perform this operation?');
    if (answer) {
  	  return true;
    } else {
  	  return false;
    }
  });
}

jQuery(document).ready(function(){
  javascriptLinks();
});