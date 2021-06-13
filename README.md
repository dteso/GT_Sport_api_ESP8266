# GT_Sport_api_ESP8266
Consigue en tiempo real tus puntuaciones de usuario, próximos eventos tanto en un display 128x32 controlado por nodeMCU como en un web server controlado por el ESP. Se hace el uso de llamadas HTTPS, comunicación serie con aplicación Ionic para configurar tus datos WiFi y de usuario

Prea hacerlo funcionar:

1. Recuerda establecer tu ssid y tu password en el fichero config.h. Igualmente deberás conocer tu número de usuario, eol cual puedes obtener a través de la página oficial de PlaySatation Network

2. Necesitarás únicmanete un NodeMCU 8266 y un display 128x32 i2c

¿Qué utiliza este proyecto que puede serme útil para otros proyectos?

- Uso como NTP client con conexión a servidor gratuito NTP aprovechando la conexión WiFi para obtener la hora en tiempo real, como alternativa a no usar el RTC interno del ESP.

- Llamadas a apis externas con cifrado ssl utilizando la libreria WiFiClientSecureBearSSL. En este caso se utiliza la api de Gran Turismo Sport https://www.gran-turismo.com/us/api/gt7sp, pero hay otros tipos de apis como a las que te podría gustar peticionar y que son https igualmente, llevando a cabo tus llamadas de forma análoga a como se realiza en este ejemplo. 

- Diseña y carga tus propias imágenes para poder cargarlas en tu display oled. Esta transformación se realiza gracias a la aportación de DIY Usthad mediante la funcionalidad que pone a nuestra disposición accediendo a https://diyusthad.com/2019/02/display-images-in-oled-display.html
