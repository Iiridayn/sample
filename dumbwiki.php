<?php

//echo phpinfo(); die;

$qs = $_SERVER['REDIRECT_QUERY_STRING'] ?? 'index.html';
$qs = urldecode($qs);
$filename = basename($qs);
$ext = pathinfo($filename, PATHINFO_EXTENSION);

function get_file($filename) {
	return file_get_contents('../content/' . $filename);
}
function write_file($filename, $contents) {
	file_put_contents('../content/' . $filename, $contents);

	// Log change
	//die(shell_exec("cd ../content; git add .; env GIT_COMMITTER_NAME='Filk Server' GIT_COMMITTER_EMAIL='filk@iiridayn.info' git commit --author='Unknown <" . escapeshellarg($_SERVER['REMOTE_ADDR']) . ">' -m 'Change '" . $filename . " 2>&1"));
	// TODO: use the site username/email as the --author
	exec("cd ../content; git add .; env GIT_COMMITTER_NAME='Filk Server' GIT_COMMITTER_EMAIL='filk@iiridayn.info' git commit --author='Unknown <" . escapeshellarg($_SERVER['REMOTE_ADDR']) . ">' -m 'File '" . escapeshellarg($filename));
}
function remove_file($filename) {
	unlink('../content/' . $filename);
	exec("cd ../content; git add .; env GIT_COMMITTER_NAME='Filk Server' GIT_COMMITTER_EMAIL='filk@iiridayn.info' git commit --author='Unknown <" . escapeshellarg($_SERVER['REMOTE_ADDR']) . ">' -m 'Remove '" . escapeshellarg($filename));
}

function template($title, $body, $head = '', $end = '') {
?>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<title><?= $title ?></title>
		<link rel="stylesheet" type="text/css" media="all" href="/css/normalize.css" />
		<link rel="stylesheet" type="text/css" media="all" href="/css/style.css" />
		<link rel="stylesheet" type="text/css" media="all" href="/css/icons.css" />
<?= $head ?>
	</head>
	<body>
<?= $body ?>
<?= $end ?>
	</body>
</html>
<?php
}

if (substr($qs, 0, 7) === 'remove/') {
	if (!in_array(strtolower($ext), ['txt', 'html']))
		die;

	remove_file($filename);

	header('Location: ../');
	die;
}

if (substr($qs, 0, 5) === 'edit/') {
	if (!in_array(strtolower($ext), ['txt', 'html']))
		die;

	$contents = trim(get_file($filename));

	if (isset($_POST['body'])) {
		if (sha1($contents) !== $_POST['former'])
			die("File changed while editing. Please copy your edits, refresh, and make them again.");

		write_file($filename, str_replace("\r\n", "\n", $_POST['body']) . "\n");

		header('Location: ../' . substr($qs, 5));
		die;
	}
	ob_start();
?>
		<form method="POST">
			<input name="former" type="hidden" value="<?= sha1($contents) ?>" />
			<textarea name="body"><?= $contents ?></textarea>
			<input type="submit" />
		</form>
<?php
	$body = ob_get_clean();
	ob_start();
?>
		<style>
			textarea { width: 100%; height: 80ex; }
			textarea { font-family:Consolas,Monaco,Lucida Console,Liberation Mono,DejaVu Sans Mono,Bitstream Vera Sans Mono,Courier New, monospace; }
		</style>
<?php
	$head = ob_get_clean();
	ob_start();
?>
	<script>
		// https://stackoverflow.com/a/22673622/118153
		document.querySelector('textarea').addEventListener("keydown", function(e) {
			if (e.which===9) {
				e.preventDefault();
				document.execCommand("insertText", false, "\t");
			}
		}, false);
	</script>
<?php
	$end = ob_get_clean();
	template("Editing $filename", $body, $head, $end);
	die;
}

if (substr($qs, 0, 7) === 'search/' || substr($qs, 0, 10) === 'txtsearch/') {
	$offset = substr($qs, 0, 7) === 'search/' ? 7 : 10;
	$term = substr($qs, $offset);
	exec('rg -i ' . escapeshellarg($term) . ' *.' . ($offset === 7 ? 'html' : 'txt'), $out);
	ob_start();
?>
		<ul>
		<?php foreach ($out as $line): ?>
			<?php $result = explode(':', $line, 2) ?>
			<li><a href="../<?= $result[0] ?>"><?= $result[0] ?></a>: <?= htmlspecialchars($result[1]) ?></li>
		<?php endforeach; ?>
		</ul>
<?php
	$body = ob_get_clean();
	template("Searching for " . htmlspecialchars($term), $body);
	die;
}

if (strtolower($ext) == 'txt') {
	header("Content-Type: text/plain; charset=utf-8");
	echo get_file($filename);
	die;
}

$body = get_file($filename);
$has_title = substr($body, 0, 7) === 'Title: ';
if ($has_title) {
	$pos = strpos($body, "\n\n");
	$title = substr($body, 7, $pos - 7);
	$body = substr($body, $pos + 2);
	template($title, $body);
	die;
}

http_response_code(404);
//die("Whoops - contact Iiridayn");
