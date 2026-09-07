drop table if exists payment;
drop table if exists LA;
drop table if exists LAT;
drop table if exists client;

create table client(
   client_id int primary key,
   client_name char(100) not null
);

-- loan_agreement_template= LAT--
create table LAT(
   LAT_id int primary key,
   LAT_term int not null check( LAT_term>0 and LAT_term<=12),
   LAT_annual_rate decimal(5,2) not null check (LAT_annual_rate>0 and LAT_annual_rate<50)
);

-- loan agreement= LA--
create table LA(
   LA_id int primary key,
   client_id int,
   foreign key (client_id) references client(client_id),
   LAT_id int,
   foreign key (LAT_id) references LAT(LAT_id),
   LA_annual_rate decimal(5,2) not null check(LA_annual_rate>0 and LA_annual_rate<50) ,
   LA_term int not null check(LA_term>0 and LA_term<=12),
   LA_amount decimal(15,2) not null check(LA_amount>0),
   LA_signement_date date not null default getdate()
);

create table payment(
   payment_id int identity(1,1) primary key,
   LA_id int,
   foreign key (LA_id) references LA(LA_id),
   payment_date date not null default getdate(),
   payment_amount decimal(15,2) not null check(payment_amount>0)
);

drop index if exists ix_payment_date on payment;
create index ix_payment_date on payment(payment_date);

-- =====================================================
-- 1. Клиенты (предприятия)
-- =====================================================
INSERT INTO client (client_id, client_name) VALUES
(1, 'ООО "Рассвет"'),
(2, 'ЗАО "СтройИнвест"'),
(3, 'ИП Кузнецов А.В.'),
(4, 'АО "ТехноПром"'),
(5, 'ООО "АгроМир"'),
(6, 'ПАО "ЭнергоСбыт"');

-- =====================================================
-- 2. Типовые договоры (LAT)
-- =====================================================
INSERT INTO LAT (LAT_id, LAT_term, LAT_annual_rate) VALUES
(101, 3, 12.50),
(102, 6, 15.00),
(103, 12, 18.50),
(104, 9, 14.00),
(105, 4, 11.00);

-- =====================================================
-- 3. Договоры (LA)
-- =====================================================
INSERT INTO LA (LA_id, client_id, LAT_id, LA_annual_rate, LA_term, LA_amount, LA_signement_date) VALUES
-- Предприятие 1: три договора с разным состоянием
(1001, 1, 102, 15.00, 6, 500000.00, '2024-01-15'),
(1002, 1, 103, 18.50, 12, 300000.00, '2024-03-10'),
(1009, 1, 105, 11.00, 4,  75000.00, '2024-04-01'),

-- Предприятие 2: два договора
(1003, 2, 103, 18.50, 12, 1200000.00, '2024-02-01'),
(1010, 2, 104, 14.00, 9,  600000.00, '2024-05-01'),

-- Предприятие 3: должник без платежей
(1004, 3, 101, 12.50, 3, 150000.00, '2024-05-20'),

-- Предприятие 4: просроченный и текущий
(1005, 4, 104, 14.00, 9, 800000.00, '2024-01-10'),
(1006, 4, 105, 11.00, 4, 200000.00, '2024-06-01'),

-- Предприятие 5: полностью погашен (возможна переплата)
(1007, 5, 102, 15.00, 6, 450000.00, '2024-02-15'),

-- Предприятие 6: небольшой кредит с одним платежом
(1008, 6, 101, 12.50, 3, 100000.00, '2024-07-01');

-- =====================================================
-- 4. Платежи (payment)
-- =====================================================
INSERT INTO payment (LA_id, payment_date, payment_amount) VALUES
-- Договор 1001 (почти погашен)
(1001, '2024-02-15', 100000.00),
(1001, '2024-03-15', 120000.00),
(1001, '2024-04-15', 130000.00),
(1001, '2024-05-15', 140000.00),

-- Договор 1002 (частично)
(1002, '2024-04-10', 40000.00),
(1002, '2024-05-10', 45000.00),

-- Договор 1003 (крупные суммы)
(1003, '2024-03-01', 150000.00),
(1003, '2024-04-01', 150000.00),
(1003, '2024-05-01', 200000.00),
(1003, '2024-06-01', 200000.00),
(1003, '2024-07-01', 100000.00),

-- Договор 1004 – 0 платежей

-- Договор 1005 (просрочен, частично оплачен)
(1005, '2024-02-10', 100000.00),
(1005, '2024-03-10', 100000.00),
(1005, '2024-04-10', 50000.00),

-- Договор 1006 (платежи по графику)
(1006, '2024-07-01', 55000.00),
(1006, '2024-08-01', 55000.00),

-- Договор 1007 (полностью погашен, с переплатой)
(1007, '2024-03-15', 200000.00),
(1007, '2024-04-15', 250000.00),
(1007, '2024-05-15', 50000.00),

-- Договор 1008 (один платёж)
(1008, '2024-07-15', 35000.00),

-- Договор 1009 – без платежей

-- Договор 1010 (один платёж)
(1010, '2024-06-01', 100000.00);


drop function if exists dbo.get_monthly_interest;
drop function if exists dbo.get_total_interest;
drop function if exists dbo.amount_to_return;
go
-- functions used for creating reports --
create function dbo.get_monthly_interest( @amount decimal(15,2), @rate decimal(5,2) ) returns decimal(15,2) as
begin
   return @amount*@rate/1200.;
end;
go

create function dbo.get_total_interest(@amount decimal(15,2), @rate decimal(5,2), @term int) returns decimal(15,2) as
begin
   return dbo.get_monthly_interest(@amount, @rate)*@term;
end;
go

create function dbo.amount_to_return(@amount decimal(15,2), @rate decimal(5,2), @term int) returns decimal(15,2) as
begin
   return dbo.get_total_interest(@amount, @rate, @term) + @amount;
end;
go