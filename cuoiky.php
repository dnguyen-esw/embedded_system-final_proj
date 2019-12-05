<?php
$blue_led=$_POST["blue_led"];
$white_led=$_POST["white_led"];
$fan=$_POST["fan"];
$pump=$_POST["pump"];
$roof=$_POST["roof"];
$shutdown=$_POST["shutdown"];

$temp=$_POST["temp"];
$humid=$_POST["humid"];
$light=$_POST["light"];
if ($shutdown =="0") 
{
    $blue_led="0";
	$white_led="0";
	$fan="0";
	$pump="0";
	$roof="0";
}
$time=date("H:i:s");
//ket noi database
$servername = "localhost";
$username = "root";
$password = "12345";
$dbname = "embedded_sys";
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

$cmd="Update io_dev SET state='$blue_led',time='$time' WHERE dev='blue_led'";
$conn->query($cmd);
$cmd="Update io_dev SET state='$white_led',time='$time' WHERE dev='white_led'";
$conn->query($cmd);
$cmd="Update io_dev SET state='$fan',time='$time' WHERE dev='fan'";
$conn->query($cmd);
$cmd="Update io_dev SET state='$pump',time='$time' WHERE dev='pump'";
$conn->query($cmd);
$cmd="Update servo SET state='$roof',time='$time' WHERE dev='roof'";
$conn->query($cmd);

$cmd="Update alarm SET temp='$temp',time='$time' ";
$conn->query($cmd);
$cmd="Update alarm SET humid='$humid',time='$time' ";
$conn->query($cmd);
$cmd="Update alarm SET light='$light',time='$time' ";
$conn->query($cmd);

$output_1 = shell_exec('./io_devices');
// Load html layout
ob_start();
include "cuoiky.html";
$return = ob_get_contents();
ob_clean();
echo $return;

$result = mysqli_query($conn,"SELECT * FROM io_dev");
echo "<table border='1' width='350' height='320' class='zui-table'>
<tr>
<th>Devices</th>
<th>State</th>
<th>Last access</th>
</tr>";

while($row = mysqli_fetch_array($result))
{
	echo "<tr>";
	echo "<td>" . $row['dev'] . "</td>";
	echo "<td>" . $row['state'] . "</td>";
	echo "<td>" . $row['time'] . "</td>";
	echo "</tr>";
}
echo "Table 1. Working devices state";
echo "</table>";
$conn->close();//close database*/
?>



