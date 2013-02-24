(function ($) {
  
  $(document).ready(function() {

    // Search through the menu items and pull out the one that 
    // matches the page title    
    
    $('.block-menu-menu-products .content a').each(function(i) {
        if ( $(this).text() === $('h1.title').text() ) {
            $(this).remove();
        }
    });

    $('h1.title span').click(function() {
        $('.block-menu-menu-products .content').slideToggle();
    });

  });
  
}(jQuery));