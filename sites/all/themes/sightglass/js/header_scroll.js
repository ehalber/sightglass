(function ($) {
  
  $(document).ready(function() {

    // Once the logo passes underneath the nav bar
    // show the sightglass wave
    
    var winTop = '';

    $(document).bind('load, scroll', function() {
        winTop = $(window).scrollTop();
        if (winTop >= 154) {
            $('.separator').addClass('wave');
        } else {
            $('.separator').removeClass('wave');
        }
    });

    // Scroll the document back to top when the 
    // user clicks on this element
    $('.menu .separator').click(function() {
        if (winTop >= 154) {
            $('html,body').animate({scrollTop: 0 }, 'fast', 'easeOutQuad');
        }
    });

  });
  
}(jQuery));