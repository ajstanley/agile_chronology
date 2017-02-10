<?php

function getAuthorshipLink($linkID, $linkType) {
  // build query
  $authorshipTable = AUTHORSHIP_TABLE;
  $linkTable = AUTHORSHIP_LINK_TABLE;

  $query = db_select($authorshipTable, 'a');
  $getDefault = $query
      ->fields('a', array('authorship_id', 'text_en'))
      ->condition('default_type', 'C')
      ->execute()
      ->fetchAssoc();

  // if no results, return false
  if (!$getDefault) {
    return false;
  }
  $defaultID = $getDefault['authorship_id'];


  $query = db_select($linkTable, 'AL');
  $query->join($authorshipTable, 'A', 'AL.authorship_id = A.authorship_id');
  $results = $query
      ->fields('AL', array('authorship_id', 'link_id', 'link_type'))
      ->fields('A', array('text_en'))
      ->condition('AL.link_id', $linkID)
      ->condition('AL.link_type', $linkType)
      ->execute()
      ->fetchAssoc();
  if (!$results) {
    return FALSE;
  }
  if (empty($results)) {
    $results = $getDefault;
  }
  return array('0' => $defaultID, '1' => $results);
}

