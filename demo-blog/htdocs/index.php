<?php
/*
Purpose:
To build the smallest and simplist Blog that could possibly work, while
simultaneously demonstrating my familiarity with various web development
languages, etc - most particularly to serve as an indicator of my coding
style.

Relevant technologies include XHTML, PHP, MySQL, and possibly some JS.
Despite being a "toy" program, best security practices will need to be
followed throughout as I will be somewhat evaluated based on how I use them.
This project shouldn't make use of any "fun" or "neat" technologies that I'd
like to play with, as the stated intent is not to show exploratory
programming.
@todo - clean up the purpose block to remove trailing one word lines.

Sadly, showing off is hard to avoid with an audience :P
*/

require_once __DIR__.'../config.php';

// Set up here so that I close it in this file
// ... so, no guarantees on die :P.
// if I want to close it cleanly, need to __destruct it?
$db = mysql_connect(BLOG_HOST, BLOG_USER, BLOG_PASS);
if (!$db || !mysql_select_db(BLOG_DB, $db)) {
    error('Could not connect to db - error: '.mysql_error());
}
// I'm just going to use this as front end processing script.
// note, not using REST urls
if (!isset($_GET['action'])) {
    $action = 'index';
} else {
    $action = basename(strtolower($_GET['action']), 'php');
}
$whitelist = array('index','view','add');
if (in_array($action, $whitelist)) {
    render(BLOG_ROOT.'header.php', array(
        'title' => 'Simple Blog Site - '.ucfirst($action),
        'index' => ($action == 'index'?true:false)
    ));
    require_once BLOG_ROOT.$_GET['action'].'.php';
    readfile(BLOG_ROOT.'footer.html');
} else {
    header('Location: index.php'); // @todo - root agnostic
    die;
}
?>
