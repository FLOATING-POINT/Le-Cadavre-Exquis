<?php

require '_php/vimeo/vimeo.php';
require '_php/utils/class.mysqlConnection.php';

require 'videoFiles.php';

$CONSUMER_KEY			= 'YOUR-CONSUMBER-KEY';
$CONSUMER_SECRET		= 'YOUR-CONSUMER-SECRET';
$ACCESS_TOKEN			= 'YOUR-ACCESS-TOKEN';
$ACCESS_TOKEN_SECRET	= 'YOUR-ACCESS-TOKEN-SECRET';

 
 
echo "UPLOAD STARTED";

//if(!empty($_POST['videoName'])){ 
	
	
	$mysql			= new mysqlConnection();
	// record submission
	$mysql->connect();
	
	$location 		= 'Nottingham Contemporary';
	$pathToFile 	= 'videos/'.$_GET['videoName'];
	
	date_default_timezone_set('Europe/London');
	$date			= date("jS F Y, H:i:s ");
	
	$title  		= $location.' '.date("jS F Y, H:i:s ");
	$uploadTime		= $title;
	$description 	= '';
	
	$query 			= mysql_query("INSERT INTO uploads ( name, status, uploadTime ) VALUES ('$pathToFile', 'UPLOAD_STARTED', '$uploadTime')") or die(mysql_error());
	$msqlID 		= mysql_insert_id();
	
	
	
	$vimeo 			= new phpVimeo($CONSUMER_KEY, $CONSUMER_SECRET, $ACCESS_TOKEN, $ACCESS_TOKEN_SECRET);
	
	

	try {
		date_default_timezone_set('UTC');
		$time_start = microtime(true);
		$video_id 	= $vimeo->upload($pathToFile);
	
		if ($video_id) {
			
			$time_end 	= microtime(true);
			$time 		= $time_end - $time_start;			
	
			$vimeo->call('vimeo.videos.setTitle', array('title' => $title, 'video_id' => $video_id));
			$vimeo->call('vimeo.videos.setDescription', array('description' => $description, 'video_id' => $video_id));
			
			echo '<a href="http://vimeo.com/' . $video_id . '">Upload successful! Uploaded in  '.$time.' seconds</a>';
			$mysql->connect();
			$query = mysql_query("UPDATE uploads SET status = 'COMPLETE', uploadDuration = $time, vimeoid = '$video_id' WHERE id = $msqlID ") or die(mysql_error());
			
		} else {
			
			echo "Video file did not exist!";
			$err = 'Video file did not exist/ connection lost';
			$query = mysql_query("UPDATE uploads SET status = 'FAILED', localErrMsg = $err WHERE id = $msqlID ") or die(mysql_error());
			
		}
		
	} catch (VimeoAPIException $e) {
		
		echo "Encountered an API error -- code {$e->getCode()} - {$e->getMessage()}";
		$errMsg = $e->getMessage();
		$errCode = $e->getCode();
		$query = mysql_query("UPDATE uploads SET status = 'API_ERROR', vimeoErrCode = '$errCode', vimeoErrMsg = '$errMsg' WHERE id = $msqlID ") or die(mysql_error());


	}
//}
generateVideoXML();

?>