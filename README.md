# ESP-HardwareControlWiFi

Hardware Control WiFi adalah mini project yang saya buat untuk penerapan kontrol hardware via WiFi menggunakan mikrokontroller ESP8266 (NodeMCU-12E). Tujuan awalnya adalah untuk memudahkan murid-murid Teknik elektro agar dapat mengoperasikan suatu hardware untuk melakukan eksperimen-eksperimen. Pengoperasian ini dapat dilakukan dari jarak jauh menggunakan internet. 

Model Sistem ini terinspirasi dari salah satu pembaca blog Wildanie yang memberikan request untuk membuat sistem dengan latar belakang permasalahan seperti ini. Setelah direalisasikan, ternyata ada banyak sekali penerapan-penerapan di kondisi-kondisi yang lain menggunakan sistem seperti ini. 

Harapannya adalah, sistem ini bisa menjadi suatu pondasi untuk aplikasi-aplikasi yang inovatif seperti smarthome, otomasi mesin produksi, dan lain sebagainya. 


## Anatomi Sistem

Project ini terdiri hanya terdiri dari komponen mikrokontroller berbasis WiFi ESP8266 seperti NodeMCU-12E, sebuah jaringan lokal, dan perangkat client seperti 
PC/Laptop, tablet atau smartphone. 
Satu anatomi yang lain bersifat opsional yakni IP publik dari suatu ISP yang nantinya bisa dimanfaatkan untuk menghubungkan sistem kontrol ini ke Internet. Caranya dengan membuat port forwarding di router pusat, dan diarahkan ulang ke alamat IP perangkat WiFi ESP8266



