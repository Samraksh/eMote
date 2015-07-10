# test script comments start with '#'
# sleep ms
sleep 5000
# COM_receive <enable/disable> <file name>
COM_receive file enable testTemp\rx_data.txt
sleep 5000
# COM_receive <enable/disable> <file name>
COM_receive file disable testTemp\rx_data.txt
test_file testTemp\rx_data.txt
