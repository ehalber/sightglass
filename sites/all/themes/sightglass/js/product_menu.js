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

    // Inject the shopping cart link into the menu
    //$('.block-menu-menu-top-right .menu li:nth-child(2)').after('<li class="leaf">hello</li>');


  });
  
}(jQuery));