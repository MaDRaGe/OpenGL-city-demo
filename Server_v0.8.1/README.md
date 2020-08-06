# City Server

## О программе
Простой сервер симулятора города: генерируется случайный город и отвечает на запросы клиентов.

---

## Запуск
Для запуска сервера рекомендуется использовать файл ``run_server.bat``, чтобы увидеть ошибки, в случае их возникновения.

---

## Файлы
Ниже приведены файлы и папки, необходимые для сервера:
- **server_settings.xml** – основные настройки сервера (все параметры приведены в следующем параграфе);
- **/resources/demo_scene.xml** – описание демо-сцены;
- **/resources/resources.xml** – описание всех объектов сервера (здания, транспорт и пр.);
- **/resources/roofs.xml** – связь пресетов крыши с зданиями;
- **/resources/variants_list.xml** – список вариантов объектов для пресетов;
- **/meshes/\*** – геометрия объектов, необходима для вычисления OBB;
- **/roof_presets/\*** – все возможные варианты крыши;
- **/area_presets/\*** – файлы всех кварталов.

---

## Настройки сервера
Для запуска сервера необходим файл **server_settings.xml**.

Раздел        | Параметр      | Описание                                
--------------|---------------|-----------------------------------------
Net           | IP            | IP адрес сетевого интерфейса сервера.
Net           | Port          | Порт, с которым "связывается" сервер. По умолчанию 27000
Net           | Ping          | Искусственная задержка между запросом клиента и ответом сервера. Атрибуты ```min``` и ```max``` задают интервал задержки в миллисекундах. По умолчанию ```min=0```, ```max=0```.
Net           | ShowClients   | Вывод информации о клиентах, если атрибут ```value="1"```. Подробнее о таблице клиентов описано в следующем параграфе.
City          | Size          | Размеры города ```width``` и ```height```. Минимальный размер 4x4. Если останется лишняя часть области, то она будет отброшена.
City          | ShowField     | Параметр, необходимый для отладки. Выводится схематичное поле города, с указанием дорог, перекрестов и кварталов. Для вывода поля на экран необходимо задать атрибут ```value="1"```.
Command       | ObjectsLimit  | Данный параметр позволяет ограничить количество объектов, возвращаемых командой 2 (получить ближайшие объекты по радиусу и координатам).
Probabilities | Hydrant       | Вероятность появления пожарного гидранта на каждой улице (дорога от перекрёстка до перекрёстка). Атрибут ```0.0 <= probability <= 1.0```.
Probabilities | Roof          | Вероятность появления крыши здания. Атрибут ```0.0 <= probability <= 1.0```.

---

## Информация о клиентах
Информация о клиентах отображается в виде таблицы. Описание столбцов приведено ниже:
- **IP** – IP-адрес клиента;
- **data** – общее количество принятых и отправленных данных в Кб;
- **speed** – скользящее среднее скорости передачи и отправки данных за последние 10 секунд;
- **ping** – время прошедшее между приёмом и ответом для последнего запроса в миллисекундах;
- **iptime** – время с момента подключения клиента в секундах.