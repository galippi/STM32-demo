AT
AT+CWMODE=1
AT+CWJAP="WiFi-SSID","Wifi-password"

AT+CWJAP="WiFi-SSID","Wifi-password"

AT+CWMODE=?
Set station mode: AT+CWMODE=1
AT+CWLAP
Connect to WiFi: AT+CWJAP="WiFi-SSID","Wifi-password"

Get IP status: AT+CIFSR

Enable multiple connection: AT+CIPMUX=1

Create UDP socket ID 4: AT+CIPSTART=4,"UDP","192.168.2.99",1234,1234,0
Create UDP socket ID 4: AT+CIPSTART=4,"UDP","192.168.2.101",1234,1234,0
Send 5 bytes UDP: AT+CIPSEND=4,5

Ping: AT+PING="192.168.2.99"
