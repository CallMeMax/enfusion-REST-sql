 <?php
$servername = "host.docker.internal";
$username = $_POST["username"];
$password = $_POST["password"];
$dbname = $_POST["dbname"];
error_reporting(0);

$return = [];
$return_types = [];
$return_names = [];

$sql = $_POST["data"];

$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
	echo json_encode(["data"=>$return,"types"=>$return_types,"names"=>$return_names, "error"=>$conn->connect_error, "query"=>$sql]);
} else {
	
	$result = $conn->query($sql);
	if ($result->num_rows > 0) {
	  // output data of each row
	  while($row = $result->fetch_row()) {
		$return[] = $row;
	  }
	  while($row = $result->fetch_field()) {
		$t = $row->type;
		$return_names[] = $row->name;
		if ($t == 4 || $t == 5)
			$return_types[] = "FLOAT";
		
		if ($t == 12 || $t == 7)
			$return_types[] = "DATE";
		
		if ($t == 0 || $t == 246 || $t == 1 || $t == 2 || $t == 3 || $t == 8 || $t == 9 || $t == 16 || $t == 13)
			$return_types[] = "INT";
		
		if ($t == 15 || $t == 247 || $t == 253 || $t == 254)
			$return_types[] = "STRING";
		
		
	  }
	  
	}

	echo json_encode(["data"=>$return,"types"=>$return_types,"names"=>$return_names, "error"=>$mysqli->error, "query"=>$sql]);


	$conn->close();
}
?> 