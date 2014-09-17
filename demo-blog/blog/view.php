<?php
/*
Displays a single post, handles comments
*/

if (!isset($action[1])) {
    error();
}
$id = explode('-', $action[1]);
if (!is_numeric($id)) {
    error();
}

$sql = "SELECT id, title, created, content,
        FROM posts
        WHERE id = ".mysql_real_escape_string($id);
$result = mysql_query($sql, $db);
if (!$result){
    error('Error in MySQL query: '.mysql_error());
}

if (!($post = mysql_fetch_assoc($result))) {
    error();
}
?>
<div class="post">
    <h1><?php echo clean($post['title']); ?></h1>
    <div id="post-body">
        <?php /* @todo - htmlpurifier */ ?>
        <?php echo $post['content']; ?>
    </div>
    <div class="posted-date">
        <?php echo date(DATE_RFC822, strtotime($post['created'])); ?>
    </div>
    <div id="comments">
    <?php
        $sql = "SELECT content, created, name
                FROM comments
                WHERE post_id = ".mysql_real_escape_string($id)."
                ORDER BY created ASC";
        $result = mysql_query($sql, $db);
        if (!$result){
            error('Error in MySQL query: '.mysql_error());
        }
    ?>
        <?php if (!mysql_num_rows($result)): ?>
            There are no comments yet for this post. Be the first!
        <?php else: ?>
        <ol>
            <?php while($comment = mysql_fetch_assoc($result)): ?>
            <li>
                <p>Comment from: <?php echo clean($comment['name']); ?></p>
                <p><?php echo clean($comment['content']); ?></p>
            </li>
            <?php endwhile; ?>
        </ol>
        <?php endif; ?>
        <?php /* todo, comment form */ ?>
    </div>
</div>
<?php endwhile; ?>
