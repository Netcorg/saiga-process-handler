SELECT
    name,
    COUNT(*) AS session_count,
    SUM(duration) AS total_duration
FROM (
    SELECT
        app1.name,
        (app2.end_time - app1.start_time) AS duration
    FROM
        (SELECT pid, hwnd, name, time_tag AS start_time
         FROM process
         WHERE state = 1 AND time_tag > 0) AS app1
    JOIN
        (SELECT pid, hwnd, name, time_tag AS end_time
         FROM process
         WHERE state = 2 AND time_tag < 123456899) AS app2
    ON app1.pid = app2.pid
       AND app1.hwnd = app2.hwnd
       AND app1.name = app2.name
)

GROUP BY name
ORDER BY duration;