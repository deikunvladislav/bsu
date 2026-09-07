-- ----------------------------------------------------------
-- Отчёт 1: Аналитическая ведомость выполнения обязательств
-- ----------------------------------------------------------
DECLARE @ReportDate DATE = '2024-09-30';  -- <-- ДАТA

WITH enterprise_data AS (
    SELECT
        e.client_id,
        e.client_name,
        ISNULL(SUM(l.LA_amount), 0) AS issued_credits,
        ISNULL(SUM(p.paid_amount), 0) AS paid_total
    FROM client e
    LEFT JOIN LA l ON e.client_id = l.client_id AND l.LA_signement_date <= @ReportDate
    LEFT JOIN (
        SELECT LA_id, SUM(payment_amount) AS paid_amount
        FROM payment
        WHERE payment_date <= @ReportDate
        GROUP BY LA_id
    ) p ON l.LA_id = p.LA_id
    GROUP BY e.client_id, e.client_name
),
debt_details AS (
    SELECT
        e.client_id,
        ISNULL(SUM(CASE WHEN dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term) 
                              - ISNULL(p.paid_amount, 0) > 0 
                         THEN l.LA_amount 
                         ELSE 0 END), 0) AS not_repaid,
        ISNULL(SUM(
            dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term) 
            - ISNULL(p.paid_amount, 0)
        ), 0) AS total_debt
    FROM client e
    LEFT JOIN LA l ON e.client_id = l.client_id AND l.LA_signement_date <= @ReportDate
    LEFT JOIN (
        SELECT LA_id, SUM(payment_amount) AS paid_amount
        FROM payment
        WHERE payment_date <= @ReportDate
        GROUP BY LA_id
    ) p ON l.LA_id = p.LA_id
    GROUP BY e.client_id
)
SELECT
    ed.client_name AS [Предприятие],
    ed.issued_credits AS [Выдано кредитов],
    ed.paid_total AS [Оплачено],
    dd.not_repaid AS [Не погашено],
    dd.total_debt AS [Задолженность]
FROM enterprise_data ed
JOIN debt_details dd ON ed.client_id = dd.client_id

UNION ALL

SELECT
    'ИТОГО',
    SUM(ed.issued_credits),
    SUM(ed.paid_total),
    SUM(dd.not_repaid),
    SUM(dd.total_debt)
FROM enterprise_data ed
JOIN debt_details dd ON ed.client_id = dd.client_id;

GO

-- ----------------------------------------------------------
-- Отчёт 2: Предприятия-должники
-- ----------------------------------------------------------
DECLARE @ReportDate DATE = '2024-09-30'; -- <-- ДАТА 

SELECT
    [Предприятие],
    [№],
    [Дата],
    [Сумма кредита],
    [Годовой %],
    [Срок погашения, месяцы],
    [Должно быть получено],
    [Получено банком],
    [Задолженность банку]
FROM (
    SELECT
        c.client_name AS [Предприятие],
        l.LA_id AS [№],
        l.LA_signement_date AS [Дата],
        l.LA_amount AS [Сумма кредита],
        l.LA_annual_rate AS [Годовой %],
        l.LA_term AS [Срок погашения, месяцы],
        dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term) AS [Должно быть получено],
        ISNULL(SUM(p.payment_amount), 0) AS [Получено банком],
        dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term)
            - ISNULL(SUM(p.payment_amount), 0) AS [Задолженность банку],
        1 AS sort_priority
    FROM LA l
    JOIN client c ON l.client_id = c.client_id
    LEFT JOIN payment p ON l.LA_id = p.LA_id
         AND p.payment_date <= @ReportDate
    WHERE l.LA_signement_date <= @ReportDate
    GROUP BY
        c.client_name,
        l.LA_id,
        l.LA_signement_date,
        l.LA_amount,
        l.LA_annual_rate,
        l.LA_term
    HAVING dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term)
             - ISNULL(SUM(p.payment_amount), 0) > 0

    UNION ALL

    SELECT
        'ИТОГО',
        NULL,
        NULL,
        SUM(l.LA_amount),
        NULL,
        NULL,
        SUM(dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term)),
        SUM(paid.total_paid),
        SUM(dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term) - ISNULL(paid.total_paid, 0)),
        2
    FROM LA l
    JOIN client c ON l.client_id = c.client_id
    LEFT JOIN (
        SELECT LA_id, SUM(payment_amount) AS total_paid
        FROM payment
        WHERE payment_date <= @ReportDate
        GROUP BY LA_id
    ) paid ON l.LA_id = paid.LA_id
    WHERE l.LA_signement_date <= @ReportDate
      AND dbo.amount_to_return(l.LA_amount, l.LA_annual_rate, l.LA_term) - ISNULL(paid.total_paid, 0) > 0
) AS t
ORDER BY
    sort_priority,
    [Предприятие],
    [Дата];

