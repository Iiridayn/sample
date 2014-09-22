<?php

/**
 * Why static? State isn't needed. I would feel dirtier building something slow
 * just to prove a point :P. Yes, I know that the state is implicitly managed by
 * the call stack - that's what it's there for.
 */
interface OutputFramework {
	public static function output($header, $footer, $body);

	private static function multiple($content);
	private static function section($content, $title);
	private static function table($tbody);

	private static function input($content, $label, $name);
	private static function checkbox($content, $label, $name);
	private static function image($content);
	private static function text($content);
	private static function legal($content);
}

/**
 * Note that no provisions for testing of data safety have been made. It's not
 * a bug, it's a feature :P.
 */
class HtmlOutput implements OutputFramework {
	public static function output($header, $footer, $body) {
		// send headers, etc
		// do header, body, footer

		$document = '<html><head><title>Ivinex Development Demonstration</title>';
		$document .'</head><body>';

		$document .= '<div class="pageHeader">'.self::multiple($header).'</div>';
		$document .= '<div class="body">'.self::multiple($body).'</div>';
		$document .= '<div class="pageFooter">'.self::multiple($footer).'</div>';

		$document .= '</body></html>';

		return $document;
	}

	// could use reflection to test for method existance. Could use dynamic calls...
	// both are annoyingly slower and harder to read for the uninitiated.
	private static function multiple($content) {
		$document = '';
		foreach($content as $tag) {
			switch($tag['type']) {
			case 'input':
				$document .= self::input($tag['content'], $tag['label'], $tag['name']);
				break;
			case 'checkbox':
				$document .= self::checkbox($tag['content'], $tag['label'], $tag['name']);
				break;
			case 'image':
				$document .= self::image($tag['content']);
				break;
			case 'text':
				$document .= self::text($tag['content']);
				break;
			case 'legal':
				$document .= self::legal($tag['content']);
				break;

			case 'section':
				$document .= self::section($tag['content'], $tag['title']);
				break;
			case 'table':
				$document .= self::section($tag['tbody']);
				break;
			case 'multiple':
				$document .= self::multiple($tag['content']);
				break;
			}
		}
	}

	private static function section($content, $title) {
		$output = '<div class="section"><h2>'.$title.'</h2>';
		$output .= self::multiple($content);
		$output .= '</div>';
		return $output;
	}

	private static function table($tbody) {
		$output = '<table>';
		// TODO - colspan support, max depth calculations, etc
		foreach($tbody as $row) {
			if (!$row) {
				$ouput .= '<tr class="empty"><td></td></tr>';
				continue;
			}

			$output .= '<tr>';
			foreach($row as $cell) {
				$output .= '<td>'.self::multiple(array($cell)).'</td>';
			}
			$output .= '</tr>';
		}
		$output .= '</table>';
	}



	private static function input($content, $label, $name, $type='text') {
		$output = '<div class="input '.$type.'">';
		$output .= '<label for="'.$name.'">'.$content.'</label>';
		$output .= '<input type="'.$type.'" id="'.$name.'" name="'.$name.'" />';
		$output .= '</div>';

		return $output;
	}

	private static function checkbox($content) {
		return self::input($content, $label, $name, 'checkbox');
	}

	private static function image($content) {
		$content = '/img/'.$content; // not security checked - not a problem now
		return '<img src="'.$content.'" />';
	}

	private static function text($content, $class=false) {
		return '<p'.($class?' class="'.$class.'"':'').'>'.nl2br($content).'</p>';
	}
	private static function legal($content) {
		return self::input($content, 'legal');
	}
}

?>
