<?php
/*
Global convenience functions file
... Now I am beginning to understand the layout of the F3 framework.
*/
function error($message) {
    // do something different if debug mode is on
    die($message);
}

// Maybe consider having an option to write to buffer and return
function render($_file, $_data) {
    if (!is_readable($_file)) {
        error();
    }
    extract($_data);
    include $_file;
}

function clean($text) {
    return htmlentities(
        iconv('UTF-8', 'UTF-8//IGNORE', $text),
        ENT_QUOTES|ENT_XHTML,
        'UTF-8'
    );
}

// Nice, found below, but no license. Safe for this purpose, but not prod.
// http://sourcecookbook.com/es/recipes/8/function-to-slugify-strings-in-php
function slug($id, $text) {
    // replace non letter or digits by -
    $text = preg_replace('~[^\\pL\d]+~u', '-', $text);
    // trim
    $text = trim($text, '-');
    // transliterate - key
    $text = iconv('utf-8', 'us-ascii//TRANSLIT', $text);
    // lowercase
    $text = strtolower($text);
    // remove unwanted characters
    $text = preg_replace('~[^-\w]+~', '', $text);

    return $id.'-'.$text;
}
?>
