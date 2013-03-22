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

  });
  
}(jQuery));