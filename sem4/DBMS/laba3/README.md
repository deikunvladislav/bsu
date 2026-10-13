# Лабораторная работа №3

**Тема:** Создание базы данных стоматологической клиники и работа с таблицами в MS SQL Server.

## Цель работы
Изучить создание таблиц, определение связей (внешних ключей) и заполнение базы данных тестовыми данными.

## Задание
1. Создать базу данных `little_tooth_clinic`.
2. Реализовать таблицы:
   - `Doctors` — врачи;
   - `Patients` — пациенты;
   - `Teeth` — зубы пациентов;
   - `Visits` — посещения.
3. Настроить первичные и внешние ключи.
4. Заполнить таблицы тестовыми данными.
5. Выполнить простую выборку: `SELECT * FROM Teeth;`.

## Структура базы данных

- **Doctors**: `Doctor_ID`, `Fullname`, `Adress`, `Birthdate`, `Phone`.
- **Patients**: `Patient_ID`, `Fullname`, `Adress`, `Birthdate`, `Preffered_Doctor` (FK → Doctors).
- **Teeth**: `Tooth_ID`, `Patient_ID` (FK → Patients), `number`, `State`.
- **Visits**: `Visit_ID`, `Patient_ID` (FK → Patients), `Date`, `Doctor_ID` (FK → Doctors), `Description`, `Paymentform`, `cost`.

## Что сделано
- Написаны `DROP TABLE` для безопасного пересоздания.
- Созданы 4 таблицы с ограничениями `PRIMARY KEY`, `FOREIGN KEY`, `NOT NULL`.
- Добавлены данные:
  - 4 врача;
  - 6 пациентов;
  - 21 запись о зубах;
  - 18 посещений.
- Выполнен итоговый запрос `SELECT * FROM Teeth;`.

## Как запустить
1. Открыть скрипт `little_tooth_clinic.sql` в MS SQL Server Management Studio.
2. Выполнить его целиком (F5).
3. Убедиться, что таблицы созданы и заполнены, а последний запрос выводит содержимое таблицы `Teeth`.