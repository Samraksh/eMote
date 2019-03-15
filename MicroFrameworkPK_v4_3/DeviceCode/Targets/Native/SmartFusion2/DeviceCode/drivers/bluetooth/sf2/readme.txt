- To create .gatt file -

from cmd prompt that allows python --version to run

python convert_gatt_service.py 
gives list of services

python convert_gatt_service.py org.bluetooth.service.generic_access out.gatt
creates the .gatt file


- To create .h "profile_data" file

python compile_gatt.py spp_and_le_counter.gatt btmain.h