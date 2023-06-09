# Прервывания

После загрузки `sudo insmod my_tasklet.ko` необходимо показать линии IRQ.

`cat /proc/interupts`

Далее программа на первой строке, что это?

> Номер линии IRQ

Почему 1?

> На первой линии IRQ находится клавиатура

Как регистрируем?

> request\_rq

Что передаем?

Номер линии IRQ, адрес функции обработчика, во флаги передаем флаг IRQF\_SHARED для возможности разделения линии IRQ, название такслета, и dev id.

Зачем это делать?

Для того, чтобы можно было осовобдить линии IRQ?

Что делает обработчик?

Получает код клавиши клавиатуры и выводит ее. И устанаваливает планировщик.

## WorkQueue

Загружаем `sudo insmod myworkqueue.ko` и также показываем линию IRQ, она разделяется.

Что мы деалем при ините workqueue?

request, alloc очереди, выделяем память для структур.

В обработчике прерываний ставим в очередь.

Кто будет отвечать за ее выполнение в системе?

> системный демон kworker

Что это?

> Поток

Сравнить sofirq, очереди и такслекты:

1. Регистрация: sofirq статически загружено в ядро для изменения необходимо перекомпилировать ядро, очереди и такслеты могут быть зарегитсрированы как статически так и динамически.
2. Такслет не может параллельно выполняться параллельно, а такслет и очереди могут. То есть такслеты не могут блокироваться, а очереди могут.
3. Таклеты нет сериализуются, очереди да, софтирк - нет.


Статически --- это на стадии компиляции

Динамически --- это на стадии выполнения

4. После того, как такслет запланирован, он выполняется только один раз.

Спинлок блокировака - это активное ожидание на процессоре и в такслетах мы для взаимослючения используем спинлоки.
