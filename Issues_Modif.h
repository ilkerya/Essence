/*
Hardware Modifications
1 - Fan_PWM output moved to GPIO22 from 'SENSOR_VN GPIO39' since 'SENSOR_VN GPIO39' is an Input only!!! 




Issues
1 - The fan by default runs by full speed when no PWM applied, it even runs when 5V supply applied insted of 12V unfortunately, the speed is low
     due to the low supply voltage. When the uc is on by applying low duty cycle the fan can be stopped regardless of the supply voltage however
     when the uc goes into deepsleep mode the PWM cant be supplied anymore afterwards the fan begins to rotate becuse the dc-dc converter supplies
     voltage around 5V ''the fan might work even at 3V ??'. we need to cut the supply of the dc-dc converter fully to avoid this.
     A controlled ldo can be used, there are specific pins that the output  can be latched by the uc during deepsleep GPIO2 is suitable for that
     but ideally the en pin of the ldo by default could be set to off so any output pin can be used.
     in this case there is no need to control the en pin of the dc-dc converter anymore.
     In lightsleep mode this is not an issue since the peripherials keep working so PWM is supplied so the fan doesnt work with supplied low dutycycle

2 - The reset circuit related to long press of the key is not needed in my opinion, since wdt is runnnig if the  code is stuck it will cause a reset
    I even add long press >7sec power on reset function for the key.

3 - The 3V Sensors voltage control seems to be not working, in any case 3V is supplied !!

4 - 



Notes 
Input Only IO s
'SENSOR_VN GPIO39'
'SENSOR_VP GPIO36'
GPIO35
GPIO34









 













*/