DROP TABLE IF EXISTS sales;
DROP TABLE IF EXISTS products; 
DROP TABLE IF EXISTS orders;
DROP TABLE IF EXISTS supliers;
DROP TABLE IF EXISTS users; 

create table users(
    user_id int primary key identity(1,1),
    user_name varchar(100) not null,
    email varchar(100) not null,
    phone_number varchar(20) not null,
    constraint ck_users_email CHECK (email LIKE '%@%'),
    constraint ck_users_phone_number CHECK (phone_number LIKE '+375%')
);
create index ix_users_email on users(email);

insert into users(user_name, email, phone_number) values
('jopacotik', 'danilappap@gmail.com', '+375293250045'),
('lexariofilini', 'lehakartoxa@mail.ru', '+375441758191'),
('i_van_hoe', 'vanya_pick_me_femboychick@gmail.com', '+375296767676'),
('valeriya', 'meschenko_lerchick@gmail.com', '+375296607804');

create table supliers(
    suplier_id int primary key identity(1,1),
    suplier_name varchar(100) not null,
    phone_number varchar(20) not null,
    constraint ck_supliers_phone_number CHECK (phone_number LIKE '+375%'),
    adress varchar(200) not null
);

insert into supliers( suplier_name, phone_number, adress) values
('ООО "Аромат"', '+375291579534', 'Москва, ул. Цветочная, 1'),
('ЗАО "Парфюм Трейд"', '+375441234567', 'Санкт-Петербург, Невский пр., 100'),
('Blue Cosmetics LTD', '+375332486351', 'London, Big Ben, 100'),
('La Roche-Posay', '+375257412586', 'Paris, Rue Royal, 14');

create table orders(
    order_id int primary key identity(1,1),
    user_id int,
    order_date date not null default getdate(),
    status varchar(20) default 'not completed',
    order_amount decimal(7,2) not null,
    constraint ck_order_amount check (order_amount >=0),
    foreign key (user_id) references users(user_id)
       on update cascade
       on delete cascade
);

create index ix_orders_user_id on orders(user_id);
create index ix_orders_order_date on orders(order_date);

insert into orders( user_id, order_date, status, order_amount) values
(1, '2025-11-29' , 'delivered' , 64.61),
(1, '2026-01-10', 'delivered', 850),
(2, '2026-02-07', 'new', 47.95),
(3, '2026-02-07', 'approved', 11.35),
(4, '2026-03-08', 'delivered', 59.05);

create table products(
    product_id int primary key identity(1,1),
    product_name varchar(100) not null,
    category varchar(50) not null,
    price decimal(6,2) not null,
    constraint ck_products_price check (price>0),
    barcode varchar(50) not null,
    suplier_id int,
    foreign key (suplier_id) references supliers(suplier_id)
       on update cascade
       on delete set null
);

create index ix_products_suplier_id on products(suplier_id);
create index ix_products_category on products(category);

insert into products(product_name, category, price, barcode, suplier_id) values
('bielenda skin clinic professional', 'skin care', 11.35, '123654789', 3),
('cicaplast b5+ baume', 'skin care', 64.61, '147852369',4),
('lipikar urea 30%', 'hand cream', 47.95, '951753852', 4),
('Chanel No.5 100ml', 'parfume', 850, '741852963', 1),
('bielita hairspray', 'hair cosmeics', 11.10,'846295137', 2);

create table sales(
    sale_id int primary key identity(1,1),
    order_id int,
    product_id int,
    quantity int,
    sales_price int,
    constraint ck_quantity check (quantity>= 1),
    constraint ck_sales_price check (sales_price>0),
    foreign key (order_id) references orders(order_id)
       on update cascade
       on delete cascade,
    foreign key (product_id) references products(product_id)
       on update cascade
       on delete no action
);

create index ix_sales_order_id on sales(order_id);
create index ix_sales_product_id on sales(product_id);
create index ix_sales_order_product on sales(order_id,product_id);

insert into sales(order_id, product_id, quantity, sales_price) values
(1, 2, 1, 64.61),
(2, 4, 1, 850), 
(4, 1, 1, 11.35),
(5, 5, 1, 11.10),
(5, 3, 1, 47.95);