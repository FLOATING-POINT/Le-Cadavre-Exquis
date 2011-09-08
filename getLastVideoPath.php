<?php

require '_php/utils/class.mysqlConnection.php';

$mysql = new mysqlConnection();
$mysql->connect();

$query 	= mysql_query("SELECT name FROM uploads  WHERE name !='' ORDER BY id DESC LIMIT 1 ") or die(mysql_error());
$name 	= '';

while($r = mysql_fetch_array($query)){
	$name = $r['name'];
}

echo $name;

?>