(function ($) {
  
  $(document).ready(function() {

    var test = eh.getParameterByName('type');
    
    // If the is a subscription page, inject a class into
    // the body tag that will help with turn specific 
    // elements on and off
    if (test === 'subscription') {
    	$('body').addClass('subscription');
    }

    /*
      On the homepage, override the href of certain items.
      This was put into place to make it easy for content
      creators to create a piece of content that can appear
      on the homepage, but does not exist anywhere else on 
      the site.
     */
    
    $('.view-front-page-gallery .views-row').each(function() {
        if ( $(this).find('.views-field-nid .hide').text() == 168 ) {
            $(this).find('a').attr('href', '/subscriptions');
        }

    });
    
    // If there is an out of stock item, hide
    // the quantity field
    // 
    
    /*
      On the homepage we need to hack up one of the slides
      to automatically anchor down and show the about us
      video. In order to do this, we're going to do a bit of 
      DOM work to add an anchor to the hyperlink. This is assuming
      the video we want to link to is the first slide in the
      sequence. Both the image and inline text are updated with
      this manipulation.
     */
    if ($('.front').length) {
      var videoURL = $('.view-front-slideshow .views-row-1').find('a').attr('href');
      $('.view-front-slideshow .views-row-1').find('a').attr('href', videoURL + '#intromovie');
    }
    
    

  });
  
}(jQuery));