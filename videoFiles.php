<?php



function generateVideoXML(){
	
	if(!class_exists("mysqlConnection"))require '_php/utils/class.mysqlConnection.php';
	
	
	$mysql			= new mysqlConnection();
	// record submission
	$mysql->connect();
	$query 			= mysql_query(" SELECT * FROM uploads WHERE vimeoid != '' AND active = '1' ORDER BY id ASC") or die(mysql_error());
	
	$output = '<container>';  
	
	while($r = mysql_fetch_array($query)){
		
		$output .= '<video>'; 
			$output .= '<vimeoID>'.$r['vimeoid'].'</vimeoID>'; 
			$output .= '<file>'.$r['name'].'</file>'; 
		$output .= '</video>'; 
					
	}
	
	$output .= '</container>'; 
	//---------------------------------------------------------------------------
	
	
	$filename = 'videos.xml';
	
	if (is_writable($filename)) {
			
		if (!$handle = fopen($filename, 'w+')) {
			echo "Cannot open file ($filename)";
			 exit;
		}
	
		// Write $somecontent to our opened file.
		if (fwrite($handle, $output) === FALSE) {
			echo "Cannot write to file ($filename)";
			exit;
		}
	
		echo "Success, wrote to file ($filename)";
	
		fclose($handle);
	
	} else {
		echo "The file $filename is not writable";
	}
}

?>
