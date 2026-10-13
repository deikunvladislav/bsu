# Лабораторная работа №4

**Тема:** Создание базы данных интернет-магазина косметики и написание SQL-запросов.

## Цель работы
Изучить создание таблиц с ограничениями целостности, индексами, внешними ключами, а также освоить написание аналитических запросов (JOIN, GROUP BY, фильтрация, сортировка).

## Задание
1. Создать базу данных `golden_apple`.
2. Реализовать таблицы:
   - `users` — пользователи;
   - `supliers` — поставщики;
   - `orders` — заказы;
   - `products` — товары;
   - `sales` — продажи (позиции заказов).
3. Настроить первичные и внешние ключи, ограничения `CHECK`, `DEFAULT`, каскадные операции.
4. Создать индексы для ускорения поиска.
5. Заполнить таблицы тестовыми данными.
6. Написать набор SQL-запросов для выборки и анализа данных.

## Структура базы данных

| Таблица    | Ключевые поля и связи |
|------------|------------------------|
| `users`    | `user_id` (PK), `user_name`, `email`, `phone_number` |
| `supliers` | `suplier_id` (PK), `suplier_name`, `phone_number`, `adress` |
| `orders`   | `order_id` (PK), `user_id` (FK → users), `order_date`, `status`, `order_amount` |
| `products` | `product_id` (PK), `product_name`, `category`, `price`, `barcode`, `suplier_id` (FK → supliers) |
| `sales`    | `sale_id` (PK), `order_id` (FK → orders), `product_id` (FK → products), `quantity`, `sales_price` |

## Что реализовано
- Создание 5 таблиц с ограничениями: `PRIMARY KEY`, `FOREIGN KEY`, `CHECK`, `DEFAULT`.
- Каскадные операции: `ON UPDATE CASCADE`, `ON DELETE CASCADE`, `ON DELETE SET NULL`, `ON DELETE NO ACTION`.
- Индексы по полям `email`, `user_id`, `order_date`, `suplier_id`, `category`, `order_id`, `product_id`, составной индекс `(order_id, product_id)`.
- Заполнение таблиц тестовыми данными (пользователи, поставщики, товары, заказы, продажи).
- Написаны SQL-запросы:
  - простая выборка (`SELECT *`);
  - фильтрация (`WHERE`, `LIKE`, `AND`);
  - сортировка (`ORDER BY`);
  - соединение таблиц (`INNER JOIN`, `LEFT JOIN`);
  - группировка и агрегация (`GROUP BY`, `SUM`, `COUNT`).

## Файлы
- `golden_apple.sql` — создание БД, таблиц, индексов и вставка данных.
- `zaprosy_primery.sql` — примеры аналитических запросов.

## Как запустить
1. Открыть `golden_apple.sql` в MS SQL Server Management Studio и выполнить (F5).
2. Открыть `zaprosy_primery.sql` и выполнить — будут показаны результаты выборок.