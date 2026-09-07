select * from users;

SELECT * FROM users WHERE user_name = 'jopacotik';

SELECT * FROM users WHERE email LIKE '%gmail%';

SELECT * FROM orders 
WHERE status = 'delivered' AND order_amount > 50;

select * from orders order by order_date desc;

SELECT o.order_id, u.user_name, o.order_date, status,  o.order_amount
FROM orders o
JOIN users u ON o.user_id = u.user_id;

SELECT s.sale_id, o.order_id, p.product_name, s.quantity, s.sales_price
FROM sales s
JOIN orders o ON s.order_id = o.order_id
JOIN products p ON s.product_id = p.product_id;

SELECT p.category, SUM(s.quantity * s.sales_price) AS total_sales
FROM sales s
JOIN products p ON s.product_id = p.product_id
GROUP BY p.category;

SELECT u.user_name, COUNT(o.order_id) AS order_count
FROM users u
LEFT JOIN orders o ON u.user_id = o.user_id
GROUP BY u.user_name;