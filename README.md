# vk
Этот сервис позволяет пользователям регистрироваться, авторизоваться и просматривать ленту (фиктивную).

Язык: C++
СУБД: MySQL
API:
###
/register (POST): регистрация пользователя
###
Входные параметры:
###
email: адрес электронной почты
###
password: пароль
###
Выходные параметры:
###
user_id: ID пользователя
###
password_check_status: статус проверки пароля ("good", "perfect")
###
/authorize (POST): авторизация пользователя
###
Входные параметры:
###
email: адрес электронной почты
###
password: пароль
###
Выходные параметры:
###
access_token: JWT-токен, содержащий ID пользователя
###
/feed (GET): просмотр ленты
###
Входные параметры:
###
access_token: JWT-токен
###
Ответ:
###
200 OK, если токен валиден
###
401 Unauthorized, если токен невалиден
###
Запуск:
###
Установить MySQL
###
Создайте базу данных users
###
Импортируйте дамп базы данных dump.sql
###
Установите Docker
###
Скачайте код проекта: git clone https://github.com/bard-code/cpp-auth-feed.git
###
Соберите проект: docker build -t auth-feed .
###
Запустите контейнер: docker run -p 8080:8080 auth-feed
###
Отправьте запросы к API:
###
register: curl -X POST -H "Content-Type: application/json" -d '{ "email": "user@example.com", "password": "password123" }' http://localhost:8080/register
###
authorize: curl -X POST -H "Content-Type: application/json" -d '{ "email": "user@example.com", "password": "password123" }' http://localhost:8080/authorize
###
feed: curl -H "Authorization: Bearer <access_token>" http://localhost:8080/feed
###
