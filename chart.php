<?php
header('Content-Type: application/json');

$conn = mysqli_connect("localhost","root","12345","embedded_sys");

$sqlQuery = "SELECT ID,Temperature,Humidity,Time,Light from sensor WHERE ID>(select MAX(ID) from sensor)-30";

$result = mysqli_query($conn,$sqlQuery);

$data = array();
foreach ($result as $row) 
{
	$data[] = $row;
}

mysqli_close($conn);

echo json_encode($data);
?>
