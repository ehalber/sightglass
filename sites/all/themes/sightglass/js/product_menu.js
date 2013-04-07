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

    // This is some js hackery to append a modified shopping
    // cart to the menu and then display different states of the
    // cart based on quantity
    $('.menu-bag').append($('.menu-cart .line-item-quantity-raw'));

    var menuText = $('.menu-bag').text();

    if ( $('.menu-bag').text() == 'Bag: ' ) {
        $('.menu-bag').removeClass('has-contents').text('Bag');

    } else {
        $('.menu-bag').addClass('has-contents');
    }

  });
  
}(jQuery));