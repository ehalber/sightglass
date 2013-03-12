(function ($) {
  
  $(document).ready(function() {

    // Search through the menu items and pull out the one that 
    // matches the page title    
    
    $('.block-menu-menu-products .content a, .block-menu-menu-brewing-guides .content a').each(function(i) {
        if ( $(this).text() === $('h1.title').text() ) {
            $(this).remove();
        }
    });

    $('h1.title span').click(function() {
        $('.block-menu-menu-products .content').slideToggle();
        $('.block-menu-menu-brewing-guides .content').slideToggle();
    });


  });
  
}(jQuery));