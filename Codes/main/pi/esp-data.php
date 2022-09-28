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

        .row:after {
            content: "";
            display: table;
            clear: both;
        }

        .half {
            float: left;
            width: 50%;
            text-align: center;
        }

        .oneinfour {
            float: left;
            width: 25%;
        }
    </style>
</head>

<body>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Mushroom Data Logger</title>
    <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css">
    <script src="https://code.jquery.com/jquery-1.12.4.js"></script>
    <script src="https://code.jquery.com/ui/1.12.1/jquery-ui.js"></script>
    <script>
        $(function() {
            $(".dateFilter").datepicker({
                dateFormat: "yy-mm-dd",
            });
        });
    </script>
    <div class="row" style="padding-bottom: 5px;">
        <div class="half">
            <!-- Search filter -->
            <form method='post' action=''>
                Search Date <input type='text' class='dateFilter' name='searchDate' value='<?php if (isset($_POST['searchDate'])) echo $_POST['searchDate']; ?>'>
                <input type='submit' name='btn_search' value='Search'>
            </form>
        </div>
        <div class="half">
            <!-- Search Range filter -->
            <form method='post' action=''>
                Start Date <input type='text' class='dateFilter' name='fromDate' value='<?php if (isset($_POST['fromDate'])) echo $_POST['fromDate']; ?>'>

                End Date <input type='text' class='dateFilter' name='endDate' value='<?php if (isset($_POST['endDate'])) echo $_POST['endDate']; ?>'>

                <input type='submit' name='btn_search_range' value='Search'>
            </form>
        </div>
    </div>

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
    // Date filter search range
    if (isset($_POST['btn_search_range'])) {
        $fromDate = $_POST['fromDate'];
        $fromDate .= " 00:00:00";  // Second start date
        $endDate = $_POST['endDate'];
        $endDate .= " 23:59:59";   // Second end date
        //echo $fromDate;
        //echo " - ";
        //echo $endDate;
        if (!empty($fromDate) && !empty($endDate)) {
            $sql = "SELECT * FROM SensorData WHERE save_time BETWEEN '" . $fromDate . "' AND '" . $endDate . "' ";
        }
    }
    echo '<table class="table1">';
    echo '<tr> 
        <th>ID</th> 
        <th>Temperature (*c)</th> 
        <th>Humidity (%)</th>
        <th>Light (lx)</th> 
        <th>Temp Limit Min</th> 
        <th>Temp Limit Max</th>
        <th>Humi Limit Min</th> 
        <th>Humi Limit Max</th>
        <th>Set Light (lx)</th> 
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
            $row_lux = $row["lux"];
            $row_temp_limit_min = $row["temp_limit_min"];
            $row_temp_limit_max = $row["temp_limit_max"];
            $row_humi_limit_min = $row["humi_limit_min"];
            $row_humi_limit_max = $row["humi_limit_max"];
            $row_set_lux = $row["set_lux"];
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
                <td>' . $row_lux . '</td>
                <td>' . $row_temp_limit_min . '</td> 
                <td>' . $row_temp_limit_max . '</td>
                <td>' . $row_humi_limit_min . '</td>
                <td>' . $row_humi_limit_max . '</td>
                <td>' . $row_set_lux . '</td>
                <td>' . $row_ctrl_mode . '</td>
                <td>' . $row_pump_state . '</td>
                <td>' . $row_fan_state . '</td>    
                <td>' . $row_save_time . '</td> 
              </tr>';
        }
        $result->free();
    } else {
        echo "<tr>";
        echo "<td colspan='9' class = 'center'>No record found.</td>";
        echo "</tr>";
        $result->free();
    }
    echo '</table>';

    $conn->close();
    ?>


</body>

</html>