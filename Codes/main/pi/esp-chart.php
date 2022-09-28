<?php
include 'connect.php';
?>

<!DOCTYPE html>
<html>

<head>
    <title>Mushroom Chart</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://code.highcharts.com/highcharts.js"></script>
    <style>
        body {
            min-width: 310px;
            max-width: 1280px;
            height: 500px;
            margin: 0 auto;
            margin-top: 20px;
        }

        h2 {
            font-family: Arial;
            font-size: 2.5rem;
            text-align: center;
        }
    </style>
</head>

<body>
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
    $cerrent_date = date("Y-m-d");
    //echo $cerrent_date;
    //$sql = "SELECT id, temp, humi, save_time FROM SensorData ORDER BY save_time DESC LIMIT 24"; //ASC||DESC
    //defalt sql search by cerrent date
    $sql = "SELECT id, temp, humi, lux, save_time FROM SensorData WHERE save_time LIKE '" . $cerrent_date . "%' ORDER BY save_time DESC";

    // Date filter
    if (isset($_POST['btn_search'])) {
        $searchDate = $_POST['searchDate'];
        //echo $searchDate;
        if (!empty($searchDate)) {
            $sql = "SELECT id, temp, humi, lux, save_time FROM SensorData WHERE save_time LIKE '" . $searchDate . "%' ORDER BY save_time DESC";
        }
    }

    $result = $conn->query($sql);

    while ($data = $result->fetch_assoc()) {
        $sensor_data[] = $data;
    }

    $save_time = array_column($sensor_data, 'save_time');

    $temp = json_encode(array_reverse(array_column($sensor_data, 'temp')), JSON_NUMERIC_CHECK);
    $humi = json_encode(array_reverse(array_column($sensor_data, 'humi')), JSON_NUMERIC_CHECK);
    $lux = json_encode(array_reverse(array_column($sensor_data, 'lux')), JSON_NUMERIC_CHECK);
    $save_time = json_encode(array_reverse($save_time), JSON_NUMERIC_CHECK);

    $result->free();
    $conn->close();
    ?>

    <div id="chart-temperature" class="container"></div>
    <div id="chart-humidity" class="container"></div>
    <div id="chart-light" class="container"></div>
    <script>
        var temp = <?php echo $temp; ?>;
        var humi = <?php echo $humi; ?>;
        var lux = <?php echo $lux; ?>;
        var save_time = <?php echo $save_time; ?>;

        var chartT = new Highcharts.Chart({
            chart: {
                renderTo: 'chart-temperature'
            },
            title: {
                text: 'Mushroom Temperature'
            },
            series: [{
                showInLegend: false,
                data: temp
            }],
            plotOptions: {
                line: {
                    animation: false,
                    dataLabels: {
                        enabled: true
                    }
                },
                series: {
                    color: '#DE2105'
                }
            },
            xAxis: {
                type: 'datetime',
                categories: save_time
            },
            yAxis: {
                title: {
                    text: 'Temperature (Celsius)'
                }
                //title: { text: 'Temperature (Fahrenheit)' }
            },
            credits: {
                enabled: false
            }
        });

        var chartH = new Highcharts.Chart({
            chart: {
                renderTo: 'chart-humidity'
            },
            title: {
                text: 'Mushroom Humidity'
            },
            series: [{
                showInLegend: false,
                data: humi
            }],
            plotOptions: {
                line: {
                    animation: false,
                    dataLabels: {
                        enabled: true
                    }
                },
                series: {
                    color: '#00add6'
                }
            },
            xAxis: {
                type: 'datetime',
                //dateTimeLabelFormats: { second: '%H:%M:%S' },
                categories: save_time
            },
            yAxis: {
                title: {
                    text: 'Humidity (%)'
                }
            },
            credits: {
                enabled: false
            }
        });

        var chartL = new Highcharts.Chart({
            chart: {
                renderTo: 'chart-light'
            },
            title: {
                text: 'Mushroom Light'
            },
            series: [{
                showInLegend: false,
                data: lux
            }],
            plotOptions: {
                line: {
                    animation: false,
                    dataLabels: {
                        enabled: true
                    },
                    series: {
                    color: '#e47025'
                }
                }
            },
            xAxis: {
                type: 'datetime',
                //dateTimeLabelFormats: { second: '%H:%M:%S' },
                categories: save_time
            },
            yAxis: {
                title: {
                    text: 'Light (lx)'
                }
            },
            credits: {
                enabled: false
            }
        });
    </script>
</body>

</html>