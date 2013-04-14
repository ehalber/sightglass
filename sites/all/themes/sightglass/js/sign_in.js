(function ($) {
  
  $(document).ready(function() {
 
    // I'm using the live method here as the the menus
    // are appended to the DOM are done so after the 
    // DOM is loaded and do not have events attached
    // to them.
    $('.pane-user-login input').live('focus', function() {
      if ( $(this).val().length ) {
        return;
      } else {
        $(this).prev().fadeOut();
      }
      
    });	

    $('.pane-user-login input').live('blur', function() {
      if ( $(this).val().length ) {
        return;
      } else {
        $(this).prev().fadeIn();
      }
      
    }); 

    // Replace the default loaded cart with one that
    // actually works. I will never use the minipanels
    // menu plugin again, it's just a huge pain in 
    // the fucking ass
    //$('.menu-minipanel-1382').html('hello there fucker');
    //
    

    /* 
      Checkout Review - On this page, client wanted to include 
      some additional links on the confirmation page to make 
      it easy to return to the shipping/billing page. I would 
      have liked to to this with retheming the checkout pane
      but I don't have the time to dive in deeply and googling
      isn't turning up anything easy to follow. Javascript
      to the rescue!!

      The first important piece here is to pass the order id from
      the url (php to js). Once this is done, it's easy to plug
      it in and append the anchor links.
    */
   
    if (Drupal.settings.site_info.order_id) {
      var order_id = Drupal.settings.site_info.order_id;
    }

    $('.checkout-review .pane-data:eq(2) .field-item').append('<div class=""><a href="/checkout/'+ order_id +'">Changing Billing Address</a></div>');
    $('.checkout-review .pane-data:eq(3) .field-item').append('<div class=""><a href="/checkout/'+ order_id +'">Changing Shipping Address</a></div>');





  });
  
}(jQuery));