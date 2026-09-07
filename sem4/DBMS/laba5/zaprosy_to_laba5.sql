DROP VIEW IF EXISTS order_details;
GO

CREATE VIEW order_details AS
SELECT o.order_id, u.user_name, o.order_date, o.status, p.product_name, s.quantity, s.sales_price
FROM orders o
INNER JOIN users u ON o.user_id = u.user_id
INNER JOIN sales s ON o.order_id = s.order_id
INNER JOIN products p ON s.product_id = p.product_id;
GO

SELECT o.order_id, u.user_name, o.order_date, o.status, o.order_amount
FROM orders o
INNER JOIN users u ON o.user_id = u.user_id
WHERE o.status = 'delivered' AND o.order_amount > 50
ORDER BY o.order_date DESC;

SELECT s.sale_id, p.product_name, s.quantity, s.sales_price,
       s.quantity * s.sales_price AS total_cost
FROM sales s
INNER JOIN products p ON s.product_id = p.product_id;

SELECT u.user_name, o.order_id, o.order_date, o.order_amount
FROM users u
LEFT JOIN orders o ON u.user_id = o.user_id;

SELECT p.product_name, s.sale_id, s.quantity, s.sales_price
FROM products p
LEFT JOIN sales s ON p.product_id = s.product_id;

SELECT p.category, 
       SUM(s.quantity) AS total_quantity,
       SUM(s.quantity * s.sales_price) AS total_revenue
FROM products p
INNER JOIN sales s ON p.product_id = s.product_id
GROUP BY p.category
HAVING SUM(s.quantity * s.sales_price) > 100;

INSERT INTO users (user_name, email, phone_number)
VALUES ('test_user', 'test@example.com', '+375291234567');

INSERT INTO orders (user_id, order_date, status, order_amount)
VALUES (1, GETDATE(), 'new', 10.00);

DELETE FROM orders WHERE user_id = 1;

DELETE FROM orders WHERE order_amount = 10.00;

DELETE FROM users WHERE user_name = 'test_user';
GO

UPDATE orders SET status = 'completed' WHERE order_id = 1;

DROP TABLE IF EXISTS high_value_orders;
GO
SELECT * INTO high_value_orders
FROM orders
WHERE order_amount > 500;

SELECT user_name AS name, 'user' AS type FROM users
UNION
SELECT suplier_name, 'supplier' FROM supliers;

SELECT user_name
FROM users
WHERE user_id IN (
    SELECT user_id
    FROM orders
    GROUP BY user_id
    HAVING SUM(order_amount) > (SELECT AVG(order_amount) FROM orders)
);

DROP TABLE IF EXISTS order_summary;
GO
CREATE TABLE order_summary (
    summary_id INT IDENTITY(1,1) PRIMARY KEY,
    user_id INT,
    total_orders INT,
    total_amount DECIMAL(10,2)
);

DROP INDEX IF EXISTS IX_users_phone ON users;
GO
CREATE INDEX IX_users_phone ON users(phone_number);