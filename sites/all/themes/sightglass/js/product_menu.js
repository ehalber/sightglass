(function ($) {
  
  $(document).ready(function() {

    
    
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