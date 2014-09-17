<?php
/*
Displays a small list of posts
Not going to use HTML5 stuff since it doesn't validate yet.
*/
$sql = "SELECT p.id, p.title, p.created, p.content,
            COUNT(comments.*) as comments
        FROM posts AS p
        LEFT JOIN comments ON posts.id = comments.post_id
        GROUP BY posts.id
        ORDER BY created
        LIMIT 3";
$result = mysql_query($sql, $db);
if (!$result){
    error('Error in MySQL query: '.mysql_error());
}
while($row = mysql_fetch_assoc($result)):
?>
<div class="post">
    <h2>
        <a href="/view/<?php echo slug($row['id'], $row['title']); ?>">
            <?php echo clean($row['title']); ?>
        </a>
    </h2>
    <div class="summary">
        <?php
            echo strip_tags(
                explode("\0",
                    wordwrap($row['content'], 200, "\0")
                )[0]
            );
        ?>
    </div>
    <div class="posted-date">
        <?php echo date(DATE_RFC822, strtotime($row['created'])); ?>
    </div>
    <div class="comment-info">
        Comments (<?php echo $row['comments']; ?>)
    </div>
</div>
<?php endwhile; ?>