-- --------------------------------------------------------------------
-- Запрос 1: перенос дат подписания с выходных на блиайшие рабочие дни
-- --------------------------------------------------------------------

    UPDATE LA
SET LA_signement_date = 
    CASE DATEPART(WEEKDAY, LA_signement_date)
        WHEN 7 THEN DATEADD(DAY, 2, LA_signement_date)   -- суббота → понедельник
        WHEN 1 THEN DATEADD(DAY, 1, LA_signement_date)   -- воскресенье → понедельник
        ELSE LA_signement_date
    END
WHERE DATEPART(WEEKDAY, LA_signement_date) IN (1, 7);

-- -------------------------------------------------------------------------------------------------------
-- Запрос 2: корректировка сумму кредита( например клиент через неделю обратился с просьбой добавить 20%)
-- -------------------------------------------------------------------------------------------------------

UPDATE LA
SET LA_amount = LA_amount * 1.2    -- увеличиваем на 20%
WHERE LA_id = 1009
  AND NOT EXISTS (SELECT 1 FROM payment WHERE payment.LA_id = LA.LA_id);

-- -------------------------------------------------------------------------------------------------------
-- Запрос 3: запрос на обновление условий шаблонного договора
-- -------------------------------------------------------------------------------------------------------

  UPDATE LAT
SET LAT_term = 8,
    LAT_annual_rate = 16.00
WHERE LAT_id = 104;

-- -------------------------------------------------------------------------------------------------------
-- Запрос 4: выдача нового кредита
-- -------------------------------------------------------------------------------------------------------

INSERT INTO LA (LA_id, client_id, LAT_id, LA_annual_rate, LA_term, LA_amount, LA_signement_date)
SELECT 
    1011,                         -- новый уникальный номер договора
    2,                            -- client_id (ЗАО "СтройИнвест")
    LAT.LAT_id,
    LAT.LAT_annual_rate,          -- фиксируем ставку из шаблона
    LAT.LAT_term,                 -- фиксируем срок из шаблона
    750000.00,                    -- сумма кредита
    GETDATE()                     -- дата подписания – сегодня
FROM LAT
WHERE LAT.LAT_id = 104;           -- выбран типовой договор №104 (9 мес., 14%)

-- ============================================================
-- ОТКАТ ИЗМЕНЕНИЙ
-- ============================================================

-- Откат запроса 4: удаление только что выданного кредита
DELETE FROM LA WHERE LA_id = 1011;

-- Откат запроса 3: возврат исходных условий типового договора 104
UPDATE LAT
SET LAT_term = 9,
    LAT_annual_rate = 14.00
WHERE LAT_id = 104;

-- Откат запроса 2: возврат суммы кредита 1009 (было 75 000)
UPDATE LA
SET LA_amount = 75000.00
WHERE LA_id = 1009;

-- Откат запроса 1: восстановление истинных дат подписания (1002 и 1006)
UPDATE LA
SET LA_signement_date = '2024-03-10'
WHERE LA_id = 1002;

UPDATE LA
SET LA_signement_date = '2024-06-01'
WHERE LA_id = 1006;