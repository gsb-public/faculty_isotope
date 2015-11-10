<div class="isotope-item views-row <?php print $class; ?>" data-nid="<?php print $nid; ?>">
  <div class="node-faculty">
    <div class="field-name-field-image-single-public">
      <?php print theme_image_style(array('path' => $photo, 'style_name' => '170x170', 'height' => 170, 'width' => 170)); ?>
    </div>
    <div class="field-name-title">
      <?php print l($title, $link); ?>
    </div>
    <div class="person-position">
      <?php print $rank_area; ?>
    </div>
  </div>
</div>