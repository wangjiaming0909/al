- mysql vairiables:
    mysql> show variables like 'gtid%'; ```
    +----------------------------------+-----------+
    | Variable_name                    | Value     |
    +----------------------------------+-----------+
    | gtid_executed                    |           |
    | gtid_executed_compression_period | 1000      |
    | gtid_mode                        | ON        |
    | gtid_next                        | AUTOMATIC |
    | gtid_owned                       |           |
    | gtid_purged                      |           |
    +----------------------------------+-----------+
    6 rows in set (0.00 sec)
    ```

    show slave status\G

    mysql> show slave hosts; "to see slaves of current master
    ```
    +-----------+------+-------+-----------+--------------------------------------+
    | Server_id | Host | Port  | Master_id | Slave_UUID                           |
    +-----------+------+-------+-----------+--------------------------------------+
    |         3 |      | 13002 |         1 | 0ceeca3f-66a0-11ea-b18c-e454e8b0c44f |
    |         2 |      | 13001 |         1 | 0ce87e43-66a0-11ea-b186-e454e8b0c44f |
    +-----------+------+-------+-----------+--------------------------------------+
    2 rows in set (0.00 sec)
    ```

    mysql> show processlist\G "Binlog means slave connections in master
    -----on a master
    ```
    *************************** 3. row ***************************
       Id: 4
       User: root
       Host: localhost:48974
       db: NULL
    Command: Binlog Dump
       Time: 16054
      State: Master has sent all binlog to slave; waiting for more updates
       Info: NULL
    *************************** 4. row ***************************
       Id: 5
       User: root
       Host: localhost:48976
       db: NULL
    Command: Binlog Dump
       Time: 16054
      State: Master has sent all binlog to slave; waiting for more updates
       Info: NULL
    ```

    ----- on a slave, 
    two threads: 
      io thread read log from master, write to lay log
      sql thread to read lay log and execute sql commands
    ```
    *************************** 12. row ***************************
       Id: 23
       User: system user
       Host: 
       db: NULL
    Command: Connect
       Time: 16092
      State: Waiting for master to send event
       Info: NULL
    *************************** 13. row ***************************
       Id: 24
       User: system user
       Host: 
       db: NULL
    Command: Connect
       Time: 16041
      State: Slave has read all relay log; waiting for more updates
       Info: NULL
    ```
```
START TRANSACTION [transaction_characteristics...]: {
  WITH CONSISTENT SNAPSHOT
  | READ WRITE
  | READ ONLY
}
```
- WITH CONSISTENT SNAPSHOT
  start a consistent read (only to innodb) only valid when the isolation level is ```REPEATABLE READ```

disable auto commit ```SET autocommit=0;``` session variable
```
SAVEPOINT identifier
ROLLBACK [WORK] TO [SAVEPOINT] identifier
RELEASE SAVEPOINT identifier
```

```
LOCK TABLES
    tbl_name [[AS] alias] lock_type
    [, tbl_name [[AS] alias] lock_type] ...

lock_type: {
    READ [LOCAL]
  | [LOW_PRIORITY] WRITE
}

UNLOCK TABLES
```

