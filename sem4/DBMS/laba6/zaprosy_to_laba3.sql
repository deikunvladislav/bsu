-- 1. Многотабличный запрос
SELECT 
    p.Fullname AS PatientName,
    d.Fullname AS DoctorName,
    v.Date,
    v.Description,
    v.Cost
FROM Visits v
INNER JOIN Patients p ON v.Patient_ID = p.Patient_ID
INNER JOIN Doctors d ON v.Doctor_ID = d.Doctor_ID
WHERE YEAR(v.Date) = 2023
ORDER BY v.Date DESC;
GO

-- 2. Вычисляемые поля
SELECT 
    p.Fullname,
    p.Birthdate,
    DATEDIFF(YEAR, p.Birthdate, GETDATE()) AS Age,
    v.Cost,
    v.Cost * 1.2 AS CostWithVAT
FROM Patients p
JOIN Visits v ON p.Patient_ID = v.Patient_ID;
GO

-- 3. Внешнее объединение
SELECT 
    p.Fullname AS Patient,
    d.Fullname AS PreferredDoctor
FROM Patients p
LEFT JOIN Doctors d ON p.Preffered_Doctor = d.Doctor_ID;
GO

-- 4. Группировка с HAVING
SELECT 
    p.Patient_ID,
    p.Fullname,
    SUM(v.Cost) AS TotalCost
FROM Patients p
JOIN Visits v ON p.Patient_ID = v.Patient_ID
GROUP BY p.Patient_ID, p.Fullname
HAVING SUM(v.Cost) > 10000
ORDER BY TotalCost DESC;
GO

-- 5. Добавление записи
INSERT INTO Doctors (Fullname, Adress, Birthdate, Phone)
VALUES ('Соколова Ирина Викторовна', 'г. Москва, ул. Вятская, д. 10', '1988-05-19', '+7(495)567-89-01');
GO

-- 6. Удаление записи (только если нет зависимостей)
DELETE FROM Doctors 
WHERE Fullname = 'Соколова Ирина Викторовна' 
  AND Doctor_ID NOT IN (SELECT Doctor_ID FROM Visits)
  AND Doctor_ID NOT IN (SELECT Preffered_Doctor FROM Patients WHERE Preffered_Doctor IS NOT NULL);
GO

-- 7. Обновление (в транзакции с откатом)
BEGIN TRANSACTION;
UPDATE Visits SET Cost = Cost * 1.10 WHERE Doctor_ID = 1;
ROLLBACK TRANSACTION;
GO

-- 8. Создание таблицы на основе существующей (с предварительным удалением)
DROP TABLE IF EXISTS Doctors_Backup;
SELECT * INTO Doctors_Backup FROM Doctors;
GO

-- 9. UNION
SELECT Fullname, 'Doctor' AS Type FROM Doctors
UNION
SELECT Fullname, 'Patient' FROM Patients
ORDER BY Type, Fullname;
GO

-- 10. Вложенный запрос
SELECT DISTINCT p.Fullname
FROM Patients p
WHERE p.Patient_ID IN (
    SELECT v.Patient_ID
    FROM Visits v
    WHERE v.Cost > (SELECT AVG(Cost) FROM Visits)
);
GO

-- 11. Создание новой таблицы (с нуля)
DROP TABLE IF EXISTS TreatmentCategories;
CREATE TABLE TreatmentCategories (
    CategoryID int IDENTITY(1,1) PRIMARY KEY,
    CategoryName varchar(50) NOT NULL,
    MinCost decimal(9,2),
    MaxCost decimal(9,2)
);
GO

-- 12. Создание индекса (с предварительным удалением)
DROP INDEX IF EXISTS IX_Visits_Date ON Visits;
CREATE INDEX IX_Visits_Date ON Visits([Date]);
GO

-- 13. Представление (с предварительным удалением)
DROP VIEW IF EXISTS VisitsDetails;
GO
CREATE VIEW VisitsDetails AS
SELECT 
    v.Visit_ID,
    p.Fullname AS PatientName,
    d.Fullname AS DoctorName,
    v.Date,
    v.Description,
    v.Paymentform,
    v.Cost
FROM Visits v
INNER JOIN Patients p ON v.Patient_ID = p.Patient_ID
INNER JOIN Doctors d ON v.Doctor_ID = d.Doctor_ID;
GO

-- Проверка представления
SELECT * FROM VisitsDetails WHERE Date >= '2024-01-01';
GO