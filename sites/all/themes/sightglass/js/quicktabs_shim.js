(function ($) {
  
  $(document).ready(function() {

    // Inject a little shim into the quicktabs markup
    $('.quicktabs_main').before('<div class="shim" />');

    // Hide all open tabbed content and force all tabs
    // to display non-active state when viewing blog
    if ( $('.page-brewing-tools-and-goods').length || $('.page-blog').length || $('.page-blog-categories').length || $('.page-blog-archive').length || $('.node-type-article').length ) {
        $('.quicktabs-tabpage').addClass('quicktabs-hide');
        $('.quicktabs-tabs li').removeClass('active');
        if ( $('.page-blog-archive-all').length ) {
            
        }
    }

    // Add extra open/close functionality to quicktabs tabs
    // This code allows user to close tab on a second click if
    // that tab is currently open    
    if ( $('.page-brewing-tools-and-goods').length || $('.page-blog').length || $('.page-blog-categories').length || $('.page-blog-archive').length || $('.node-type-article').length ) {
        $('.quicktabs-tabs li a').click(function() {

            if ($(this).parent().hasClass('tab-open')) {
                $(this).parent().removeClass('active');
                $('.quicktabs-tabpage').addClass('quicktabs-hide');
                $('.quicktabs-tabs li').removeClass('tab-open');
            } else {
                $('.quicktabs-tabs li').removeClass('tab-open');
                $(this).parent().addClass('tab-open');
            }

        });
    }

    // Add subscriptions button to only non-subscription 
    // catalog pages 
    if ( $('.page-coffees-subscriptions').length == 0 ) {
        $('.product-catalog .quicktabs-wrapper .item-list').append('<a href="/subscriptions" class="button-subscriptions">Subscriptions</a>');
    }
    
    
    
  });
  
}(jQuery));