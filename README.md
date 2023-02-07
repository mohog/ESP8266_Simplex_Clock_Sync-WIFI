# ESP8266_Simplex_Clock_Sync-WIFI

This arduino code can be used to keep a simplex hourly corrected clock on time.

It will connect to Wifi, sync it's time with NTP and pulse the relay for 8 and 14 seconds to sync a Simplex "slave" clock, using the clock's sync "clutch". 

The hourly sync will happen a few minutes before every hour and the daily sync will happen at (a couple minutes before) 6AM and 6PM.

Wire one half of the electromechanical clutch to the neutral wire.  Second leg will connect to the NO terminal on the relay.  Hot power will be connected to the Common terminal on the relay.

I used a ESP8266 D1 mini (clone) and a D1 style relay.

![D1 Mini](https://user-images.githubusercontent.com/5247441/217359031-541843c5-81a5-4bce-a558-75f2ad3c15c5.PNG)
![D1 Relay](https://user-images.githubusercontent.com/5247441/217359105-40104c2b-8097-41ab-9a7c-444a0b1549ca.PNG)
