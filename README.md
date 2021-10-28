**Настройка точности маятниковых часов**

Точность хода исправных маятниковых часов определяется длиной маятника. Для определения его длины требуется замеры отклонения хода часов от точного времени. В зависимости от отклонения маятник удлиняется или укорачивается. Обычно это делается в течении нескольких часов и "на глаз" корректируется длина, снова наблюдение в течении нескольких часов, уточнение в длине и т.д. до приемлемой погрешности.

Цель данного проекта сократить время корректировки длины маятника.

Для этого контроллер с помощью щелевого датчика определять полупериод качания маятника и выводить информацию для корректировки длины маятника.


**Использование в качестве сенсора щелевой датчик.**
Маятник часов имеет снизу ось с резьбой, на которую нанизан диск. На оси есть винт, которым регулируется длина. Как правило ось длинее и "торчит" ниже маятника и регулирующего винта. Для работы данного прибора требуется, чтобы ось проходила через щелевой датчик.
Опыт показал, щелевой датчик неудобен.

**Использование в качестве сенсора ИК датчик наличия преграды.**
Настроить положение датчика так, чтобы он "ловил" дальнего от центра край диска маятника в одном крайнем положении.
ИК датчик удобен. Через стекло может ловить. Металлическая поверхность диска маятника хорошо отражает ИК.


