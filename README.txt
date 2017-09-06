# unlikeVK
Unlikes all your liked posts and photos. Requires captcha solving sometimes.
Uses Qt libs.

Снимает лайки со всех понравившихся постов и фото. Иногда придётся разгадывать капчу.
Использует библиотеки Qt.

Preparation:
1) Make your own vk application (https://vk.com/apps?act=manage) - get insruction here https://habrahabr.ru/post/265563/ (point 1 only)
2) Fill my_id and app_id in lib.h
3.1) Uncomment for_token string output in main.cpp
3.2) paste it into your browser address line, press Enter
3.3) agree with access rights (could be edited by "scope" variable)
3.4) copy access_token from address line into lib.h

Подготовка:
1) Создайте своё приложение, инструкция по ссылке.
2) Впишите ID свой страницы и своего приложения в соответствующие константы в файле lib.h
3) Раскомментируйте вывод строки for_token в файле main.cpp
3.2) Перейдите по этому адресу в браузере
3.3) Разрешите запрашиваемый доступ (можно изменить с помощью переменной scope)
3.4) Скопируйте access_token из адресной строки браузера в соответствующую константу lib.h

