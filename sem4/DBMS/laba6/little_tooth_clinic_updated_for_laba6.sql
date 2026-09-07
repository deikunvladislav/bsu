DROP TABLE IF EXISTS Visits;
DROP TABLE IF EXISTS Teeth;
DROP TABLE IF EXISTS Patients;
DROP TABLE IF EXISTS Doctors;

Create table Doctors(
Doctor_ID int identity(1,1) Primary key,
Fullname char(100) not null, 
Adress char(100) not null,
Birthdate date null, 
Phone char(20) not null,
);

Create table Patients(
Patient_ID int identity(1,1) Primary key,
Fullname char(100) Not null, 
Adress char(100) null,
Birthdate date null,
Preffered_Doctor int null,
Foreign key(Preffered_Doctor) References Doctors( Doctor_ID)
);

Create table Teeth(
Tooth_ID int identity(1,1) Primary key,
Patient_ID int,
Foreign key(Patient_ID) References Patients(Patient_ID),
State char(1)
);

CREATE TABLE Visits (
Visit_ID int identity(1,1) PRIMARY KEY,
Patient_ID int, 
Foreign key( Patient_ID) References Patients (Patient_ID),
[Date] date not null, 
Doctor_ID int, 
Foreign key( Doctor_ID) References Doctors (Doctor_ID),
[Description] char(300) not null,
Paymentform char(50) not null,
cost decimal(9,2) not null
);

INSERT INTO Doctors (Fullname, Adress, Birthdate, Phone)
VALUES 
('Петрова Анна Ивановна', 'г. Москва, ул. Тверская, д. 15', '1975-03-12', '+7(495)123-45-67'),
('Смирнов Олег Петрович', 'г. Москва, пр. Мира, д. 22', '1980-07-25', '+7(495)234-56-78'),
('Кузнецова Елена Владимировна', 'г. Москва, ул. Арбат, д. 8', '1985-11-03', '+7(495)345-67-89'),
('Морозов Дмитрий Алексеевич', 'г. Москва, ул. Ленинская, д. 5', '1970-12-17', '+7(495)456-78-90');

INSERT INTO Patients (Fullname, Adress, Birthdate, Preffered_Doctor)
VALUES
('Иванов Сергей Петрович', 'г. Москва, ул. Лесная, д. 3', '1990-04-10', 1),
('Козлова Мария Дмитриевна', 'г. Москва, ул. Садовая, д. 7', '1985-09-22', 2),
('Сидоров Алексей Владимирович', 'г. Москва, ул. Новая, д. 12', '2000-01-15', NULL),
('Васильева Ольга Игоревна', 'г. Москва, ул. Парковая, д. 9', '1995-06-30', 3),
('Новиков Павел Андреевич', 'г. Москва, ул. Речная, д. 2', '1982-11-11', 1),
('Фёдорова Екатерина Сергеевна', NULL, '1998-12-01', NULL);

INSERT INTO Teeth (Patient_ID, State)
VALUES
(1, 'G'), (1, 'C'), (1, 'G'), (1, 'M'),
(2, 'F'), (2, 'G'), (2, 'C'), (2, 'G'),
(3, 'G'), (3, 'G'), (3, 'C'),
(4, 'M'), (4, 'F'), (4, 'G'), (4, 'C'),
(5, 'G'), (5, 'G'), (5, 'G'),
(6, 'C'), (6, 'C'), (6, 'F');

INSERT INTO Visits (Patient_ID, Date, Doctor_ID, Description, Paymentform, Cost)
VALUES
(1, '2023-02-10', 1, 'Осмотр, лечение кариеса', 'card', 1500.00),
(1, '2023-05-20', 2, 'Удаление зуба', 'cash', 2500.00),
(1, '2024-01-15', 1, 'Плановый осмотр', 'card', 800.00),
(2, '2023-03-05', 2, 'Лечение пульпита', 'cash', 3200.00),
(2, '2023-09-12', 3, 'Профессиональная чистка', 'card', 1200.00),
(3, '2023-04-18', 1, 'Пломбирование', 'cash', 1800.00),
(3, '2023-11-22', 4, 'Осмотр', 'card', 600.00),
(3, '2024-02-28', 1, 'Лечение кариеса', 'cash', 2100.00),
(4, '2023-06-30', 3, 'Удаление зуба мудрости', 'card', 5000.00),
(4, '2023-12-10', 3, 'Имплантация', 'cash', 15000.00),
(4, '2024-03-01', 4, 'Контрольный осмотр', 'card', 900.00),
(5, '2022-11-05', 1, 'Лечение периодонтита', 'cash', 4200.00),
(5, '2023-01-20', 1, 'Установка коронки', 'card', 7800.00),
(5, '2023-07-15', 1, 'Осмотр', 'cash', 700.00),
(5, '2024-04-10', 2, 'Второе мнение', 'card', 1100.00),
(6, '2023-08-25', 2, 'Лечение двух зубов', 'cash', 3400.00),
(6, '2024-01-05', 3, 'Чистка', 'card', 950.00),
(2, '2024-02-14', 2, 'Косметическая реставрация', 'card', 12000.00);