SELECT
    app_name,
    COUNT(*) AS session_count,
    SUM(duration) AS total_duration
FROM (
    SELECT
        app1.app_name,
        (app2.end_time - app1.start_time) AS duration
    FROM
        (SELECT pid, ppid, app_name, time AS start_time
         FROM process
         WHERE state = 1 AND time > 0) AS app1
    JOIN
        (SELECT pid, ppid, app_name, time AS end_time
         FROM process
         WHERE state = 2 AND time < 123456899) AS app2
    ON app1.pid = app2.pid
       AND app1.ppid = app2.ppid
       AND app1.app_name = app2.app_name
)

GROUP BY app_name
ORDER BY duration;