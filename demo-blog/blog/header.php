<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html>
<head>
    <title><?php echo clean($title); ?></title>
    <link rel="stylesheet" href="css/blog.css" />
</head>
<body>
    <div id="header">
        <h<?php echo ($index?1:2); ?>>
            <?php echo clean($title); ?>
        </h<?php echo ($index?1:2); ?>>
    </div>
    <div id="content">
