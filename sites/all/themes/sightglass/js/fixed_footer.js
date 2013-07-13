(function ($) {
  
  $(document).ready(function() {

    // If window height is greater than document height then 
    // make the footer fixed
    
    var fixedOptions = {
        position: 'fixed',
        bottom:   0,
        width: 1024
    } 

    var normalOptions = {
        position: 'relative',
        bottom:   'auto'
    }
    
    $(window).bind('resize, load', function() {
        return;
    	docHeight = $(document).height();
    	winHeight = $(window).height();
    	if (winHeight >= docHeight) {
	      $('.section-footer').css(fixedOptions);
	    } else {
	      $('.section-footer').css(normalOptions);
	    }
    });
    
  });
  
}(jQuery));