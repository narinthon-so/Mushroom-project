<?php
include 'connect.php';
/*#4CAF50         */
?>

<!doctype html>
<html lang="en">

<head>
    <style>
        table.table1 {
            border-collapse: collapse;
            width: 100%;
        }

        table.table1 th {
            background-color: #4CAF50;
            color: white;
            text-align: left;
            padding: 8px;
        }

        table.table1 td {
            text-align: left;
            padding: 8px;
        }

        table.table1 tr:nth-child(even) {
            background-color: #f2f2f2;
        }
    </style>
</head>

<body>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Mushroom Data Logger</title>
    <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">
    <link rel="stylesheet" href="/resources/demos/style.css">
    <script src="https://code.jquery.com/jquery-1.12.4.js"></script>
    <script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script>
    <script>
        $(function() {
            $("#dateFilter").datepicker({
                dateFormat: "yy-mm-dd",
            });
        });
    </script>
    <!-- Search filter -->
    <form method='post' action=''>
        Search Date <input type='text' id='dateFilter' name='searchDate' value='<?php if (isset($_POST['searchDate'])) echo $_POST['searchDate']; ?>'>
        <input type='submit' name='btn_search' value='Search'>
    </form>
    <?php

    //$sql = "SELECT * FROM SensorData ORDER BY id ASC";
    $sql = "SELECT * FROM SensorData WHERE 1 ";

    // Date filter
    if (isset($_POST['btn_search'])) {
        $searchDate = $_POST['searchDate'];
        //echo $searchDate;
        if (!empty($searchDate)) {
            $sql .= " AND save_time LIKE '" . $searchDate . "%' ";
        }
    }
    echo '<table class="table1">';
    echo '<tr> 
        <th>ID</th> 
        <th>Temperature</th> 
        <th>Humidity</th> 
        <th>Temp Limit</th> 
        <th>Humi Limit</th> 
        <th>Ctrl Mode</th> 
        <th>Pump State</th> 
        <th>Fan State</th> 
        <th>Timestamp</th> 
      </tr>';

    $result = $conn->query($sql);
    if (mysqli_num_rows($result) > 0) {
        while ($row = $result->fetch_assoc()) {
            $row_id = $row["id"];
            $row_temp = $row["temp"];
            $row_humi = $row["humi"];
            $row_temp_limit = $row["temp_limit"];
            $row_humi_limit = $row["humi_limit"];
            $row_ctrl_mode = $row["ctrl_mode"];
            $row_pump_state = $row["pump_state"];
            $row_fan_state = $row["fan_state"];
            $row_save_time = $row["save_time"];
            // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
            //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));

            // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
            //$row_save_time = date("Y-m-d H:i:s", strtotime("$row_save_time + 7 hours"));

            echo '<tr> 
                <td>' . $row_id . '</td> 
                <td>' . $row_temp . '</td> 
                <td>' . $row_humi . '</td> 
                <td>' . $row_temp_limit . '</td> 
                <td>' . $row_humi_limit . '</td>
                <td>' . $row_ctrl_mode . '</td>
                <td>' . $row_pump_state . '</td>
                <td>' . $row_fan_state . '</td>    
                <td>' . $row_save_time . '</td> 
              </tr>';
        }
        $result->free();
    } else {
        echo "<tr>";
        echo "<td colspan='4' class = 'center'>No record found.</td>";
        echo "</tr>";
        $result->free();
    }
    echo '</table>';

    $conn->close();
    ?>


</body>

</html>