<?php
    $json_obj = file_get_contents("http://localhost:4040/api/tunnels");
    $php_obj = json_decode($json_obj, true);

    $tunnel0 = $php_obj['tunnels'][0];
    $tunnel0_url = $tunnel0['public_url'];
    $tunnel0_name = $tunnel0['name'];
    //echo $tunnel0_url;

    $tunnel1 = $php_obj['tunnels'][1];
    $tunnel1_url = $tunnel1['public_url'];
    $tunnel1_name = $tunnel1['name'];
    //echo $tunnel1_url;

    $arr = array($tunnel0_name => $tunnel0_url, $tunnel1_name => $tunnel1_url);

    $json = json_encode($arr);
    echo $json;

?>