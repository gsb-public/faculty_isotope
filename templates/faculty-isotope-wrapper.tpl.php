<div id="faculty-isotope" class="view-content pane-faculty-filters-faculty-list">
  <div class="view-faculty-filters">
    <div class="filter-group">
      <div class="views-exposed-search-wrap">
        <div id="edit-search-wrapper">
          <div class="form-item form-item-search">
            <input class="form-text" placeholder="search by name, research interests, or other keywords" type="text" id="edit-search" name="search" value="" size="30" maxlength="128">
          </div>
        </div>
        <div class="views-submit-button">
          <input type="submit" id="edit-submit-faculty-filters" />
        </div>
      </div>
      <div class="views-widget-filter-secondary">
        <fieldset class="collapsible form-wrapper collapse-processed" id="edit-secondary">
          <legend>
            <span class="fieldset-legend">
              <a class="fieldset-title" href="#">
                <span class="fieldset-legend-prefix element-invisible">Hide</span> Narrow your results
              </a>
              <span class="summary"></span>
            </span>
          </legend>
          <div class="fieldset-wrapper" style="overflow: hidden;">
            <div class="bef-secondary-options">
              <div class="form-item form-type-select academic-areas">
                <label for="edit-term-node-tid-depth">Academic Area </label>
                <div class="form-checkboxes bef-select-as-checkboxes bef-required-filter-processed">
                  <div class="bef-checkboxes">
                    <?php foreach ($academic_areas as $academic_area_key => $academic_area_value): ?>
                      <div class="form-item">
                        <input type="checkbox" name="<?php print $academic_area_key; ?>" value="<?php print $academic_area_key; ?>">
                        <label class="option"><?php print $academic_area_value; ?></label>
                      </div>
                    <?php endforeach; ?>
                  </div>
                </div>
              </div>
              <div class="form-item form-type-select centers">
                <label for="edit-term-node-tid-depth-1">Centers, Initiatives, and Institutes </label>
                <div class="form-checkboxes bef-select-as-checkboxes bef-required-filter-processed">
                  <div class="bef-checkboxes">
                    <?php foreach ($centers as $center_key => $center_value): ?>
                      <div class="form-item">
                        <input type="checkbox" name="<?php print $center_key; ?>" value="<?php print $center_key; ?>">
                        <label class="option"><?php print $center_value; ?></label>
                      </div>
                    <?php endforeach; ?>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </fieldset>
      </div>
    </div>
    <div class="faculty-list" style="clear: both;">
      <?php print $items; ?>
    </div>
  </div>
</div>