(function ($) {
  
  $(document).ready(function() {

    // -- On location page, move headers below maps
    
    if ( $('.page-our-company').length ) {
        $('.location-box').each(function(i) {
            $(this).find('.pane-content').append($(this).find('.pane-title'));
        });
    }
    
    // -- On featured partner detail page, close up tabs
    
    if ( $('.node-type-featured-partner').length ) {
        $('.quicktabs-wrapper li.active').removeClass('active');
        $('.quicktabs-tabpage').addClass('quicktabs-hide');
    }

    // -- On inquiry form, move the description underneath the label
    
    if ( $('.page-wholesale').length ) {
        $('.webform-component-textarea').each(function(i) {
            $(this).find('label').append($(this).find('.description'));
        });

        $('#webform-component-currently-owned-equipment .description').append('<div class="description">Please indicate the make / model.</div>');
    }

    // -- On resources page, added vertical divider
    if ( $('.page-resources').length ) {
        $('.view-brewing-guides-home .views-row:nth-child(2n-1)').append('<div class="divider" />')
    }

    if ( $('.node-type-brewing-guide').length ) {
        $('.related-resources .views-row:nth-child(2n-1)').append('<div class="divider" />')
    }



  });
  
}(jQuery));