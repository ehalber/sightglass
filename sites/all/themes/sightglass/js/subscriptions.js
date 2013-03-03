(function ($) {
  
  $(document).ready(function() {

    var test = eh.getParameterByName('type');
    
    // If the is a subscription page, inject a class into
    // the body tag that will help with turn specific 
    // elements on and off
    if (test === 'subscription') {
    	$('body').addClass('subscription');
    }
    
    


  });
  
}(jQuery));