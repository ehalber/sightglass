(function ($) {
  
  $(document).ready(function() {

    // Create an array from the provided list of nids
    var detail_pager = [];
    /* -------------------------------------------------------------------------- */


    /*
    if (Drupal.settings.paintings.list) {
      list = Drupal.settings.paintings.list;
    } else if (Drupal.settings.objects.list){
      list = Drupal.settings.objects.list;
    }
    */
    var list = [];
    $('.view-display-id-block_10 .views-row').each(function(i) {
        list[i] = $(this).find('.views-field-nid span').text();
    });

    // get current node id
    var currentNodeId = Drupal.settings.node_lookup.nid;

    console.log(currentNodeId);

    // initialize variables
    var goBack;
    var goNext;
    var firstEl = list[0];
    var lastEl  = list[list.length - 1];

    // loop through work and assign back and next button correct url
    $.each(list, function(index, value) {
        if (value ==  currentNodeId) {

            // set goback value
            if (list[index - 1] === undefined) {
                goBack = lastEl;
            } else {
                goBack = list[index - 1];
            }

            // set go next value
            if (list[index + 1] === undefined) {
                    goNext = firstEl;
            } else {
                    goNext = list[index + 1];
            }

        }
    });

    // assign node locations to pager
    $('.pager-controls .goback').attr('href', '/node/' + goBack);
    $('.pager-controls .gonext').attr('href', '/node/' + goNext);

    //-----------------------------------------------------
    // END OBJECTS PAGER CONTROLS
    //-----------------------------------------------------
    
    
    
  });
  
}(jQuery));