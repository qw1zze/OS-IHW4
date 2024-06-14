# OS_IHW4
## Калякин Дмитрий Александрович, БПИ226

#### Вариант 8: Задача об обедающих философах. 
Это классическая задача на взаимодействие параллельных процессов. Пять философов сидят возле круглого стола. Они проводят жизнь, чередуя приемы пищи и размышления. В центре стола находится большое блю- до спагетти. Спагетти длинные и запутанные, философам тяжело управляться с ними, поэтому каждый из них, что бы поесть, дол- жен пользоваться двумя вилками. К несчастью, философам дали только пять вилок. Между каждой парой философов лежит од- на вилка. Поэтому эти высококультурные и предельно вежливые люди договорились, что каждый будет пользоваться только теми вилками, которые лежат рядом с ним (слева и справа).

### Сущности:

Клиенты - каждый клиент является философом. Когда философ в состоянии приема пищи резервирует две вилки.

Сервер - отражает стол с вилками. Сервер реализует возможность оперировать общими данными, в данном случае вилками.

Монитор - клиент, получающий информацию и состоянии вилок.

### Взаимодействие
Сервер запускается и занимается прослушкой на заданном IP-адресе и порту. Клиенты подключаются к серверу и начинают симулировать работу философов. Сервер получает данные и занимается их обработкой блокируя и разблокируя "вилки". Мониторы отслеживают, что происходит на сервере.

#### IP-адрес и порт сервера задаются через командную строку при запуске программ. Приложение корректно работает в распределенном режиме.

### Команды для запуска программ после их компиляции:

#### 4_5:

./server 12345

./client 0 127.0.0.1 12345

Передаются ip-адрес и порт. При запуске клиента передается также id философа (в диапазоне от 0 до 4 включительно).

#### 6_7:

./server 12345

./client 0 127.0.0.1 12345

./monitor 127.0.0.1 12345

Передаются те же параметры, при запуске монитора передаются также ip-адрес и порт.

#### 8

./server 12345

./client 0 127.0.0.1 12345

./monitor 127.0.0.1 12345 (Мониторы могут быть в нескольких экземплярах)

#### Для каждого процесса приведен пример выходных данных, находящийся в одноменных файлах, лежащих в каждой директории.

### 4-5 баллов:
Сначала запускается сервер, после запускаются пять независимых процессов клиентов. Каждый клиент выводит информацию о состоянии программы. Сервер выводит информацию о занятых и освобожденных вилках. Сервер работает бесконечно до завершения с помощью ctrl + с. Клиенты завершаются с помощью ctrl + c, не нарушая работы программы и не блокируя вилки. После завершения работы клиента его можно заново запустить.

### 6-7 баллов:
Сначала запускается сервер, после запускаются пять независимых процессов клиентов и монитор. Клиенты и сервер выводит ту же информацию. Монитор выводит состояние вилок, где 1 - свободна, 0 - занята. Завершение монитора производится через ctrl + с. После чего можно заново его запустить. Монитор не влияет на работу сервера и сервер продолжает свою работу в независимости от работы монитора.

### 8 баллов:
Программы работают идентично, сервер поддерживает запуск и подключение нескольких мониторов, каждый из которых выводит информацию о вилках. Мониторы можно отключать с помошью Ctrl + c и обратно включать, не нарушая работу сервера.
