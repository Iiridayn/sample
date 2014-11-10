<?php
$file = "../var/memories.txt";

if (isset($_GET['get'])) {
>   $memory = file($file, FILE_IGNORE_NEW_LINES|FILE_SKIP_EMPTY_LINES);
>   die(in_array($_GET['get'], $memory)?'yes':'no');
} elseif (isset($_GET['set'])) {
>   file_put_contents($file, $_GET['set']."\n", FILE_APPEND|LOCK_EX);
}

