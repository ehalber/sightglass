(function ($) {
  
    var logoCSS = {
        width: 130,
        height: 130,
        background: 'url(sites/all/themes/sightglass/images/logo.png) -520px 0',
        margin: '30px auto 0 auto',
        position: 'fixed'

    }

  $(document).ready(function() {

    // If window height is greater than document height then 
    // make the footer fixed
    
    $('.page-blog .views-field-field-image a').each(function(i,val) {
        $(this).attr('href', $(this).find('img').attr('src'));
        var string = $(this).attr('href');
        $(this).attr('href', string.replace(/blog_hero/gi, 'lightbox_display'));  
        $(this).attr('title', $(this).find('img').attr('title')); 
    });

    $('.fancybox').fancybox({
        transitionIn: 'fade',
        transitionOut: 'fade',
        easeIn: 'easeInOutQuint',
        easeOut: 'easeInOutQuint',
        speedIn: 250,
        speedOut: 400,
        overlayOpacity: 1,
        overlayColor: '#473000',
        scrolling: 'no',
        padding: 3,
        titleFormat: function() {
            return '<div class="image-title">' + $(this).attr('title') + '</div><div class="author">Photo by Jerad Jerad</div>';
        },
        titlePosition: 'outside',
        onStart: function() {
            $('#fancybox-overlay').append('<div class="logo" />');
            $('.logo').css(logoCSS);
            $('.logo').css('left', $(window).width()/2 - 65);
        },
        onComplete: function() {
            $('body').css('overflow-y', 'hidden');
        },
        onClosed: function() {
            $('.logo').remove();
            $('body').css('overflow-y', 'scroll');
        }
    });



    
  });
  
}(jQuery));